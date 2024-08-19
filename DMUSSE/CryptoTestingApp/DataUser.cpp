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

DataUser::DataUser(int userId,int eid){
    this->userId = userId;
    this->eid = eid;
}

vector<int> DataUser::Search(string w){
    // cout<<"DataUser::Search : 1"<<endl;
    vector<string> TList;
    vector<string> diffs = server->Addrs[userId];
    uint8_t buffer[w.size() + sizeof(int)];
    for(string diff : diffs){
        uint8_t decryptdDiff[diff.size()];
        aes_decrypt((unsigned char*)diff.c_str(),diff.size(),key,iv,decryptdDiff);
        string w1 = string((char *)decryptdDiff,diff.size() - sizeof(int));
        FileCnts[w1] = *(int *)(decryptdDiff + diff.size() - sizeof(int));
    }
    int cnt = FileCnts[w];
    for(int i = 1 ; i <= cnt ; i++){
        uint8_t digest[DIGEST_SIZE];
        memcpy(buffer,w.c_str(),w.size());
        memcpy(buffer + w.size(),&i,sizeof(int));
        sha256_digest(buffer,w.size()+sizeof(int),digest);
        TList.emplace_back(string((char *)digest,DIGEST_SIZE));
    }
    vector<Revoketag> revoketags = server->Revtag[userId];
    vector<string> DelCnts = server->FileDelCnts[userId];

    for(string delcnt : DelCnts){
        uint8_t decryptdDelCnt[delcnt.size()];
        aes_decrypt((unsigned char*)delcnt.c_str(),delcnt.size(),key,iv,decryptdDelCnt);
        string w1 = string((char *)decryptdDelCnt,delcnt.size() - sizeof(int));
        int cnt1 = *(int*) (decryptdDelCnt + delcnt.size() - sizeof(int));
        FileDelCnts[w1] = cnt1;
    }
    vector<GGMNode> remain_node;
    bool flag = true;
    BloomFilter<32, GGM_SIZE, HASH_SIZE> D;
    for (Revoketag revoketag : revoketags){
        uint8_t decryptdRevokeTagAddr[revoketag.addr.size()];
        aes_decrypt((unsigned char*)revoketag.addr.c_str(),revoketag.addr.size(),key,iv,decryptdRevokeTagAddr);
        string w1 = string((char *)decryptdRevokeTagAddr,revoketag.addr.size() - sizeof(int));
        int cnt1 = *(int*) (decryptdRevokeTagAddr + revoketag.addr.size() - sizeof(int));
        FileDelCnts[w1] = cnt1;
        if( w1 == w && cnt1 == FileDelCnts[w1]){
            flag = false;
            vector<long> delete_pos = revoketag.D.search();
            D = revoketag.D;
            // ecall_SRE_cKRev(eid,(char *)key,KEY_LEN,&revoketag.D.bits,&remain_node,sizeof(revoketag.D),sizeof(remain_node));
            break;
        }
    }
    if(flag){
        remain_node.emplace_back(GGMNode(0,0,key));
    }
    uint8_t digest[DIGEST_SIZE];
    sha256_digest((unsigned char *)w.c_str(),w.size(),digest);
    // cout<<"DataUser::Search : 2"<<endl;
    unordered_map<string,int> Res = server->search(TList,remain_node,string((char*)digest,DIGEST_SIZE),D,userId);
    // cout<<"DataUser::Search : 3"<<endl;
    vector<int> res;
    for(const auto &pair:Res){
        res.emplace_back(pair.second);
    }
    return res;
}