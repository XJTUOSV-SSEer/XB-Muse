/***
 * Demonstrate Client
 * maintain a current Kf
 * read documents in a given directory and give one by one to App.cpp with <fileId, array of words>
 * develop utility to enc and dec file with a given key kf
 * issue a random update operation (op,in) to App
 * issue a random keyword search
 */
#ifndef DATAOWNER_H
#define DATAOWNER_H

#include "../common/data_type.h"
#include "../common/encrypt_config.h"
#include "BloomFilter.h"
#include "GGMTree.h"
// extern "C"{
#include "Utils.h"
// }
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Server.h"

#endif

using namespace std;

class DataOwner{

public:
    unordered_map<int,unordered_set<int>> AccessList;      //文件id->用户id
    unordered_map<int,unordered_map<string,int>> FileCnts;
    unordered_map<int,unordered_map<string,int>> FileDelCnts;
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

    void update(int ind,vector<string> WList,OP op);

    DataOwner(boost::asio::io_service *io_service,
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

};

