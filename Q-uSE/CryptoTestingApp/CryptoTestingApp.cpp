
#include <string>
#include "stdio.h"
#include "stdlib.h"

#include "sgx_urts.h"

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

//main func
int main(int argc,char* argv[])
{
	// /* Setup enclave */
	// sgx_enclave_id_t eid; //sgx id
	// sgx_status_t ret; //sgx状态类型
	// sgx_launch_token_t token = { 0 };
	// int token_updated = 0;

	// /********************创建enclave环境****************************/
	// ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &token_updated, &eid, NULL); //eid
	// if (ret != SGX_SUCCESS)
	// {
	// 	printf("sgx_create_enclave failed: %#x\n", ret);
	// 	return 1;
	// }

    /**********************************************************************/

	int status = atoi(argv[1]);

	vector<int> args;

	if(status != 1) args = prase_argv_to_int(argc,argv);

	if(status == 0){
		test0(args);
	}else if(status == 1){
		test1(argc,argv);
	}else if(status == 1){
		test2(args);
	}else if(status == 1){
		test3(args);
	}else{
		test4(args);
	}
	return 0;
}

