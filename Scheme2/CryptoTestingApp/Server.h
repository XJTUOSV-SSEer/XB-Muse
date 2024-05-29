#ifndef SERVER_H
#define SERVER_H

#include "../common/data_type.h"
#include "../common/encrypt_config.h"
#include "Utils.h"
#include "GGMNode.h"

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>

using namespace std;

class Server{
public:

    unordered_map<int,unordered_set<int>> AccessList;
    unordered_map<string,Val> DictW; //(addr,val),addr是一个string
    unordered_map<int,vector<string>> Addrs;//Queue[u]

    unordered_map<int,vector<Revoketag>> Revtag;
    unordered_map<int,vector<string>> FileDelCnts;
    
    unordered_map<int,unordered_map<string,unordered_map<string,int>>> EDBcache;
    unordered_map<int,unordered_map<string,vector<bool>>> flags;

    unordered_map<string,int> batchCnt;

    int eid;

public:

    Server(vector<int> userIds,int eid);

    void addFile(int ind,int userId,vector<string> cntEnc,vector<KeyValue> keyValues);
    void delFile(int userId,vector<Revoketag> Revoketags,vector<string> DelCntDiffs);

    void batch_revoke(string w,int cnt);

    unordered_map<string,int> search(vector<string> Tlist,vector<GGMNode> remain_node,string tkn,vector<bool> flags,int userId);

};

#endif