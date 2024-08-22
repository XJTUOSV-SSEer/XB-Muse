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
#include "GGMTree.h"
#include "../common/BF/BloomFilter.h"
#include "../common/GGMNode.h"
#include "../common/data_type.h"

using namespace std;

void ecall_init(){
    printf("ecall init success");
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
            
    vector<GGMNode> node_list;
    node_list.reserve(remain_pos.size());
    for (long pos : remain_pos) {
        node_list.emplace_back(GGMNode(pos, GGMTree::get_level()));
    }

    vector<GGMNode> vec_GGM = GGMTree::min_coverage(node_list);

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

void compute_leaf_keys(const vector<GGMNode>& node_list, int level,unordered_map<long,uint8_t *> &keys,vector<long> search_pos) {

    unordered_map<int,unordered_map<long,GGMNode>> node_dict;

    for(GGMNode node : node_list) {
        node_dict[node.level][node.index] = node;
    }

    for(long pos : search_pos){
        long index = pos;
        for(int searchLevel = level ; searchLevel >=0 ; searchLevel--){
            if(!(node_dict.find(searchLevel) == node_dict.end()) && !(node_dict[searchLevel].find(index) == node_dict[searchLevel].end())){
                uint8_t derive_key[AES_BLOCK_SIZE];
                GGMTree::derive_key_from_tree(derive_key,  pos, level - node_dict[searchLevel][index].level, 0);
                if(keys.find(pos) == keys.end()){
                    keys[pos] = (uint8_t*) malloc(AES_BLOCK_SIZE);
                    memcpy(keys[pos], derive_key, AES_BLOCK_SIZE);
                }
                break;
            }
            index = index >> 1;
        }
    }
}

void ecall_insert_set_string(void *DelInd_ptr,char *val_tag,size_t DelInd_ptr_size,size_t val_tag_size){
    unordered_set<string> *ptr = (unordered_set<string> *) DelInd_ptr;
    ocall_insert_set_string(ptr,val_tag,sizeof(*ptr),val_tag_size);
}

void ecall_check_doc(const void *remain_node,const void *D,char *val_tag,char *val_ct,
                    void *NewInd,void *DelInd,void *flag,
                    size_t remain_node_size, size_t bloom_filter_size, 
                    size_t val_tag_size,size_t val_ct_size,size_t val_ct_cnt,
                    size_t NewInd_size,size_t DelInd_size,size_t flag_size,int index){
    vector<GGMNode> *remain_node_ptr = (vector<GGMNode> *) remain_node;
    BloomFilter<32, GGM_SIZE, HASH_SIZE> *D_ptr = (BloomFilter<32, GGM_SIZE, HASH_SIZE> *) D;

    unordered_map<string,int> *NewInd_ptr = (unordered_map<string,int> *) NewInd;
    unordered_set<string> *DelInd_ptr = (unordered_set<string> *) DelInd;
    vector<bool> *flag_ptr = (vector<bool> *) flag;
    unordered_map<long,uint8_t *> keys;

    vector<int> res_list;
    
    vector<long> search_pos = BloomFilter<32, GGM_SIZE, HASH_SIZE>::get_index((uint8_t*)val_tag);
    sort(search_pos.begin(), search_pos.end());

    compute_leaf_keys(*remain_node_ptr, GGMTree::get_level(),keys,search_pos);

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
        if(*((int*) res) > 0) {
            res_list.emplace_back(*((int*) res));
        }
        break;
    }

    for(auto &pair : keys){
        free(pair.second);
    }

    if(res_list.size() > 0){
        char val_tag_1[DIGEST_SIZE];
        memcpy(val_tag_1,val_tag,DIGEST_SIZE);
        ocall_insert_map_str_int(NewInd_ptr,val_tag_1,res_list[0],sizeof(*NewInd_ptr),val_tag_size);
    }else{
        ocall_insert_set_string(DelInd_ptr,val_tag,sizeof(*DelInd_ptr),val_tag_size);
    }
}

