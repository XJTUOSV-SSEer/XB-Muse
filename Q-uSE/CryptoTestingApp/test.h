#ifndef TEST_H
#define TEST_H

#include <string>
#include <vector>
#include <unordered_map>

#include "Server.h"
#include "DataOwner.h"
#include "DataUser.h"

using namespace std;

vector<std::string> split_string(const std::string& input);

int init_data_set(string filePath,unordered_map<string,vector<int>> &dataSet,unordered_map<int,vector<string>> &dataSet_reverted);

vector<int> prase_argv_to_int(int argc,char* argv[]);

void auth_all(DataOwner *dataOwner,unordered_map<string,vector<int>> &dataSet,int dataUserId);

void auth_all(Server *server,unordered_map<string,vector<int>> &dataSet,int dataUserId);

void update_all(DataOwner *dataOwner,unordered_map<int,vector<string>> &dataSet_reverted);

void test0(int argc,char* argv[]);

void test1(int argc,char* argv[]);

void test2(int argc,char* argv[]);

void test3(int argc,char* argv[]);

void test4(int argc,char* argv[]);

#endif