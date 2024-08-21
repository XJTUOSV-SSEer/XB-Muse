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
    
}

void DataOwner::update(int ind,vector<string> WList,OP op){
    unordered_set<int> userList = AccessList[ind];
    unordered_map<string,string> keyValues;
    unordered_map<int,vector<string>> CntDiffs;
    
    for(int userId : userList){
        for(string w : WList){
            if(FileCnts.find(userId) == FileCnts.end()){
                FileCnts[userId] = unordered_map<string,int>();
            }
            if(FileCnts[userId].find(w) == FileCnts[userId].end()){
                FileCnts[userId][w] = 0;
            }

            FileCnts[userId][w]++;
            int cnt = FileCnts[userId][w];

            uint8_t buffer[w.size() + 2 * sizeof(int)];

            memcpy(buffer,w.c_str(),w.size());
            memcpy(buffer + w.size(),(uint8_t*)&cnt, sizeof(int));
            uint8_t cipertext[w.size() + sizeof(int)];
            aes_encrypt(buffer,w.size() + sizeof(int),key,iv,cipertext);

            CntDiffs[userId].emplace_back(string((char *)cipertext,w.size()+sizeof(int)));

            memset(buffer,0,w.size() + 2 * sizeof(int));
            memcpy(buffer,w.c_str(),w.size());
            memcpy(buffer + w.size(),(uint8_t*)&cnt,sizeof(int));
            memset(buffer + w.size() + sizeof(int),0,1);
            uint8_t addr[DIGEST_SIZE];

            // uint8_t *tag;
            sha256_digest(buffer,w.size() + 2 * sizeof(int),addr);
            
            uint8_t addr1[SHA256_DIGEST_LENGTH];
            uint8_t id_op[SHA256_DIGEST_LENGTH];

            //计算addr1
            memset(buffer + w.size() + sizeof(int),1,1);

            sha256_digest(buffer,w.size() + 2 * sizeof(int),addr1);

            memset(id_op,0,SHA256_DIGEST_LENGTH);
            memset(id_op,op,1);
            memset(id_op + sizeof(int),ind,1);


            //计算异或值
            uint8_t val[SHA256_DIGEST_LENGTH];
            for(size_t i = 0 ; i < SHA256_DIGEST_LENGTH ; i++){
                val[i] = addr1[i] ^ id_op[i];
            }

            keyValues[string((char*)addr,DIGEST_SIZE)] = string((char*)val,SHA256_DIGEST_LENGTH);
        }
        server->update(keyValues,CntDiffs);
    }
};