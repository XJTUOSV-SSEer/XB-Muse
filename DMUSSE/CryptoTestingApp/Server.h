#ifndef SERVER_H
#define SERVER_H

#include "../common/data_type.h"
#include "../common/config.h"
#include "Utils.h"
#include "../common/GGMNode.h"

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>

using namespace std;

class Server{
public:

    unordered_map<int,unordered_set<int>> AccessList;
    unordered_map<string,string> DictW; //(addr,val),addr是一个string
    unordered_map<int,vector<string>> Addrs;//Queue[u]

public:

    Server(vector<int> userIds);

    void update(unordered_map<string,string> keyValues,unordered_map<int,vector<string>> CntDiffs);

};

#endif