#ifndef TEST_H
#define TEST_H

#include <string>
#include <vector>
#include <unordered_map>

#include "Server.h"
#include "DataOwner.h"
#include "DataUser.h"

#include "Utils.h"

using namespace std;

//搜索数据集对应的目标词
extern vector<string> target_keys;

vector<std::string> split_string(const std::string& input);

int init_data_set(string filePath,unordered_map<string,vector<int>> &dataSet,unordered_map<int,vector<string>> &dataSet_reverted);

vector<int> prase_argv_to_int(int argc,char* argv[]);

void auth_all(DataOwner *dataOwner,unordered_map<string,vector<int>> &dataSet,int dataUserId);

void auth_all(Server *server,unordered_map<string,vector<int>> &dataSet,int dataUserId);

void update_all(DataOwner *dataOwner,unordered_map<int,vector<string>> &dataSet_reverted,int dataUserId);

void test0(vector<int> args,int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

void test1(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

void test2(vector<int> args,int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

void test3(vector<int> args,int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

void test4(vector<int> args,int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

void test5(vector<int> args,int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

void test6(vector<int> args,int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

void test7(vector<int> args,int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

void test8(vector<int> args,int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

#endif