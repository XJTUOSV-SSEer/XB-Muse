#include "Server.h"
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <iostream>

using namespace std;

Server::Server(vector<int> userIds){
    for(int userId:userIds){
        Addrs[userId] = vector<string>();
    }
}

void Server::update(unordered_map<string,string> keyValues,unordered_map<int,vector<string>> CntDiffs){
    DictW.insert(keyValues.begin(),keyValues.end());
    for(const auto& pair:CntDiffs){
        Addrs[pair.first].emplace_back(pair.second);
    }
}