#include "DataUser.h"
#include "GGMNode.h"
#include "GGMTree.h"
#include "CryptoEnclave_u.h"
#include <string>
//#include <string.h> // memset(KF, 0, sizeof(KF));
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

    // cout << "Get request:" << endl;
    // cout << request << endl << endl;

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

    return std::bitset<GGM_SIZE>(bit_string);
}
// vector<int> DataUser::Search(string w){
//     vector<string> TList;
//     vector<string> diffs = server->Addrs[userId];
//     uint8_t buffer[w.size() + sizeof(int)];
//     for(string diff : diffs){
//         uint8_t decryptdDiff[diff.size()];
//         aes_decrypt((unsigned char*)diff.c_str(),diff.size(),key,iv,decryptdDiff);
//         string w1 = string((char *)decryptdDiff,diff.size() - sizeof(int));
//         // cout<<"num:"<<*(int *)(decryptdDiff + diff.size() - sizeof(int))<<endl;
//         FileCnts[w1] = *(int *)(decryptdDiff + diff.size() - sizeof(int));
//         // cout<< w1 <<" "<<*(int*)(decryptdDiff + diff.size() - sizeof(int))<<endl;
//     }
//     int cnt = FileCnts[w];
//     // cout<<"cnt:"<<cnt<<endl;
//     for(int i = 1 ; i <= cnt ; i++){
//         uint8_t digest[DIGEST_SIZE];
//         memcpy(buffer,w.c_str(),w.size());
//         memcpy(buffer + w.size(),&i,sizeof(int));
//         sha256_digest(buffer,w.size()+sizeof(int),digest);
//         TList.emplace_back(string((char *)digest,DIGEST_SIZE));
//     }
//     vector<Revoketag> revoketags = server->Revtag[userId];
//     vector<string> DelCnts = server->FileDelCnts[userId];
//     // cout<<"DelCnts size : "<<DelCnts.size()<<"     "<<"revoketags size:"<<revoketags.size()<<endl;
//     for(string delcnt : DelCnts){
//         uint8_t decryptdDelCnt[delcnt.size()];
//         aes_decrypt((unsigned char*)delcnt.c_str(),delcnt.size(),key,iv,decryptdDelCnt);
//         string w1 = string((char *)decryptdDelCnt,delcnt.size() - sizeof(int));
//         int cnt1 = *(int*) (decryptdDelCnt + delcnt.size() - sizeof(int));
//         FileDelCnts[w1] = cnt1;
//         // cout<<"DelCnts:         "<<"w1 : "<<w1 <<"     " <<"cnt1 : "<<cnt1<<endl;
//     }
//     vector<GGMNode> remain_node;
//     bool flag = true;
//     BloomFilter<32, GGM_SIZE, HASH_SIZE> D;
//     for (Revoketag revoketag : revoketags){
//         uint8_t decryptdRevokeTagAddr[revoketag.addr.size()];
//         aes_decrypt((unsigned char*)revoketag.addr.c_str(),revoketag.addr.size(),key,iv,decryptdRevokeTagAddr);
//         string w1 = string((char *)decryptdRevokeTagAddr,revoketag.addr.size() - sizeof(int));
//         int cnt1 = *(int*) (decryptdRevokeTagAddr + revoketag.addr.size() - sizeof(int));
//         FileDelCnts[w1] = cnt1;
//         if( w1 == w && cnt1 == FileDelCnts[w1]){
//             flag = false;
//             vector<long> delete_pos = revoketag.D.search();
//             ecall_SRE_cKRev(eid,(char *)key,KEY_LEN,&revoketag.D.bits,&remain_node,sizeof(revoketag.D),sizeof(remain_node));
//             break;
//         }
//     }
//     if(flag){
//         remain_node.emplace_back(GGMNode(0,0,key));
//     }
//     uint8_t digest[DIGEST_SIZE];
//     sha256_digest((unsigned char *)w.c_str(),w.size(),digest);
//     unordered_map<string,int> Res = server->search(TList,remain_node,string((char*)digest,DIGEST_SIZE),D,userId);
//     vector<int> res;
//     for(const auto &pair:Res){
//         res.emplace_back(pair.second);
//     }
//     return res;
// }

vector<int> DataUser::Search_batch(string w){
    vector<string> TList;
    vector<string> diffs = server->Addrs[userId];
    uint8_t buffer[w.size() + sizeof(int)];
    for(string diff : diffs){
        uint8_t decryptdDiff[diff.size()];
        aes_decrypt((unsigned char*)diff.c_str(),diff.size(),key,iv,decryptdDiff);
        string w1 = string((char *)decryptdDiff,diff.size() - sizeof(int));
        // cout<<"num:"<<*(int *)(decryptdDiff + diff.size() - sizeof(int))<<endl;
        FileCnts[w1] = *(int *)(decryptdDiff + diff.size() - sizeof(int));
        // cout<< w1 <<" "<<*(int*)(decryptdDiff + diff.size() - sizeof(int))<<endl;
    }
    int cnt = FileCnts[w];
    // cout<<"cnt:"<<cnt<<endl;
    for(int i = 1 ; i <= cnt ; i++){
        uint8_t digest[DIGEST_SIZE];
        memcpy(buffer,w.c_str(),w.size());
        memcpy(buffer + w.size(),&i,sizeof(int));
        sha256_digest(buffer,w.size()+sizeof(int),digest);
        TList.emplace_back(string((char *)digest,DIGEST_SIZE));
    }
    // vector<Revoketag> revoketags = server->Revtag[userId];

    int delCnt = server->batchCnt[w];
    vector<BloomFilter<32, GGM_SIZE, HASH_SIZE>> Ds;
    for(int i = 1 ; i <= delCnt ; i++){
        // cout << "check point"<<endl;
        uint8_t buffer[w.size() + sizeof(int)];
        memcpy(buffer,w.c_str(),w.size());
        memcpy(buffer + w.size(),(uint8_t*)&i,sizeof(int));
        uint8_t addr[DIGEST_SIZE];
        sha256_digest(buffer,w.size() + sizeof(int),addr);
        BloomFilter<32, GGM_SIZE, HASH_SIZE> d;
        d.bits = getD(*io_service,endpoint_iterator,string((char*)addr,DIGEST_SIZE));
        vector<long> is = d.search();
        // cout<<"已经撤销的索引："<<endl;
        // for(long index : is){
        //     cout<<index<<endl;
        // }
        // cout<<endl;
        Ds.emplace_back(d);
    }

    vector<bool> flags(TList.size());
    for(int i = 0 ; i < TList.size() ; i++){
        flags[i] = false;
        string tag = TList[i];
        // cout<<"TList"<<i<<endl;
        // vector<long> is = Ds[0].get_index((uint8_t*)tag.c_str());
        // for(long index : is){
        //     cout<<index<<endl;
        // }
        for(BloomFilter<32, GGM_SIZE, HASH_SIZE> d:Ds){
            // vector<long> is = d.search();
            // cout<<"已经撤销的索引："<<endl;
            // for(long index : is){
            //     cout<<index<<endl;
            // }
            // cout<<endl;
            vector<long> indexs = d.get_index((uint8_t*)tag.c_str());
            bool flag = false; //标志此tag对应的此布隆过滤器是否有全1的
            for(int index:indexs){
                if(d.bits[index] == 0){
                    flag = true;
                }
            }
            if(!flag){     //若某个布隆过滤器标志全部删除，则检验不通过
                flags[i] = true;
                break;
            }
        }
    }
    // for(bool f : flags){
    //     cout<<f<<endl;
    // }
    // cout<<endl;
    vector<GGMNode> remain_node;
    if(Ds.size() == 0){
        remain_node.emplace_back(GGMNode(0,0,key));
    }else{
        vector<GGMNode> node_list;
        for(int i = 0 ; i < GGM_SIZE ; i++){
            if(Ds[0].bits[i] == 0){
                node_list.emplace_back(GGMNode(i,GGM_LEVEL));
            }
        }
        remain_node = GGMTree::min_coverage(node_list);
    }
    uint8_t digest[DIGEST_SIZE];
    sha256_digest((unsigned char *)w.c_str(),w.size(),digest);
    // cout<<"check point 2"<<endl;
    unordered_map<string,int> Res = server->search(TList,remain_node,string((char*)digest,DIGEST_SIZE),Ds,userId);
    vector<int> res;
    for(const auto &pair:Res){
        res.emplace_back(pair.second);
    }
    return res;
}