#include "test.h"

#include "../common/data_type.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <ctime>

using namespace std;

vector<string> target_keys = {
    "YADOXGCCJI",
    "CGAMP",
    "AFA",
    "VMO",
    "KPXGT",

    "FLPTGGDGBO",
    "URNOIDX",
    "XAHOE",
    "NHB",
    "CM",

    "QZV",
    "GX",
    "VUQHKM",
    "EVVOOPNK",
    "HXHIPC",

    "ELGPS",
    "SQ"
};

std::vector<std::string> split_string(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, ' ')) {
        result.push_back(token);
    }

    return result;
}

int init_data_set(string filePath,unordered_map<string,vector<int>> &dataSet,unordered_map<int,vector<string>> &dataSet_reverted){
	//初始化实验参数
	std::ifstream file(filePath);

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

	for (const auto& pair : dataSet) {
		for (size_t i = 0; i < pair.second.size(); ++i) {
			dataSet_reverted[pair.second[i]].emplace_back(pair.first);
		}
	}

	return 0;
}

void auth_all(DataOwner *dataOwner,unordered_map<string,vector<int>> &dataSet,int dataUserId){
    for (const auto& pair : dataSet) {
		for (size_t i = 0; i < pair.second.size(); ++i) {
			dataOwner->AccessList[pair.second[i]].insert(dataUserId);
		}
	}
}

void auth_all(Server *server,unordered_map<string,vector<int>> &dataSet,int dataUserId){
    for (const auto& pair : dataSet) {
		for (size_t i = 0; i < pair.second.size(); ++i) {
			server->AccessList[pair.second[i]].insert(dataUserId);
		}
	}
}

void update_all(DataOwner *dataOwner,unordered_map<int,vector<string>> &dataSet_reverted){
    for (const auto& pair : dataSet_reverted) {
		vector<string> WList;
		for (size_t i = 0; i < pair.second.size(); ++i) {
			WList.emplace_back(pair.second[i]);
		}
		dataOwner->update(pair.first,WList,ADD);
	}
}

std::vector<int> prase_argv_to_int(int argc,char* argv[]){

	std::vector<int> args;
	for(int i = 1 ; i < argc ; i++){
		args.emplace_back(atoi(argv[i]));
	}
	return args;
}

//test
void test0(vector<int> args,int eid){
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
}

//compare_search_a
void test1(int argc,char* argv[],int eid){
    // 初始化server、dataowner和datauser
	vector<int> userIds;
	userIds.emplace_back(1);

	DataOwner *dataOwner = new DataOwner();
	Server *server = new Server(userIds,eid);
	DataUser *dataUser1 = new DataUser(1,eid);
	dataOwner->server = server;
	dataUser1->server = server;

	//处理数据集
    string dataSetPath = string(argv[2]);
    unordered_map<string,vector<int>> dataSet;
    unordered_map<int,vector<string>> dataSet_reverted;
    init_data_set(dataSetPath,dataSet,dataSet_reverted);
	auth_all(dataOwner,dataSet,1);
	auth_all(server,dataSet,1);
	update_all(dataOwner,dataSet_reverted);

    string targetKey = "566";

	vector<string> WList = {targetKey};
	vector<int> toRevokeList;
	for(int i = 0 ; i < 20 ; i++){
        dataOwner->update(dataSet[targetKey][i],WList,DEL);
    }

	clock_t start = clock();
	vector<int> Res = dataUser1->Search(targetKey);
	clock_t end = clock();

    double duration = static_cast<double>(end - start) / 1000;
	cout<<duration<<endl;
}

//search - b
void test2(vector<int> args,int eid){
    
}

//search - c
void test3(vector<int> args,int eid){
    
}

//search - d
void test4(vector<int> args,int eid){
    // 初始化server、dataowner和datauser
	vector<int> userIds;
	userIds.emplace_back(1);

	DataOwner *dataOwner = new DataOwner();
	Server *server = new Server(userIds,eid);
	DataUser *dataUser1 = new DataUser(1,eid);
	dataOwner->server = server;
	dataUser1->server = server;

	string toSearchWord = target_keys[args[1] - 1];

	unordered_map<string,vector<int>> dataSet;
	unordered_map<int,vector<string>> dataSet_reverted;
	// cout << "test4:2" <<endl;
	init_data_set("../DataSet/Lab1DataSet"+to_string(args[1]),dataSet,dataSet_reverted);
	// cout << "test4:3" <<endl;
	auth_all(dataOwner,dataSet,1);
	// cout << "test4:4" <<endl;
	auth_all(server,dataSet,1);
	// cout << "test4:5" <<endl;
	update_all(dataOwner,dataSet_reverted);
	// cout << "test4:6" <<endl;

	vector<int> toRevokeList;
	toRevokeList.insert(toRevokeList.end(),dataSet[toSearchWord].begin(),dataSet[toSearchWord].begin() + args[2]);
	// cout << "test4:7" <<endl;
	clock_t start = clock();
	vector<int> Res = dataUser1->Search(toSearchWord);
	// cout << "test4:8" <<endl;
	clock_t end = clock();

    double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
	cout<<duration<<endl;
}

//update - a
void test5(vector<int> args,int eid){
    
}

//update - b
void test6(vector<int> args,int eid){
    
}

//update - c
void test7(vector<int> args,int eid){
    
}

//update - d
void test8(vector<int> args,int eid){
    
}