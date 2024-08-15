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

DataOwner::DataOwner(){
    tree = new GGMTree(GGM_SIZE);
}

void DataOwner::update(int ind,vector<string> WList,OP op){
    unordered_set<int> userList = AccessList[ind];
    vector<string> keyValues;
    int Cnt = 0;
    for(int userId : userList){
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

            sha256_digest(buffer,w.size() + sizeof(int),tag);

            if(op == ADD){
                FileCnts[userId][w]++;
                int cnt = FileCnts[userId][w];
                memcpy(buffer,w.c_str(),w.size());
                memcpy(buffer + w.size(),(uint8_t*)&cnt, sizeof(int));
                uint8_t cipertext[w.size() + sizeof(int)];
                aes_encrypt(buffer,w.size() + sizeof(int),key,iv,cipertext);
                cntEnc.emplace_back(string((char *)cipertext,w.size()+sizeof(int)));
                
                //SRE加密
                // get all offsets in BF
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
                // uint8_t *tag;

                sha256_digest(buffer,w.size() + sizeof(int),addr);
                keyValues.emplace_back(KeyValue(string((char*)addr,DIGEST_SIZE),Val(ciphertext_list,string((char*)tag,DIGEST_SIZE))));
                
                
            }else{
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

                D[userId][w].add_tag(tag);
                vector<long> v = D[userId][w].get_index(tag);
                Revoketags.emplace_back(Revoketag(string((char*)addr,w.size() + sizeof(int)),D[userId][w]));
            }

        }

        if(op == ADD){
            server->addFile(ind,userId,cntEnc,keyValues);
        }else{
            server->delFile(userId,Revoketags,DelCntDiffs);
        }
    }
};