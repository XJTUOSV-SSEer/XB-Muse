
#include <string>
#include "stdio.h"
#include "stdlib.h"

#include "sgx_urts.h"
#include "CryptoEnclave_u.h"

#include "../common/data_type.h"
#include "../common/encrypt_config.h"

#include "Server.h"
#include "DataOwner.h"
#include "DataUser.h"
#include "Utils.h"

//for measurement
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

std::vector<std::string> split_string(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, ' ')) {
        result.push_back(token);
    }

    return result;
}


//main func
int main()
{
	std::cout<< "不可信部分构建成功" <<std::endl;
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

	int status = 1;

	if(status == 0){
		// 初始化server、dataowner和datauser
		vector<int> userIds;
		userIds.emplace_back(1);
		userIds.emplace_back(2);

		DataOwner *dataOwner = new DataOwner();
		Server *server = new Server(userIds,eid);
		DataUser *dataUser1 = new DataUser(1,eid);
		DataUser *dataUser2 = new DataUser(2,eid);
		dataOwner->server = server;
		dataUser1->server = server;
		dataUser2->server = server;

	// ----------------------------------------------------------------

		//初始化实验参数
		//将第1号文件授权给userId为1的用户
		dataOwner->AccessList[1].insert(1);

		//将第21号文件授权给userId为1和2的用户
		dataOwner->AccessList[21].insert(1);
		dataOwner->AccessList[21].insert(2);

		dataOwner->AccessList[12].insert(1);



		server->AccessList[1].insert(1);

		server->AccessList[21].insert(1);
		server->AccessList[21].insert(2);

		server->AccessList[12].insert(1);


		vector<string> WList;

		WList.emplace_back("a");
		WList.emplace_back("b");
		WList.emplace_back("c");
		WList.emplace_back("d");

		dataOwner->update(21,WList,ADD);	//为21号文件添加WList中的关键字
		dataOwner->update(21,WList,ADD);	//测试多次添加不会出现问题
		dataOwner->update(1,WList,ADD);		//为1号文件添加WList中的关键字

		for (auto it = WList.begin(); it != WList.end();) {
			if (*it == "b") {
				it = WList.erase(it);
			} else {
				++it;
			}
		}
		dataOwner->update(12,WList,ADD);

		//测试用例1----------------------------------------------------------------
		vector<int> Res = dataUser1->Search("a");
		cout<<"user1搜索a的结果："<<dec<<endl;
		for(int i : Res){
			cout<< i << " ";
		}
		cout<<endl;

		//测试用例2----------------------------------------------------------------
		Res = dataUser1->Search("b");
		cout<<"user1搜索b的结果："<<dec<<endl;
		for(int i : Res){
			cout<< i << " ";
		}
		cout<<endl;

		// 测试用例3----------------------------------------------------------------
		Res = dataUser2->Search("a");
		cout<<"user2搜索a的结果："<<dec<<endl;
		for(int i : Res){
			cout<< i << " ";
		}
		cout<<endl;
		
		//测试用例4----------------------------------------------------------------
		vector<string> revokeWList;
		revokeWList.emplace_back("a");
		dataOwner->update(1,revokeWList,DEL);
		Res = dataUser1->Search("a");
		cout<<"撤销1号文件上a关键字后user1搜索a的搜索结果："<<endl;
		for(int i : Res){
			cout<< i << " ";
		}
		cout<<endl;

		//测试用例5----------------------------------------------------------------
		revokeWList = {"b"};
		dataOwner->update(21,revokeWList,DEL);
		Res = dataUser1->Search("b");
		cout<<"撤销21号文件上b关键字后user1搜索b的搜索结果："<<endl;
		for(int i : Res){
			cout<< i << " ";
		}
		cout<<endl;
	}else if(status == 1){
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
		// for (const auto& pair : dataSet) {
		// 	std::cout << "Key: " << pair.first << ", Values: [";
		// 	for (size_t i = 0; i < pair.second.size(); ++i) {
		// 		std::cout << pair.second[i];
		// 		if (i < pair.second.size() - 1) {
		// 			std::cout << ", ";
		// 		}
		// 	}
		// 	std::cout << "]" << std::endl;
		// }

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









	/****************************actual opreation******************************/
	// printf("Adding doc\n");
	
	// /*** 处理插入操作Update Protocol with op = add */
	// uint64_t start_add_time =  timeSinceEpochMillisec(); //插入操作开始时间
	// for(int i=1;i <= total_file_no; i++){  //total_file_no 多个Update
	// 	//client read a document
	// 	//printf("->%d",i);
	// 	docContent *fetch_data;//原始文档
	// 	fetch_data = (docContent *)malloc(sizeof( docContent));
    //     //获取下一篇doc
	// 	myClient->ReadNextDoc(fetch_data);

	// 	//encrypt and send to Server 
	// 	entry *encrypted_entry;
	// 	encrypted_entry = (entry*)malloc(sizeof(entry));
		
	// 	encrypted_entry->first.content_length = fetch_data->id.id_length; //初始化长度
	// 	encrypted_entry->first.content = (char*) malloc(fetch_data->id.id_length);
	// 	encrypted_entry->second.message_length = fetch_data->content_length + AESGCM_MAC_SIZE + AESGCM_IV_SIZE;	//初始化长度
	// 	encrypted_entry->second.message = (char *)malloc(encrypted_entry->second.message_length);

	// 	//客户端对doc进行加密,结果存入entry实体中
	// 	myClient->EncryptDoc(fetch_data,encrypted_entry);
		
	// 	//send(id,f) to server
	// 	myServer->ReceiveEncDoc(encrypted_entry);
		
	// 	//upload (op,id) to Enclave
	// 	/*****************更新enclave中数据结构*************************/
	// 	//encalve Update所有操作
	// 	//Question: 这个多出一个sgx id的参数是sgx的特性吗？
	// 	ecall_addDoc(eid,fetch_data->id.doc_id,fetch_data->id.id_length,
	// 				fetch_data->content,fetch_data->content_length);
	// 	/**************************************************************/

	// 	//free memory 
	// 	free(fetch_data->content);
	// 	free(fetch_data->id.doc_id);
	// 	free(fetch_data);

	// 	free(encrypted_entry->first.content);
	// 	free(encrypted_entry->second.message);
	// 	free(encrypted_entry);
	// }
	// uint64_t end_add_time =  timeSinceEpochMillisec(); //插入操作结束时间
	// std::cout << "********Time for adding********" << std::endl;
	// std::cout << "Total time:" << end_add_time-start_add_time << " ms" << std::endl;
	// std::cout << "Average time (file):" << (end_add_time-start_add_time)*1.0/total_file_no << " ms" << std::endl;
	// std::cout << "Average time (pair):" << (end_add_time-start_add_time)*1.0/total_pair_no << " ms" << std::endl;

	// //** 处理删除操作Update Protocol with op = del (id)
	// printf("\nDeleting doc\n");
	// uint64_t start_del_time =  timeSinceEpochMillisec(); //删除操作开始时间
	// //docId* delV = new docId[del_no];
	// docId delV_i; //docID:文件ID数据结构
	// for(int del_index=1; del_index <=del_no; del_index++){
	// 	//printf("->%s",delV_i[del_index].doc_id);
	// 	myClient->Del_GivenDocIndex(del_index, &delV_i);
    //     /*****************在enclave中查询关键字*************************/
	// 	ecall_delDoc(eid,delV_i.doc_id,delV_i.id_length); //加入到 d 列表
    //     /**************************************************************/
	// }
	// uint64_t end_del_time =  timeSinceEpochMillisec(); //删除操作结束时间
	// std::cout << "********Time for deleting********" << std::endl;
	// std::cout << "Total time:" << end_del_time-start_del_time << " ms" << std::endl;
	// std::cout << "Average time:" << (end_del_time-start_del_time)*1.0/del_no << " ms" << std::endl;

	// free(delV_i.doc_id);

	
    // /*** 处理搜索操作***/
	// // std::string s_keyword[2]= {"list","clinton"}; 
	// std::string s_keyword[1]= {"bird"};
	// int keyword_count = 1; //查询关键字的数量
	// std::cout << "********Time for searching********" << std::endl;
	// uint64_t total_search_time = 0;
	// for (int s_i = 0; s_i < keyword_count; s_i++){
	// 	std::cout << "Searching ==>" << s_keyword[s_i].c_str() << std::endl;
	// 	// printf("\nSearching ==> %s\n", s_keyword[s_i].c_str());
	// 	uint64_t start_time =  timeSinceEpochMillisec();
	// 	// std::cout << timeSinceEpochMillisec() << std::endl;
    //     /*****************将文档id加入删除list*************************/
	// 	ecall_search(eid, s_keyword[s_i].c_str(), s_keyword[s_i].size());//直接对应第三部分Search的所有流程
    //     /*****************将文档id加入删除list*************************/
    //     uint64_t end_time =  timeSinceEpochMillisec();
	// 	// std::cout << timeSinceEpochMillisec() << std::endl;
	// 	std::cout << "Elapsed time:" << end_time-start_time << " ms"  << std::endl;
	// 	total_search_time += end_time-start_time;
	// }
	// std::cout << "Total time:" << total_search_time << " ms" << std::endl;
	// std::cout << "Average time:" << total_search_time*1.0/keyword_count << " ms" << std::endl;

	// delete myClient;
	// // delete myServer;

	return 0;
}

