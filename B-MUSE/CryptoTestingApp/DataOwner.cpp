#include "DataOwner.h"

#include <string>
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

    // 发送 HTTP 请求报文
    send_http(io_service,endpoint_iterator,request);
}

DataOwner::DataOwner(boost::asio::io_service *io_service,
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    this->io_service = io_service;
    this->endpoint_iterator = endpoint_iterator;
}

void DataOwner::insert(int ind,vector<string> WList){
    unordered_set<int> userList = AccessList[ind];
    vector<string> keyValues;
    int Cnt = 0;

    for(int userId : userList){
        UsersKey keys = UserKeys[userId];
        vector<string> cntEnc;
        vector<KeyValue> keyValues;
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

            sha256_digest(buffer,w.size() + sizeof(int),tag);
            
            FileCnts[userId][w]++;
            int cnt = FileCnts[userId][w];
            memcpy(buffer,w.c_str(),w.size());
            memcpy(buffer + w.size(),(uint8_t*)&cnt, sizeof(int));
            uint8_t cipertext[w.size() + sizeof(int)];
            aes_encrypt(buffer,w.size() + sizeof(int),key,iv,cipertext);
            cntEnc.emplace_back(string((char *)cipertext,w.size()+sizeof(int)));
            
            //SRE加密
            vector<long> indexes = BloomFilter<32, GGM_SIZE, HASH_SIZE>::get_index(tag);
            sort(indexes.begin(), indexes.end());

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

            sha256_digest(buffer,w.size() + sizeof(int),addr);
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

        sha256_digest(buffer,w.size() + sizeof(int),tag);
        D.add_tag(tag);
    }
    setD(*io_service,endpoint_iterator,string((char*)addr,DIGEST_SIZE),D.bits);
    server->batch_revoke(w,cnt);
}