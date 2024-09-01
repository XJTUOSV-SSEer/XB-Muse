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
        int cnt = pair.first;
        const std::vector<std::string>& value = pair.second;

        // 如果 map2 中已经存在该键,则将 value 合并到 map2 中对应的 vector 中
        if (Addrs.count(cnt) > 0) {
            Addrs[cnt].insert(Addrs[cnt].end(), value.begin(), value.end());
        } else {
            // 否则,将整个 vector 插入到 map2 中
            Addrs[cnt] = value;
        }
    }
}

vector<string> Server::search(vector<string> TList){
    clock_t start = clock();
    vector<string> Fw;
    for(string str : TList){
        Fw.emplace_back(DictW[str]);
    }
    clock_t end = clock();

    double duration = static_cast<double>(end - start) / 1000;
    cout<<duration<<" ";
    return Fw;
}