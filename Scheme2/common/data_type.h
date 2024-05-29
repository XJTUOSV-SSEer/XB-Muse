#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include "config.h"
#include "encrypt_config.h"
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

// /* for all sources except OCALL/ECALL */

// const std::string raw_doc_dir= "streaming/"; 

// #define AESGCM_IV_SIZE 12//Question: AES加密算法需要的参数？补全长度？
// static unsigned char gcm_iv[] = {
//     0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84
// };

// #define AESGCM_MAC_SIZE 16//Question: AES加密算法需要的参数？补全长度？

// #define MAX_FILE_LENGTH 10 

// #define ENC_KEY_SIZE 16 // for AES128
// #define ENTRY_VALUE_LEN 128 // 1024-bit

// #define ENTRY_HASH_KEY_LEN_128 16 // for HMAC-SHA128- bit key
// #define BUFLEN 10240 //buffer for enc + dec
// #define RAND_LEN 64// 256 // 2048-bit

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

// struct DelCnt{
//     string w;
//     string enc_w_cnt1;

//     DelCnt(string w,string enc_w_cnt1){
//         this->w = w;
//         this->enc_w_cnt1 = enc_w_cnt1;
//     }

//     bool operator==(const DelCnt& other) const {
//         return w == other.w &&
//             enc_w_cnt1 == other.enc_w_cnt1;
//     }
// };

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
