#include "DataOwner.h"
// #include "Utils.h"
#include <string>
//#include <string.h> // memset(KF, 0, sizeof(KF));
#include "stdio.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <vector>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <cstring>
#include <openssl/rand.h>


using namespace std;

void setD(boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator,std::string w,std::bitset<GGM_SIZE> bits){


    std::stringstream ss;
    boost::property_tree::ptree pt;

    string finalW;

    bitset<8> bit_set;

    for(char c : w){
        bit_set = std::bitset<8>(c);
        finalW += bit_set.to_string();
    }

    pt.put("w", finalW);
    pt.put("bit_array", bits.to_string());

    // 将 property_tree 序列化为 JSON 字符串
    boost::property_tree::write_json(ss, pt);
    std::string json_str = ss.str();

    // 构建 HTTP GET 请求报文
    std::string request =
        "POST /setD HTTP/1.1\r\n"
        "Host: "+ string(BLOCKCHAIN_SERVICE_IP) +":"+ string(BLOCKCHAIN_SERVICE_PORT) +"\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + std::to_string(json_str.length()) + "\r\n"
        "\r\n" +
        json_str;

    // cout<<"SetD request:"<<endl;
    // cout<<request<<endl<<endl;
    // 发送 HTTP 请求报文
    send_http(io_service,endpoint_iterator,request);
    // cout<<"------------------"<<endl;
}

DataOwner::DataOwner(boost::asio::io_service *io_service,
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    tree = new GGMTree(GGM_SIZE);
    this->io_service = io_service;
    this->endpoint_iterator = endpoint_iterator;
}

void DataOwner::update(int ind,vector<string> WList,OP op){
    unordered_set<int> userList = AccessList[ind];
    vector<string> keyValues;
    int Cnt = 0;
    // cout<<"update : 1"<<endl;
    for(int userId : userList){
        // cout<<"update : 2"<<endl;
        UsersKey keys = UserKeys[userId];
        vector<string> cntEnc;
        vector<KeyValue> keyValues;
        vector<string> DelCntDiffs;
        vector<Revoketag> Revoketags;
        for(string w : WList){
            // cout<<"将关键字"<<w<<"加入到文件"<<ind<<"上"<<endl;
            // cout<<"update : 3"<<endl;
            // cout<<w<<endl;
            if(FileCnts.find(userId) == FileCnts.end()){
                FileCnts[userId] = unordered_map<string,int>();
                D[userId] = unordered_map<string,BloomFilter<32, GGM_SIZE, HASH_SIZE>>();
            }
            if(FileCnts[userId].find(w) == FileCnts[userId].end()){
                FileCnts[userId][w] = 0;
                D[userId][w] = BloomFilter<32, GGM_SIZE, HASH_SIZE>();
            }
            uint8_t buffer[w.size() + sizeof(int)];

            memcpy(buffer,w.c_str(),w.size());
            memcpy(buffer + w.size(),(uint8_t*)&ind, sizeof(int));
            uint8_t tag[DIGEST_SIZE];
            // uint8_t *tag;

            sha256_digest(buffer,w.size() + sizeof(int),tag);

            if(op == ADD){
                FileCnts[userId][w]++;
                int cnt = FileCnts[userId][w];
                // uint8_t plaintext[w.size() + sizeof(int)];
                memcpy(buffer,w.c_str(),w.size());
                memcpy(buffer + w.size(),(uint8_t*)&cnt, sizeof(int));
                uint8_t cipertext[w.size() + sizeof(int)];
                aes_encrypt(buffer,w.size() + sizeof(int),key,iv,cipertext);
                cntEnc.emplace_back(string((char *)cipertext,w.size()+sizeof(int)));
                
                //SRE加密
                // get all offsets in BF
                vector<long> indexes = BloomFilter<32, GGM_SIZE, HASH_SIZE>::get_index(tag);
                sort(indexes.begin(), indexes.end());
                // cout<<"布隆过滤器上的索引为："<<endl;
                // for(long index:indexes){
                //     cout<< index<<" "<<endl;
                // }
                // cout<<endl;

                // get SRE ciphertext list
                vector<string> ciphertext_list;
                for(long index : indexes) {
                    // derive a key from the offset
                    uint8_t derived_key[AES_BLOCK_SIZE];
                    memcpy(derived_key, key, AES_BLOCK_SIZE);
                    GGMTree::derive_key_from_tree(derived_key, index, tree->get_level(), 0);
                    // use the key to encrypt the id
                    uint8_t encrypted_id[AES_BLOCK_SIZE + sizeof(int)];
                    memcpy(encrypted_id, iv, AES_BLOCK_SIZE);
                    aes_encrypt((uint8_t*) &ind, sizeof(int),
                                derived_key, encrypted_id,
                                encrypted_id + AES_BLOCK_SIZE);
                    // save the encrypted id in the list
                    ciphertext_list.emplace_back(string((char*) encrypted_id, AES_BLOCK_SIZE + sizeof(int)));
                }
                // Val val;
                // val.ct = ciphertext_list;
                // val.tag = tag;

                // uint8_t addr[w.size() + sizeof(int)];
                memcpy(buffer,w.c_str(),w.size());
                memcpy(buffer + w.size(),(uint8_t*)&cnt,sizeof(int));
                uint8_t addr[DIGEST_SIZE];
                // uint8_t *tag;

                sha256_digest(buffer,w.size() + sizeof(int),addr);
                // keyValues.emplace_back(KeyValue(addr,val));
                keyValues.emplace_back(KeyValue(string((char*)addr,DIGEST_SIZE),Val(ciphertext_list,string((char*)tag,DIGEST_SIZE))));
                
                
            }else{
                // cout<<"DEL 1"<<endl;
                if(FileDelCnts.find(userId) == FileDelCnts.end()){
                    FileDelCnts[userId] = unordered_map<string,int>();
                }
                if(FileDelCnts[userId].find(w) == FileDelCnts[userId].end()){
                    FileDelCnts[userId][w] = 0;
                }
                FileDelCnts[userId][w] ++;
                int cnt1 = FileDelCnts[userId][w];
                memcpy(buffer,w.c_str(),w.size());
                memcpy(buffer + w.size(),(uint8_t*)&cnt1,sizeof(int));

                uint8_t cipertext[w.size() + sizeof(int)];
                aes_encrypt(buffer,w.size() + sizeof(int),key,iv,cipertext);

                DelCntDiffs.emplace_back(string((char *)cipertext,w.size() + sizeof(int)));

                //求摘要（G）
                memcpy(buffer,w.c_str(),w.size());
                memcpy(buffer + w.size(),(uint8_t*)&cnt1,sizeof(int));
                uint8_t addr[w.size() + sizeof(int)];

                aes_encrypt(buffer,w.size() + sizeof(int),key,iv,addr);
                // sha256_digest(buffer,w.size() + sizeof(int),addr);

                D[userId][w].add_tag(tag);
                vector<long> v = D[userId][w].get_index(tag);
                Revoketags.emplace_back(Revoketag(string((char*)addr,w.size() + sizeof(int)),D[userId][w]));
                // cout<<"DEL 2"<<endl;
            }

        }

        // cout<<"cntEnc : "<<endl;
        // for(string s : cntEnc){
        //     printHexBytes(s);
        //     uint8_t decryptdDiff[s.size()];
        //     aes_decrypt((unsigned char*)s.c_str(),s.size(),key,iv,decryptdDiff);
        //     string w1 = string((char *)decryptdDiff,s.size() - sizeof(int));
        //     cout<<"num:"<<*(int *)(decryptdDiff + s.size() - sizeof(int))<<endl;
        //     cout<<"w:"<<w1<<endl;
        // }


        if(op == ADD){
            server->addFile(ind,userId,cntEnc,keyValues);
        }else{
            server->delFile(userId,Revoketags,DelCntDiffs);
        }
    }
};

void DataOwner::insert(int ind,vector<string> WList){
    unordered_set<int> userList = AccessList[ind];
    vector<string> keyValues;
    int Cnt = 0;
    // cout<<"update : 1"<<endl;
    for(int userId : userList){
        // cout<<"update : 2"<<endl;
        UsersKey keys = UserKeys[userId];
        vector<string> cntEnc;
        vector<KeyValue> keyValues;
        vector<string> DelCntDiffs;
        vector<Revoketag> Revoketags;
        for(string w : WList){

            if(FileCnts.find(userId) == FileCnts.end()){
                FileCnts[userId] = unordered_map<string,int>();
                D[userId] = unordered_map<string,BloomFilter<32, GGM_SIZE, HASH_SIZE>>();
            }
            if(FileCnts[userId].find(w) == FileCnts[userId].end()){
                FileCnts[userId][w] = 0;
                D[userId][w] = BloomFilter<32, GGM_SIZE, HASH_SIZE>();
            }
            uint8_t buffer[w.size() + sizeof(int)];

            memcpy(buffer,w.c_str(),w.size());
            memcpy(buffer + w.size(),(uint8_t*)&ind, sizeof(int));
            uint8_t tag[DIGEST_SIZE];
            // uint8_t *tag;

            sha256_digest(buffer,w.size() + sizeof(int),tag);
            
            FileCnts[userId][w]++;
            int cnt = FileCnts[userId][w];
            // cout<<userId<<" "<<w<<" "<<cnt<<endl;
            // uint8_t plaintext[w.size() + sizeof(int)];
            memcpy(buffer,w.c_str(),w.size());
            memcpy(buffer + w.size(),(uint8_t*)&cnt, sizeof(int));
            uint8_t cipertext[w.size() + sizeof(int)];
            aes_encrypt(buffer,w.size() + sizeof(int),key,iv,cipertext);
            cntEnc.emplace_back(string((char *)cipertext,w.size()+sizeof(int)));
            
            //SRE加密
            // get all offsets in BF
            vector<long> indexes = BloomFilter<32, GGM_SIZE, HASH_SIZE>::get_index(tag);
            sort(indexes.begin(), indexes.end());
            // cout<<"keyword "<<w<<ind<<" 在布隆过滤器上的索引为："<<endl;
            // for(long index:indexes){
            //     cout<< index<<" "<<endl;
            // }
            // cout<<endl;

            // get SRE ciphertext list
            vector<string> ciphertext_list;
            for(long index : indexes) {
                // derive a key from the offset
                uint8_t derived_key[AES_BLOCK_SIZE];
                memcpy(derived_key, key, AES_BLOCK_SIZE);
                GGMTree::derive_key_from_tree(derived_key, index, tree->get_level(), 0);
                // use the key to encrypt the id
                uint8_t encrypted_id[AES_BLOCK_SIZE + sizeof(int)];
                memcpy(encrypted_id, iv, AES_BLOCK_SIZE);
                aes_encrypt((uint8_t*) &ind, sizeof(int),
                            derived_key, encrypted_id,
                            encrypted_id + AES_BLOCK_SIZE);
                // save the encrypted id in the list
                ciphertext_list.emplace_back(string((char*) encrypted_id, AES_BLOCK_SIZE + sizeof(int)));
            }

            memcpy(buffer,w.c_str(),w.size());
            memcpy(buffer + w.size(),(uint8_t*)&cnt,sizeof(int));
            uint8_t addr[DIGEST_SIZE];
            // uint8_t *tag;

            sha256_digest(buffer,w.size() + sizeof(int),addr);
            // keyValues.emplace_back(KeyValue(addr,val));
            keyValues.emplace_back(KeyValue(string((char*)addr,DIGEST_SIZE),Val(ciphertext_list,string((char*)tag,DIGEST_SIZE))));
        }

        server->addFile(ind,userId,cntEnc,keyValues);
    }
}

void DataOwner::revoke(string w,vector<int> IDList){
    if (batchCnts.find(w) == batchCnts.end()){
        batchCnts[w] = 1;
    }else{
        batchCnts[w]++;
    }
    int cnt = batchCnts[w];

    uint8_t buffer[w.size() + sizeof(int)];
    memcpy(buffer,w.c_str(),w.size());
    memcpy(buffer + w.size(),(uint8_t*)&cnt,sizeof(int));
    uint8_t addr[DIGEST_SIZE];

    sha256_digest(buffer,w.size() + sizeof(int),addr);

    BloomFilter<32, GGM_SIZE, HASH_SIZE> D;

    for(int ind : IDList){
        memcpy(buffer,w.c_str(),w.size());
        memcpy(buffer + w.size(),(uint8_t*)&ind, sizeof(int));
        uint8_t tag[DIGEST_SIZE];
        // uint8_t *tag;

        sha256_digest(buffer,w.size() + sizeof(int),tag);
        D.add_tag(tag);
    }
    setD(*io_service,endpoint_iterator,string((char*)addr,DIGEST_SIZE),D.bits);
    server->batch_revoke(w,cnt);
}