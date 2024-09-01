#include "DataUser.h"
#include "../common/GGMNode.h"
#include "GGMTree.h"
#include "CryptoEnclave_u.h"
#include <string>
#include "stdio.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <vector>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <cstring> 
#include <openssl/rand.h>

#include <iomanip>

DataUser::DataUser(int userId,int eid,boost::asio::io_service *io_service,
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    this->userId = userId;
    this->eid = eid;
    this->io_service = io_service;
    this->endpoint_iterator = endpoint_iterator;
}

std::bitset<GGM_SIZE> getD(boost::asio::io_service &io_service, boost::asio::ip::tcp::resolver::iterator endpoint_iterator,  std::string w) {

    std::stringstream ss;
    boost::property_tree::ptree pt;

    string finalW;

    bitset<8> bit_set;

    for(char c : w){
        bit_set = std::bitset<8>(c);
        finalW += bit_set.to_string();
    }

    pt.put("w", finalW);

    // 将 property_tree 序列化为 JSON 字符串
    boost::property_tree::write_json(ss, pt);
    std::string json_str = ss.str();

    // 构建 HTTP POST 请求报文
    std::string request =
        "GET /getD HTTP/1.1\r\n"
        "Host: "+ string(BLOCKCHAIN_SERVICE_IP) +":"+ string(BLOCKCHAIN_SERVICE_PORT) +"\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + std::to_string(json_str.length()) + "\r\n"
        "\r\n" +
        json_str;

    std::string response = send_http(io_service,endpoint_iterator,request);

    size_t body_start = response.find("\r\n\r\n");
    if (body_start == std::string::npos) {
        std::cout << "Error: Invalid HTTP response format" << std::endl;
        return std::bitset<GGM_SIZE>();
    }

    // 提取响应体
    std::string response_data = response.substr(body_start + 4);
    std::string pattern = "bits_array";
    size_t pos = response_data.find(pattern);

    size_t pos1 = response_data.find(':', pos);
    size_t pos2 = response_data.find('"', pos1 + 1);
    size_t pos3 = response_data.find('"', pos2 + 1);
    std::string bit_string = response_data.substr(pos2 + 1, pos3 - pos2 - 1);
    
    int size = bit_string.size();
    return std::bitset<GGM_SIZE>(bit_string.substr(0,GGM_SIZE));
}

vector<int> DataUser::Search_batch(string w){

clock_t start1 = clock();
    vector<string> TList;
    vector<string> diffs = server->Addrs[userId];
    uint8_t buffer[w.size() + sizeof(int)];
    for(string diff : diffs){
        uint8_t decryptdDiff[diff.size()];
        aes_decrypt((unsigned char*)diff.c_str(),diff.size(),key,iv,decryptdDiff);
        string w1 = string((char *)decryptdDiff,diff.size() - sizeof(int));
        FileCnts[w1] = *(int *)(decryptdDiff + diff.size() - sizeof(int));
    }
    int cnt = FileCnts[w];
    for(int i = 1 ; i <= cnt ; i++){
        uint8_t digest[DIGEST_SIZE];
        memcpy(buffer,w.c_str(),w.size());
        memcpy(buffer + w.size(),&i,sizeof(int));
        sha256_digest(buffer,w.size()+sizeof(int),digest);
        TList.emplace_back(string((char *)digest,DIGEST_SIZE));
    }

    int delCnt = server->batchCnt[w];
    vector<BloomFilter<32, GGM_SIZE, HASH_SIZE>> Ds;
    for(int i = 1 ; i <= delCnt ; i++){
        uint8_t buffer[w.size() + sizeof(int)];
        memcpy(buffer,w.c_str(),w.size());
        memcpy(buffer + w.size(),(uint8_t*)&i,sizeof(int));
        uint8_t addr[DIGEST_SIZE];
        sha256_digest(buffer,w.size() + sizeof(int),addr);
        BloomFilter<32, GGM_SIZE, HASH_SIZE> d;
        d.bits = getD(*io_service,endpoint_iterator,string((char*)addr,DIGEST_SIZE));
        vector<long> is = d.search();
        Ds.emplace_back(d);
    }

    vector<GGMNode> remain_node;

    remain_node.emplace_back(GGMNode(0,0,key));
    
    uint8_t digest[DIGEST_SIZE];
    sha256_digest((unsigned char *)w.c_str(),w.size(),digest);
    log("DataUser::Search_batch : 1");
clock_t end1 = clock();
    unordered_map<string,int> Res = server->search(TList,remain_node,string((char*)digest,DIGEST_SIZE),Ds,userId);
clock_t start2 = clock();
    log("DataUser::Search_batch : 2");
    vector<int> res;
    for(const auto &pair:Res){
        res.emplace_back(pair.second);
    }
clock_t end2 = clock();

double duration = static_cast<double>(end1 - start1 + end2 - start2) / 1000;
cout<<duration<<endl;
    return res;
}