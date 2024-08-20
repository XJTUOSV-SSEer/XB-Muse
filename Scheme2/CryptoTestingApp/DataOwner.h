#ifndef DATAOWNER_H
#define DATAOWNER_H

#include "../common/data_type.h"
#include "../common/config.h"
#include "../common/BF/BloomFilter.h"
#include "GGMTree.h"
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
    // unordered_map<int,unordered_map<string,int>> FileDelCnts;
    unordered_map<string,int> batchCnts;
    unordered_map<int,unordered_map<string,string>> MSK;
    unordered_map<int,unordered_map<string,BloomFilter<32, GGM_SIZE, HASH_SIZE>>> D;
    
    // vector<string> cntEnc;
    GGMTree *tree;

    unordered_map<int,UsersKey> UserKeys;

    Server *server;

    boost::asio::io_service *io_service;
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator;

    unsigned char *key = (unsigned char*) "0123456789123456";
    unsigned char *iv = (unsigned char*) "0123456789123456";

public:

    void insert(int ind,vector<string> WList);
    void revoke(string w,vector<int> IDList);

    DataOwner(boost::asio::io_service *io_service,
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

};

#endif