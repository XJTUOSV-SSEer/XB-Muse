#include "CryptoEnclave_t.h"

#include "EnclaveUtils.h"

#include "sgx_trts.h"
#include "sgx_tcrypto.h"
#include "stdlib.h"
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <vector>
#include <list>
#include "BloomFilter.h"
#include "GGMNode.h"
#include "GGMTree.h"
// #include "../common/EncrytoUtil/Utils.h"
#include "../common/data_type.h"

using namespace std;

void ecall_init(){
    // printf("enclave : %d",1);
    // auto *key = (unsigned char*) "0123456789123456";
    // auto *iv = (unsigned char*) "0123456789123456";
    // auto *plaintext = (unsigned char*) "The test segmentation for AES_CTR mode";

    // unsigned char ciphertext[128];

    // unsigned char nowkey[AES_BLOCK_SIZE];
    // memcpy(nowkey,key,AES_BLOCK_SIZE);

    // printf("enclave : %d",2);
    // int val = 123;
    // unsigned char digest[AES_BLOCK_SIZE];

    // printf("enclave : %d",3);

    // int ciphertext_len = aes_encrypt(plaintext, strlen((char*) plaintext),
    //         key, iv,IV_LEN,
    //         ciphertext);

    // printf("enclave : %d",4);
    // unsigned char recover[128];
    // int plaintext_len = aes_decrypt(ciphertext, ciphertext_len,
    //         key, iv, IV_LEN,recover);
    // printf("enclave : %d",5);
}

void ecall_SRE_cKRev(const char *key,size_t key_len,const void *D,void *result,size_t FloomSize,size_t resultSize){
    bitset<GGM_SIZE> *D_ptr = ( bitset<GGM_SIZE> *) D;
    BloomFilterTool<32, GGM_SIZE, HASH_SIZE> bft;
    bft.init(D_ptr);

    vector<long> bf_pos;
    for (int i = 0; i < GGM_SIZE; ++i) {
        bf_pos.emplace_back(i);
    }
    vector<long> delete_pos = bft.search();
    vector<long> remain_pos;
    set_difference(bf_pos.begin(), bf_pos.end(),
            delete_pos.begin(), delete_pos.end(),
            inserter(remain_pos, remain_pos.begin()));
    // generate GGM Node for the remain position
    vector<GGMNode> node_list;
    node_list.reserve(remain_pos.size());
    for (long pos : remain_pos) {
        node_list.emplace_back(GGMNode(pos, GGMTree::get_level()));
    }

    vector<GGMNode> vec_GGM = GGMTree::min_coverage(node_list);
    // print_bytes(vec_GGM[0].key,AES_BLOCK_SIZE);
    for(auto & i : vec_GGM) {
        memcpy(i.key, key, AES_BLOCK_SIZE);
        GGMTree::derive_key_from_tree(i.key, i.index, i.level, 0);
    }

    size_t cnt = vec_GGM.size();
    uint8_t keys[cnt * AES_BLOCK_SIZE];
    int levels[cnt];
    int indexs[cnt];
    for(size_t i = 0 ; i < cnt ; i++){
        levels[i] = vec_GGM[i].level;
        indexs[i] = vec_GGM[i].index;
        memcpy(keys + i * AES_BLOCK_SIZE,vec_GGM[i].key,AES_BLOCK_SIZE);
    }
    ocall_insert_vector_GGMNode(result,keys,levels,indexs,
    resultSize,cnt * AES_BLOCK_SIZE,cnt);

    return ;
}

void compute_leaf_keys(const vector<GGMNode>& node_list, int level,unordered_map<long,uint8_t *> &keys) {
    for(GGMNode node : node_list) {
        for (int i = 0; i < pow(2, level - node.level); ++i) {
            // printf("enclave : %d",4);
            int offset = ((node.index) << (level - node.level)) + i;
            uint8_t derive_key[AES_BLOCK_SIZE];
            memcpy(derive_key, node.key, AES_BLOCK_SIZE);
            // printf("%x %d %d %d",derive_key,  offset, level - node.level, 0);
            GGMTree::derive_key_from_tree(derive_key,  offset, level - node.level, 0);
            if(keys.find(offset) == keys.end()) {
                keys[offset] = (uint8_t*) malloc(AES_BLOCK_SIZE);
                memcpy(keys[offset], derive_key, AES_BLOCK_SIZE);
            }
        }
    }
}

void ecall_check_doc(const void *remain_node,const void *D,char *val_tag,char *val_ct,
                    void *NewInd,void *DelInd,void *flag,
                    size_t remain_node_size, size_t bloom_filter_size, 
                    size_t val_tag_size,size_t val_ct_size,size_t val_ct_cnt,
                    size_t NewInd_size,size_t DelInd_size,size_t flag_size,int index){
    // printf("enclave : %d",1);
    vector<GGMNode> *remain_node_ptr = (vector<GGMNode> *) remain_node;
    BloomFilter<32, GGM_SIZE, HASH_SIZE> *D_ptr = (BloomFilter<32, GGM_SIZE, HASH_SIZE> *) D;
    // Val *val_ptr = (Val *)val;
    unordered_map<string,int> *NewInd_ptr = (unordered_map<string,int> *) NewInd;
    unordered_set<string> *DelInd_ptr = (unordered_set<string> *) DelInd;
    vector<bool> *flag_ptr = (vector<bool> *) flag;
    unordered_map<long,uint8_t *> keys;

    compute_leaf_keys(*remain_node_ptr, GGMTree::get_level(),keys);
    // printf("%d",keys.size());

    // printf("enclave size : %d",(*remain_node_ptr).size());
    // for(int i = 0 ; i < (*remain_node_ptr).size() ; i++){
    //     printf("aaa");
    //     print_bytes((*remain_node_ptr)[i].key,AES_BLOCK_SIZE);
    // }

    vector<int> res_list;
    
    vector<long> search_pos = BloomFilter<32, GGM_SIZE, HASH_SIZE>::get_index((uint8_t*)val_tag);

    // for(int i = 0 ; i < search_pos.size() ; i++){
    //     print_bytes(keys[search_pos[i]],AES_BLOCK_SIZE);
    // }
    // printf("show : ");
    // print_bytes(keys[5],AES_BLOCK_SIZE);
    // printf("\n");

    sort(search_pos.begin(), search_pos.end());
    // derive the key from search position and decrypt the id
    vector<string> ciphertext_list;
    for(size_t i = 0;i < val_ct_size;i++){
        ciphertext_list.emplace_back(string(val_ct + i * (AES_BLOCK_SIZE + sizeof(int)),AES_BLOCK_SIZE + sizeof(int)));
    }

    for (int i = 0; i < min(search_pos.size(), ciphertext_list.size()); ++i) {
        uint8_t res[4];
        if(keys[search_pos[i]] == nullptr) break;
        aes_decrypt((uint8_t *) (ciphertext_list[i].c_str() + AES_BLOCK_SIZE), ciphertext_list[i].size() - AES_BLOCK_SIZE,
                    keys[search_pos[i]], (uint8_t *) ciphertext_list[i].c_str(),IV_LEN,
                    res);
        // printf("密文：");
        // print_bytes((uint8_t *) (ciphertext_list[i].c_str() + AES_BLOCK_SIZE),ciphertext_list[i].size() - AES_BLOCK_SIZE);
        // printf("密钥：");
        // print_bytes(keys[search_pos[i]],AES_BLOCK_SIZE);
        // printf("明文：");
        // printf("%d",*((int*) res));
        // printf("");
        if(*((int*) res) > 0) {
            res_list.emplace_back(*((int*) res));
        }
        break;
    }

    // printf("res_list.size : %d",res_list.size());
    if(res_list.size() > 0){
        // printf("NewInd insert");
        char val_tag_1[DIGEST_SIZE];
        memcpy(val_tag_1,val_tag,DIGEST_SIZE);
        ocall_insert_map_str_int(NewInd_ptr,val_tag_1,res_list[0],sizeof(*NewInd_ptr),val_tag_size);
    }else{
        ocall_insert_set_string(DelInd_ptr,val_tag,sizeof(*DelInd_ptr),val_tag_size);
    }
    (*flag_ptr)[index - 1] = true;
}

