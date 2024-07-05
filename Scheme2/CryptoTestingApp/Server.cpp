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
    }
}

void Server::batch_revoke(string w,int cnt){
    batchCnt[w] = cnt;
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

unordered_map<string,int> Server::search(vector<string> Tlist,vector<GGMNode> remain_node,string tkn,vector<BloomFilter<32, GGM_SIZE, HASH_SIZE>> Ds,int userId){

    unordered_map<string,int> NewInd;
    unordered_set<string> DelInd;
    BloomFilter<32, GGM_SIZE, HASH_SIZE> D;

    for(int i = 1 ; i <= Tlist.size() ; i++){
        // printHexBytes(Tlist[i - 1]);
        bool flag = true;

        Val val = DictW[Tlist[i - 1]];
        for(BloomFilter<32, GGM_SIZE, HASH_SIZE> d:Ds){
            vector<long> is = d.search();
            vector<long> indexs = d.get_index((uint8_t*)(val.tag.c_str()));
            sort(indexs.begin(),indexs.end());

            bool flag1 = false; //标志此tag对应的此布隆过滤器是否有全1的
            for(int index:indexs){
                if(d.bits[index] == 0){
                    flag1 = true;
                }
            }
            if(!flag1){     //若某个布隆过滤器标志全部删除，则检验不通过
                flag = false;
                break;
            }
        }
        // cout<<"flag"<<i<<"  "<<flag<<endl;

        if(flag){
            int indi;
            char val_tag[DIGEST_SIZE];
            memcpy(val_tag,val.tag.c_str(),DIGEST_SIZE);
            void *val_ct = malloc(val.ct.size() * (AES_BLOCK_SIZE + sizeof(int)));
            for(int i = 0 ; i < val.ct.size(); i++){
                memcpy((uint8_t *)val_ct + i * (AES_BLOCK_SIZE + sizeof(int)),val.ct[i].c_str(),AES_BLOCK_SIZE + sizeof(int));
            }
            // cout<<"check point 4"<<endl;
            ecall_check_doc(eid,&remain_node,&D,(char *)val_tag,(char *)val_ct,&NewInd,&DelInd,&flag,
                sizeof(remain_node),sizeof(D),
                DIGEST_SIZE,AES_BLOCK_SIZE + sizeof(int) ,val.ct.size(),
                sizeof(NewInd),sizeof(DelInd),sizeof(flag),
                i);
            // cout<<"check point 5"<<endl;
            free(val_ct);
        }else{
            DelInd.insert(val.tag);
        }
    }
    // cout<<"check point 6"<<endl;
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

