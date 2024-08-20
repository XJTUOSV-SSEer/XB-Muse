#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include "config.h"
#include "./BF/BloomFilter.h"
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <array>
#include <list>
#include <string>
#include <cstring>
#include <tuple>
#include <utility>
#include <unordered_map>

using namespace std;

enum OP {
    ADD, DEL
};

struct UsersKey{
    unsigned char K_u[KEY_LEN];
    unsigned char K_u_1[KEY_LEN];
    unsigned char K_t[KEY_LEN];

    bool operator==(const UsersKey& other) const {
        return memcmp(K_u, other.K_u, KEY_LEN) == 0 &&
               memcmp(K_u_1, other.K_u_1, KEY_LEN) == 0 &&
               memcmp(K_t, other.K_t, KEY_LEN) == 0;
    }
};

struct Val{
    vector<string> ct;
    string tag;

    Val(vector<string> ct,string tag){
        this->ct = ct;
        this->tag = tag;
    }

    Val(){

    }

    bool operator==(const Val& other) const {
        return ct == other.ct &&
            tag == other.tag;
    }
};

struct KeyValue{
    string addr;
    Val val;

    KeyValue(string addr,Val val){
        this->addr = addr;
        this->val = val;
    }

    bool operator==(const KeyValue& other) const {
        return addr == other.addr &&
            val == other.val;
    }
};

struct Revoketag{
    string addr;
    BloomFilter<32, GGM_SIZE, HASH_SIZE> D;

    Revoketag(string addr,BloomFilter<32, GGM_SIZE, HASH_SIZE> D){
        this->addr = addr;
        this->D = D;
    }

    Revoketag(){

    }

    bool operator==(const Revoketag& other) const {
        return addr == other.addr &&
            D == other.D;
    }
};
// //消息
// typedef struct
// {
//     size_t content_length;
//     unsigned char content[RAND_LEN];
// } rand_t; //used to export between ecall and ocall


// //文件ID
// typedef struct docIds {
//     char *doc_id; 
//     size_t id_length;  // length of the doc_id
// } docId; 

// // 密钥？应该是 键值对中的键
// typedef struct entryKeys {
//     char *content; 
//     size_t content_length;  // length of the entry_value
// } entryKey;

// //明文
// typedef struct entryValues {
//     char *message; 
//     size_t message_length;  // length of the entry_value
// } entryValue;

// typedef struct docContents{
//     docId id;
//     char* content;
//     int content_length;
//     //std::vector<std::string> wordList;
// } docContent;

// typedef std::pair<entryKey, entryValue> entry;//entry登记 ,作为密文

#endif
