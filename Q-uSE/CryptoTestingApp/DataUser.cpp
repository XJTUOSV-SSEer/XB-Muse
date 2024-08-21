#include "DataUser.h"
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

#include <iomanip>

DataUser::DataUser(int userId){
    this->userId = userId;
}

vector<int> DataUser::Search(string w){
    unordered_set<int> Rw;
    unordered_set<int> toRevokeSet;
    vector<string> TList;
    vector<string> diffs = server->Addrs[userId];
    uint8_t buffer[w.size() + 2 * sizeof(int)];
    // cout<<"diffs size : "<<diffs.size()<<endl;
    for(string diff:diffs){
        uint8_t decryptdDiff[diff.size()];
        aes_decrypt((unsigned char*)diff.c_str(),diff.size(),key,iv,decryptdDiff);
        string w1 = string((char *)decryptdDiff,diff.size() - sizeof(int));
        FileCnts[w1] = *(int *)(decryptdDiff + diff.size() - sizeof(int));
    }
    int cnt = FileCnts[w];
    for(int i = 1 ; i <= cnt ; i++){
        uint8_t digest[DIGEST_SIZE];
        memset(buffer,0,w.size() + 2 * sizeof(int));
        memcpy(buffer,w.c_str(),w.size());
        memcpy(buffer + w.size(),&i,sizeof(int));
        memset(buffer + w.size() + sizeof(int),0,1);
        sha256_digest(buffer,w.size() + 2 * sizeof(int),digest);
        TList.emplace_back(string((char *)digest,DIGEST_SIZE));
  
    }
    vector<string> Fw = server->search(TList);
    for(int i = 1 ; i <= Fw.size() ; i++){
        
        memset(buffer,0,w.size() + 2 * sizeof(int));
        memcpy(buffer,w.c_str(),w.size());
        memcpy(buffer + w.size(),(uint8_t*)&i,sizeof(int));
        memset(buffer + w.size() + sizeof(int),1,1);
        uint8_t addr[DIGEST_SIZE];

        // uint8_t *tag;
        sha256_digest(buffer,w.size() + 2 * sizeof(int),addr);
        uint8_t id_op[SHA256_DIGEST_LENGTH];

        for(size_t j = 0 ; j < SHA256_DIGEST_LENGTH ; j++){
            id_op[j] = addr[j] ^ ((uint8_t *)Fw[i - 1].c_str())[j];
        }

        if(*((OP*)id_op) == ADD){
            Rw.insert(*((int *)(id_op + sizeof(int))));
        }else{
            toRevokeSet.insert(*((int *)(id_op + sizeof(int))));
        }
    }
    vector<int> res;
    for(int v:Rw){
        if(toRevokeSet.find(v) == toRevokeSet.end()){
            res.emplace_back(v);
        }
    }

    return res;
}