#ifndef DATAUSER_H
#define DATAUSER_H

#include "../common/data_type.h"
#include "../common/config.h"
#include "Utils.h"
#include "Server.h"
#include <vector>

#endif

using namespace std;

class DataUser{
public:
    int userId;
    Server *server;

    unordered_map<string,int> FileCnts;
    unordered_map<string,int> FileDelCnts;

    int eid;

    unsigned char *key = (unsigned char*) "0123456789123456";
    unsigned char *iv = (unsigned char*) "0123456789123456";
    
public:
    vector<int> Search(string w);

    DataUser(int userId,int eid);
};