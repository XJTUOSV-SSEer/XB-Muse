#ifndef DATAUSER_H
#define DATAUSER_H

#include "../common/data_type.h"
#include "../common/encrypt_config.h"
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

    boost::asio::io_service *io_service;
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator;

    unsigned char *key = (unsigned char*) "0123456789123456";
    unsigned char *iv = (unsigned char*) "0123456789123456";
    
public:
    // vector<int> Search(string w);

    vector<int> Search_batch(string w);

    DataUser(int userId,int eid,boost::asio::io_service *io_service,
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
};