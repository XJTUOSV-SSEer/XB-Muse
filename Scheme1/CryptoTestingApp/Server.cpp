#include "Server.h"
#include "CryptoEnclave_u.h"
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <iostream>

using namespace std;

Server::Server(vector<int> userIds,int eid){
    this->eid = eid;
    for(int userId:userIds){
        Addrs[userId] = vector<string>();
        Revtag.emplace(userId, vector<Revoketag>());
        FileDelCnts[userId] = vector<string>();
    }
}

void Server::addFile(int ind,int userId,vector<string> cntEnc,vector<KeyValue> keyValues){
    if(AccessList.find(ind) == AccessList.end()){
        AccessList[ind] = unordered_set<int>();
    }
    AccessList[ind].insert(userId);
    for(KeyValue keyValue:keyValues){
        DictW[keyValue.addr] = keyValue.val;
    }

    // if(Addrs.find(userId) == AccessList.end()){
    //     Addrs[userId] = unordered_set<int>();
    // }
    for(string c : cntEnc){
        Addrs[userId].emplace_back(c);
    }
}

void Server::delFile(int userId,vector<Revoketag> Revoketags,vector<string> DelCntDiffs){
    // cout<<"DEL 3"<<endl;
    for(Revoketag revoketag : Revoketags){
        Revtag[userId].emplace_back(revoketag);
    }

    for(string delCnt : DelCntDiffs){
        FileDelCnts[userId].emplace_back(delCnt);
    }
}

unordered_map<string,int> Server::search(vector<string> Tlist,vector<GGMNode> remain_node,string tkn,BloomFilter<32, GGM_SIZE, HASH_SIZE> D,int userId){

    unordered_map<string,int> NewInd;
    unordered_set<string> DelInd;
    int flag_size = flags[userId][tkn].size();
    for(int i = flag_size; i < Tlist.size() ; i++){
        flags[userId][tkn].emplace_back(false);
    }
    vector<bool> &flag = flags[userId][tkn];

    for(int i = 1 ; i <= Tlist.size() ; i++){

        if(true){//!flag[i - 1]){
            Val val = DictW[Tlist[i - 1]];
            int indi;
            char val_tag[DIGEST_SIZE];
            memcpy(val_tag,val.tag.c_str(),DIGEST_SIZE);
            void *val_ct = malloc(val.ct.size() * (AES_BLOCK_SIZE + sizeof(int)));
            for(int i = 0 ; i < val.ct.size(); i++){
                memcpy((uint8_t *)val_ct + i * (AES_BLOCK_SIZE + sizeof(int)),val.ct[i].c_str(),AES_BLOCK_SIZE + sizeof(int));
            }
            ecall_check_doc(eid,&remain_node,&D,(char *)val_tag,(char *)val_ct,&NewInd,&DelInd,&flag,
                sizeof(remain_node),sizeof(D),
                DIGEST_SIZE,AES_BLOCK_SIZE + sizeof(int) ,val.ct.size(),
                sizeof(NewInd),sizeof(DelInd),sizeof(flag),
                i);
            free(val_ct);
        }
    }

    unordered_map<string,int> OldInd = EDBcache[userId][tkn];
    
    for(string tag:DelInd){
        OldInd.erase(tag);
    }
    unordered_map<string,int> Res;
    for (const auto& pair : OldInd) {
        Res[pair.first] = pair.second;
    }
    for (const auto& pair : NewInd) {
        Res[pair.first] = pair.second;
    }
    EDBcache[userId][tkn]={};
    for(const auto& pair : Res){
        EDBcache[userId][tkn][pair.first] = pair.second;
    }
    // cout<<"Res.size:"<<Res.size()<<endl;
    return Res;
}

