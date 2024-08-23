
#include <string>
#include "stdio.h"
#include "stdlib.h"

#include "sgx_urts.h"
#include "CryptoEnclave_u.h"

#include "../common/data_type.h"
#include "../common/config.h"

#include "Server.h"
#include "DataOwner.h"
#include "DataUser.h"
#include "Utils.h"

#include "test.h"

#include <cstdint>
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

uint64_t timeSinceEpochMillisec() {//截取以纪元时间为单位获取当前时间戳，以毫秒为单位

  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
//end for measurement


#define ENCLAVE_FILE "CryptoEnclave.signed.so"

static int eid;

void ocall_print_string(const char *str) {
    printf("%s\n", str);
}

void ocall_print_string_bytes(const char *str,size_t len) {
	printHexBytes(string(str,len));
}

void ocall_insert_set_string(void *set,void *str,size_t set_len,size_t string_len){
	unordered_set<string> *set_ptr = (unordered_set<string> *) set;
	(*set_ptr).insert(string((char *)str,string_len));
}

void ocall_insert_map_str_int(void *map,void *tag,int val,size_t map_len,size_t tag_len){
	unordered_map<string,int> *map_ptr = (unordered_map<string,int> *) map;
	(*map_ptr)[string((char *)tag,tag_len)]=val;

}

void ocall_insert_vector_GGMNode(void *vec, const unsigned char* keys, int *levels,int *indexs,
								size_t vec_len,size_t keys_len,size_t cnt){
	vector<GGMNode> *vec_ptr = (vector<GGMNode> *)vec;
	for(int i = 0 ; i < cnt ; i++){
		GGMNode node(indexs[i],levels[i]);
		memcpy(node.key,keys + i * AES_BLOCK_SIZE,AES_BLOCK_SIZE);
		(*vec_ptr).emplace_back(node);
	}
}

//main func
int main(int argc,char* argv[])
{
	/* Setup enclave */
	sgx_enclave_id_t eid; //sgx id
	sgx_status_t ret; //sgx状态类型
	sgx_launch_token_t token = { 0 };
	int token_updated = 0;

	/********************创建enclave环境****************************/
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &token_updated, &eid, NULL); //eid
	if (ret != SGX_SUCCESS)
	{
		printf("sgx_create_enclave failed: %#x\n", ret);
		return 1;
	}

    /**********************************************************************/

	int status = atoi(argv[1]);

	if(status == 0){
		test0(argc,argv,eid);
	}else if(status == 1){
		test1(argc,argv,eid);
	}else if(status == 2){
		test2(argc,argv,eid);
	}else if(status == 3){
		test3(argc,argv,eid);
	}else if(status == 4){
		test4(argc,argv,eid);
	}else if(status == 5){
		test5(argc,argv,eid);
	}else if(status == 6){
		test6(argc,argv,eid);
	}else if(status == 7){
		test7(argc,argv,eid);
	}else if(status == 8){
		test8(argc,argv,eid);
	}else{
		test9(argc,argv,eid);
	}

	return 0;
}

