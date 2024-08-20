#ifndef DATAOWNER_H
#define DATAOWNER_H

#include "../common/data_type.h"
#include "../common/config.h"

#include "Utils.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Server.h"

using namespace std;

class DataOwner{

public:
    unordered_map<int,unordered_set<int>> AccessList;      //文件id->用户id
    unordered_map<int,unordered_map<string,int>> FileCnts;

    Server *server;

    unsigned char *key = (unsigned char*) "0123456789123456";
    unsigned char *iv = (unsigned char*) "0123456789123456";

public:

    void update(int ind,vector<string> WList,OP op);

    DataOwner();

};

#endif