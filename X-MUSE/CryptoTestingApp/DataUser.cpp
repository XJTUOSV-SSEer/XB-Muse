#include "DataUser.h"
#include "CryptoEnclave_u.h"
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

DataUser::DataUser(int userId,int eid,bool is_anti_replace_attack){
    this->userId = userId;
    this->eid = eid;
    this->is_anti_replace_attack = is_anti_replace_attack;
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
    unordered_map<string,BloomFilter<32, GGM_SIZE, HASH_SIZE>> revoketags = server->Revtag[userId];
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
    // cout<<"DataUser::Search : 4"<<endl;
    for (auto &revoketag : revoketags){
        uint8_t decryptdRevokeTagAddr[revoketag.first.size()];
        aes_decrypt((unsigned char*)revoketag.first.c_str(),revoketag.first.size(),key,iv,decryptdRevokeTagAddr);
        if(is_anti_replace_attack){
            // cout<<"DataUser::Search : 7       "<<revoketag.addr.size()<<" "<<sizeof(int)<<endl;
            string w1 = string((char *)decryptdRevokeTagAddr,revoketag.first.size() - sizeof(int));
            // cout<<"DataUser::Search : 10"<<endl;
            int cnt1 = *(int*) (decryptdRevokeTagAddr + revoketag.first.size() - sizeof(int));
            // cout<<"DataUser::Search : 11"<<endl;
            // cout<<"DataUser::Search : 9"<<endl;
            if( w1 == w && cnt1 == FileDelCnts[w1]){
                flag = false;
                vector<long> delete_pos = revoketag.second.search();
                D = revoketag.second;
                ecall_SRE_cKRev(eid,(char *)key,KEY_LEN,&revoketag.second.bits,&remain_node,sizeof(revoketag.second),sizeof(remain_node));
                break;
            }
            // cout<<"DataUser::Search : 8"<<endl;
        }else{
            // cout<<"DataUser::Search : 5"<<endl;
            string w1 = string((char *)decryptdRevokeTagAddr,revoketag.first.size());
            if(w1 == w){
                flag = false;
                vector<long> delete_pos = revoketag.second.search();
                D = revoketag.second;
                ecall_SRE_cKRev(eid,(char *)key,KEY_LEN,&revoketag.second.bits,&remain_node,sizeof(revoketag.second),sizeof(remain_node));
                break;
            }
            // cout<<"DataUser::Search : 6"<<endl;
        }
    }
    if(flag){
        remain_node.emplace_back(GGMNode(0,0,key));
    }
    // cout<<"remain_node.size() :  "<<remain_node.size()<<endl;
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