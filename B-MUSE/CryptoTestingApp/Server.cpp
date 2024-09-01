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

    for(string c : cntEnc){
        Addrs[userId].emplace_back(c);
    }
}

unordered_map<string,int> Server::search(vector<string> Tlist,vector<GGMNode> remain_node,string tkn,vector<BloomFilter<32, GGM_SIZE, HASH_SIZE>> Ds,int userId){
clock_t start = clock();
    unordered_map<string,int> NewInd;
    unordered_set<string> DelInd;
    BloomFilter<32, GGM_SIZE, HASH_SIZE> D;
    log("Server::search : 1");

    for(int i = 1 ; i <= Tlist.size() ; i++){

        bool flag = true;

        Val val = DictW[Tlist[i - 1]];
        // printf("\n");
        // printf("d.size : %u\n",Ds.size());
        for(BloomFilter<32, GGM_SIZE, HASH_SIZE> d:Ds){
            vector<long> indexs = d.get_index((uint8_t*)(val.tag.c_str()));
            sort(indexs.begin(),indexs.end());
            // printf("tag indexs : ");
            // for(long v : indexs){
            //     printf("%ld ",v);
            // }
            // printf("\n");

            bool flag1 = false; //标志此tag对应的此布隆过滤器是否有全1的
            // printf("bloom indexs : ");
            // for(int i = 0 ; i < GGM_SIZE ; i++){
            //     if(d.bits[i] == 1){
            //         printf("%d ",i);
            //     }
            // }
            // printf("\n");
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
        // printf("flag ：%s\n",flag ? "true" : "false");
        if(flag){
            int indi;
            char val_tag[DIGEST_SIZE];
            memcpy(val_tag,val.tag.c_str(),DIGEST_SIZE);
            void *val_ct = malloc(val.ct.size() * (AES_BLOCK_SIZE + sizeof(int)));
            for(int i = 0 ; i < val.ct.size(); i++){
                memcpy((uint8_t *)val_ct + i * (AES_BLOCK_SIZE + sizeof(int)),val.ct[i].c_str(),AES_BLOCK_SIZE + sizeof(int));
            }
            log("Server::search : 2 # " + to_string(i));
            ecall_check_doc(eid,&remain_node,&D,(char *)val_tag,(char *)val_ct,&NewInd,&DelInd,&flag,
                sizeof(remain_node),sizeof(D),
                DIGEST_SIZE,AES_BLOCK_SIZE + sizeof(int) ,val.ct.size(),
                sizeof(NewInd),sizeof(DelInd),sizeof(flag),
                i);
            log("Server::search : 3");
            free(val_ct);
        }else{
            char val_tag[DIGEST_SIZE];
            memcpy(val_tag,val.tag.c_str(),DIGEST_SIZE);
            ecall_insert_set_string(eid,&DelInd,(char *)val_tag,sizeof(DelInd),DIGEST_SIZE);
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
clock_t end = clock();
double duration = static_cast<double>(end - start) / 1000;
cout<<duration<<" ";
    return Res;
}

