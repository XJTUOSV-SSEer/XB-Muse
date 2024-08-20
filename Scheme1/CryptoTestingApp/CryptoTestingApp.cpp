
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

	vector<int> args = prase_argv_to_int(argc,argv);

	int status = args[0];

	if(status == 0){
		test0(args,eid);
	}else if(status == 1){
		test1(args,eid);
	}else if(status == 2){
		test2(args,eid);
	}else if(status == 3){
		test3(args,eid);
	}else if(status == 4){
		test4(args,eid);
	}else if(status == 5){
		test5(args,eid);
	}else if(status == 6){
		test6(args,eid);
	}else if(status == 7){
		test7(args,eid);
	}else{
		test8(args,eid);
	}

	return 0;

	if(status == 1){
		// 初始化server、dataowner和datauser
		vector<int> userIds;
		userIds.emplace_back(1);

		DataOwner *dataOwner = new DataOwner();
		Server *server = new Server(userIds,eid);
		DataUser *dataUser1 = new DataUser(1,eid);
		dataOwner->server = server;
		dataUser1->server = server;

		//初始化实验参数
		std::ifstream file("../DataSet/Lab1DataSet15");
		string toSearchWord = "HXHIPC";

		unordered_map<string,vector<int>> dataSet;
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				vector<string> vc = split_string(line);
				dataSet[vc[0]] = vector<int>();
				int size = vc.size();
				for(int i = 1 ; i < size ; i++){
					dataSet[vc[0]].emplace_back(stoi(vc[i]));
				}
			}
			file.close();
		} else {
			std::cout << "Unable to open file" << std::endl;
			return 1;
		}

		unordered_map<int,vector<string>> dataSet_reverted;
		for (const auto& pair : dataSet) {
			for (size_t i = 0; i < pair.second.size(); ++i) {
				dataSet_reverted[pair.second[i]].emplace_back(pair.first);
				dataOwner->AccessList[pair.second[i]].insert(1);
				server->AccessList[pair.second[i]].insert(1);
			}
		}
		for (const auto& pair : dataSet_reverted) {
			vector<string> WList;
			for (size_t i = 0; i < pair.second.size(); ++i) {
				WList.emplace_back(pair.second[i]);
			}
			dataOwner->update(pair.first,WList,ADD);
		}
		clock_t start = clock();
		vector<int> Res = dataUser1->Search(toSearchWord);
		clock_t end = clock();

    	double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    	std::cout << "运行时间: " << duration << " 秒" << std::endl;

		cout<<"搜索的结果个数："<<dec<<Res.size()<<endl;
	}else if(status == 2){
		// 初始化server、dataowner和datauser
		vector<int> userIds;
		userIds.emplace_back(1);

		DataOwner *dataOwner = new DataOwner();
		Server *server = new Server(userIds,eid);
		DataUser *dataUser1 = new DataUser(1,eid);
		dataOwner->server = server;
		dataUser1->server = server;

		//初始化实验参数
		std::ifstream file("../DataSet/Lab1DataSet5");
		string toAddWord = "AAAAAAAAAAA";

		unordered_map<string,vector<int>> dataSet;
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				vector<string> vc = split_string(line);
				dataSet[vc[0]] = vector<int>();
				int size = vc.size();
				for(int i = 1 ; i < size ; i++){
					dataSet[vc[0]].emplace_back(stoi(vc[i]));
				}
			}
			file.close();
		} else {
			std::cout << "Unable to open file" << std::endl;
			return 1;
		}

		unordered_map<int,vector<string>> dataSet_reverted;
		for (const auto& pair : dataSet) {
			for (size_t i = 0; i < pair.second.size(); ++i) {
				dataSet_reverted[pair.second[i]].emplace_back(pair.first);
				dataOwner->AccessList[pair.second[i]].insert(1);
				server->AccessList[pair.second[i]].insert(1);
			}
		}
		for (const auto& pair : dataSet_reverted) {
			vector<string> WList;
			for (size_t i = 0; i < pair.second.size(); ++i) {
				WList.emplace_back(pair.second[i]);
			}
			dataOwner->update(pair.first,WList,ADD);
		}
		int ind = 1;
		vector<string> WList = {toAddWord};
		for(int i = 0 ; i < 4 ; i++){
			clock_t start = clock();
			for(int j = 1 ; j <= 5 * (i + 1) ; j++){
				dataOwner->update(i * 1000 + j,WList,ADD);
			}
			clock_t end = clock();
    		double duration = static_cast<double>(end - start);
			cout<< duration<<endl;
    	// std::cout << "运行时间: " << duration << " 秒" << std::endl;
		}
		// cout<<"搜索的结果个数："<<dec<<Res.size()<<endl;
	}else if(status == 3){
		// 初始化server、dataowner和datauser
		vector<int> userIds;
		userIds.emplace_back(1);

		DataOwner *dataOwner = new DataOwner();
		Server *server = new Server(userIds,eid);
		DataUser *dataUser1 = new DataUser(1,eid);
		dataOwner->server = server;
		dataUser1->server = server;

		//初始化实验参数
		std::ifstream file("../DataSet/Lab1DataSet5");
		string toRevokeWord = "AAAAAAAAAAA";

		unordered_map<string,vector<int>> dataSet;
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				vector<string> vc = split_string(line);
				dataSet[vc[0]] = vector<int>();
				int size = vc.size();
				for(int i = 1 ; i < size ; i++){
					dataSet[vc[0]].emplace_back(stoi(vc[i]));
				}
			}
			file.close();
		} else {
			std::cout << "Unable to open file" << std::endl;
			return 1;
		}

		unordered_map<int,vector<string>> dataSet_reverted;
		for (const auto& pair : dataSet) {
			for (size_t i = 0; i < pair.second.size(); ++i) {
				dataSet_reverted[pair.second[i]].emplace_back(pair.first);
				dataOwner->AccessList[pair.second[i]].insert(1);
				server->AccessList[pair.second[i]].insert(1);
			}
		}
		for (const auto& pair : dataSet_reverted) {
			vector<string> WList;
			for (size_t i = 0; i < pair.second.size(); ++i) {
				WList.emplace_back(pair.second[i]);
			}
			dataOwner->update(pair.first,WList,ADD);
		}
		vector<string> WList = {toRevokeWord};
		for(int ind = 1 ; ind <= 1000 ; ind++){
			dataOwner->update(ind,WList,ADD);
		}
		int ind = 1;
		for(int i = 0 ; i < 4 ; i++){
			clock_t start = clock();
			for(int j = 1 ; j <= 50 * (i + 1) ; j++){
				dataOwner->update(ind++,WList,DEL);
			}
			clock_t end = clock();
    		double duration = static_cast<double>(end - start);
			cout<< duration<<endl;
		}
	}else if(status == 4){
		int delNum = atoi(argv[1]);
		string dataSetNumStr = string(argv[2]);
		int dataSetNum = atoi(argv[2]);

		vector<int> userIds;
		userIds.emplace_back(1);

		DataOwner *dataOwner = new DataOwner();
		Server *server = new Server(userIds,eid);
		DataUser *dataUser1 = new DataUser(1,eid);
		dataOwner->server = server;
		dataUser1->server = server;

		//初始化实验参数
		std::ifstream file("../DataSet/Lab1DataSet"+dataSetNumStr);
		// cout<<"../DataSet/Lab1DataSet"+dataSetNumStr<<endl;
		string toSearchWord = target_keys[dataSetNum - 1];

		unordered_map<string,vector<int>> dataSet;
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				vector<string> vc = split_string(line);
				dataSet[vc[0]] = vector<int>();
				int size = vc.size();
				for(int i = 1 ; i < size ; i++){
					dataSet[vc[0]].emplace_back(stoi(vc[i]));
				}
			}
			file.close();
		} else {
			std::cout << "Unable to open file" << std::endl;
			return -1;
		}

		unordered_map<int,vector<string>> dataSet_reverted;
		for (const auto& pair : dataSet) {
			for (size_t i = 0; i < pair.second.size(); ++i) {
				dataSet_reverted[pair.second[i]].emplace_back(pair.first);
				dataOwner->AccessList[pair.second[i]].insert(1);
				server->AccessList[pair.second[i]].insert(1);
			}
		}
		for (const auto& pair : dataSet_reverted) {
			vector<string> WList;
			for (size_t i = 0; i < pair.second.size(); ++i) {
				WList.emplace_back(pair.second[i]);
			}
			dataOwner->update(pair.first,WList,ADD);
		}

		vector<string> WList = {toSearchWord};
		for(int i = 0 ; i < delNum ; i++){
			dataOwner->update(dataSet[toSearchWord][i],WList,DEL);
		}
		clock_t start = clock();
		vector<int> Res = dataUser1->Search(toSearchWord);
		clock_t end = clock();

    	double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
		cout<<duration<<endl;
    	// std::cout << "运行时间: " << duration << " 秒" << std::endl;

		// cout<<"搜索的结果个数："<<dec<<Res.size()<<endl;
		return duration;
	}else if(status == 5){
		int userNum = atoi(argv[1]);

		vector<int> userIds;
		vector<DataUser *> dataUsers;
		Server *server = new Server(userIds,eid);
		for(int i = 1 ; i <= userNum ; i++){
			userIds.emplace_back(i);
			DataUser* dataUser = new DataUser(i,eid);
			dataUser->server = server;
			dataUsers.emplace_back(dataUser);
		}

		DataOwner *dataOwner = new DataOwner();
		dataOwner->server = server;

		//初始化实验参数
		std::ifstream file("../DataSet/Lab1DataSet8");
		// cout<<"../DataSet/Lab1DataSet"+dataSetNumStr<<endl;
		string toSearchWord = target_keys[8 - 1];

		unordered_map<string,vector<int>> dataSet;
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				vector<string> vc = split_string(line);
				dataSet[vc[0]] = vector<int>();
				int size = vc.size();
				for(int i = 1 ; i < size ; i++){
					dataSet[vc[0]].emplace_back(stoi(vc[i]));
				}
			}
			file.close();
		} else {
			std::cout << "Unable to open file" << std::endl;
			return -1;
		}
		// cout<<"11111"<<endl;
		unordered_map<int,vector<string>> dataSet_reverted;
		// cout<<dataUsers.size()<<endl;
		for (const auto& pair : dataSet) {
			for (size_t i = 0; i < pair.second.size(); ++i) {
				dataSet_reverted[pair.second[i]].emplace_back(pair.first);
				for(DataUser* user : dataUsers){
					dataOwner->AccessList[pair.second[i]].insert((*user).userId);
					server->AccessList[pair.second[i]].insert((*user).userId);
				}
			}
		}
		// cout<<"22222"<<endl;
		for (const auto& pair : dataSet_reverted) {
			vector<string> WList;
			for (size_t i = 0; i < pair.second.size(); ++i) {
				WList.emplace_back(pair.second[i]);
			}
			dataOwner->update(pair.first,WList,ADD);
		}
		vector<string> WList;
		WList.emplace_back(toSearchWord);
		for(int i = 0 ; i < 150 ; i++){
			dataOwner->update(dataSet[toSearchWord][i],WList,DEL);
		}
		int time = 0;
		// cout<<"333333"<<endl;
		for(DataUser* dataUser : dataUsers){
			// cout<<(*dataUser).userId<<endl;
			clock_t start = clock();
			vector<int> Res = dataUser->Search(toSearchWord);
			clock_t end = clock();
			time = max(time,(int)(end - start));
		}
		// cout<<"444444"<<endl;


		double duration = static_cast<double>(time) / CLOCKS_PER_SEC;
		cout<<duration<<endl;
		// std::cout << "运行时间: " << duration << " 秒" << std::endl;

		// cout<<"搜索的结果个数："<<dec<<Res.size()<<endl;
		return duration;
	}

	/************************一些用来验证api的代码******************************************/

	// ecall_init(eid);
	// auto *key = (unsigned char*) "0123456789123456";
    // auto *iv = (unsigned char*) "0123456789123456";
    // auto *plaintext = (unsigned char*) "The test segmentation for AES_CTR mode";

    // unsigned char ciphertext[128];

	// int ciphertext_len = aes_encrypt(plaintext, strlen((char*) plaintext),
    //         key, iv,
    //         ciphertext);

	// // printHexBytes(string((char*)ciphertext,ciphertext_len));

    // // cout << "Input size:" << ciphertext_len << endl;
    // // printf("%d",ciphertext_len);

    // unsigned char recover[128];
    // int plaintext_len = aes_decrypt(ciphertext, ciphertext_len,
    //         key, iv,recover);

    // // cout << "Output size:" << plaintext_len << endl;
    // // cout <<"Recovered string:" << recover << endl;
    // // printf("%d",plaintext_len);
    // // printf("%s",recover);


    // int val = 123;
    // unsigned char digest[AES_BLOCK_SIZE];
    // // key_derivation((uint8_t*) &val,sizeof(int),key,AES_BLOCK_SIZE,digest);
    // // for (size_t i = 0; i < AES_BLOCK_SIZE; i++) {
    // //     printf("%x ", digest[i]);
    // // }
    // // printf("\n");
	// // sha256_digest((uint8_t *) &val,sizeof(int),digest);
	// // for (size_t i = 0; i < AES_BLOCK_SIZE; i++) {
    // //     printf("%x ", digest[i]);
    // // }
    // // printf("\n");

	return 0;
}

