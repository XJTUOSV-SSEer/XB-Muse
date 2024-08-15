#ifndef TEST_H
#define TEST_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

//搜索数据集对应的目标词
extern vector<string> target_keys;

vector<std::string> split_string(const std::string& input);

int init_data_set(string filePath,unordered_map<string,vector<int>> &dataSet,unordered_map<int,vector<string>> &dataSet_reverted);

vector<int> prase_argv_to_int(int argc,char* argv[]);

void test0(vector<int> args,int eid);

void test1(vector<int> args,int eid);

void test2(vector<int> args,int eid);

void test3(vector<int> args,int eid);

void test4(vector<int> args,int eid);

void test5(vector<int> args,int eid);

void test6(vector<int> args,int eid);

void test7(vector<int> args,int eid);

void test8(vector<int> args,int eid);

#endif