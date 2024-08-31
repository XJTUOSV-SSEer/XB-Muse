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
		dataOwner->insert(pair.first,WList);
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
void test0(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
// 初始化server、dataowner和datauser
	vector<int> userIds;
	userIds.emplace_back(1);
	userIds.emplace_back(2);

	DataOwner *dataOwner = new DataOwner(&io_service,endpoint_iterator);
	Server *server = new Server(userIds,eid);
	DataUser *dataUser1 = new DataUser(1,eid,&io_service,endpoint_iterator);
	DataUser *dataUser2 = new DataUser(2,eid,&io_service,endpoint_iterator);
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

	dataOwner->insert(21,WList);	//为21号文件添加WList中的关键字
	dataOwner->insert(21,WList);	//测试多次添加不会出现问题
	dataOwner->insert(1,WList);		//为1号文件添加WList中的关键字


	for (auto it = WList.begin(); it != WList.end();) {
		if (*it == "b") {
			it = WList.erase(it);
		} else {
			++it;
		}
	}
	dataOwner->insert(12,WList);
	cout << "check point 1"<<endl;
	//测试用例1----------------------------------------------------------------
	vector<int> Res;


	Res = dataUser1->Search_batch("a");
	cout<<"user1搜索a的结果："<<dec<<endl;
	for(int i : Res){
		cout<< i << " ";
	}
	cout<<endl;


	// 测试用例2----------------------------------------------------------------
	Res = dataUser2->Search_batch("b");
	cout<<"user1搜索b的结果："<<dec<<endl;
	for(int i : Res){
		cout<< i << " ";
	}
	cout<<endl;

	// 测试用例3----------------------------------------------------------------
	
	vector<int> revokeIndList;
	revokeIndList.emplace_back(1);
	// cout<<1<<endl;
	dataOwner->revoke("a",revokeIndList);

	Res = dataUser1->Search_batch("a");
	cout<<"撤销a与1的关系后user1搜索a的结果："<<dec<<endl;
	for(int i : Res){
		cout<< i << " ";
	}
	cout<<endl;

	// 测试用例4----------------------------------------------------------------
	// cout<<1<<endl;'
	revokeIndList = {21};
	dataOwner->revoke("a",revokeIndList);

	Res = dataUser1->Search_batch("a");
	cout<<"撤销a与21的关系后user1搜索a的结果："<<dec<<endl;
	for(int i : Res){
		cout<< i << " ";
	}
	cout<<endl;
}

//search - a
void test1(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    
}

//search - b
void test2(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    
}

//search - c
void test3(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    
}

//search - d
void test4(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
	
	vector<int> args = prase_argv_to_int(argc,argv);
    string dataSetPath = "../DataSet/Lab1DataSet"+to_string(args[1]);
    string targetKey = target_keys[args[1] - 1];
	

	// 初始化server、dataowner和datauser
	vector<int> userIds;
	userIds.emplace_back(1);
	DataOwner *dataOwner = new DataOwner(&io_service,endpoint_iterator);
	Server *server = new Server(userIds,eid);
	DataUser *dataUser1 = new DataUser(1,eid,&io_service,endpoint_iterator);
	dataOwner->server = server;
	dataUser1->server = server;

	//处理数据集
	unordered_map<string,vector<int>> dataSet;
	unordered_map<int,vector<string>> dataSet_reverted;
	init_data_set(dataSetPath,dataSet,dataSet_reverted);
	auth_all(dataOwner,dataSet,1);
	auth_all(server,dataSet,1);
	update_all(dataOwner,dataSet_reverted);

	
	int toRevokeBatchNum = args[2];
	for(int i = 0 ; i < toRevokeBatchNum ; i++){
		vector<int> toRevokeIndList;
		toRevokeIndList.insert(toRevokeIndList.end(),dataSet[targetKey].begin() + i * 300,dataSet[targetKey].begin() + (i + 1) * 300);
		dataOwner->revoke(targetKey,toRevokeIndList);
	}
	clock_t start = clock();
	vector<int> Res = dataUser1->Search_batch(targetKey);
	clock_t end = clock();

    double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
	cout<<duration<<endl;
}

//update - a
void test5(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    
}

//update - b
//args 列表含义：数据集标识 要撤销的数据量/50
void test6(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
	
	vector<int> args = prase_argv_to_int(argc,argv);
    string dataSetPath = "../DataSet/Lab1DataSet"+to_string(args[1]);
    string targetKey = target_keys[args[1] - 1];

	
	// 初始化server、dataowner和datauser
	vector<int> userIds;
	userIds.emplace_back(1);
	DataOwner *dataOwner = new DataOwner(&io_service,endpoint_iterator);
	Server *server = new Server(userIds,eid);
	DataUser *dataUser1 = new DataUser(1,eid,&io_service,endpoint_iterator);
	dataOwner->server = server;
	dataUser1->server = server;

	//处理数据集
	unordered_map<string,vector<int>> dataSet;
	unordered_map<int,vector<string>> dataSet_reverted;
	init_data_set(dataSetPath,dataSet,dataSet_reverted);
	auth_all(dataOwner,dataSet,1);
	auth_all(server,dataSet,1);
	update_all(dataOwner,dataSet_reverted);


	string toRevokeWord = "AAAAAAAAAAA";
	vector<string> WList = {toRevokeWord};
	for(int ind = 1 ; ind <= 1000 ; ind++){
		dataOwner->insert(ind,WList);
	}
	vector<vector<int>> IDLists;
	for(int i = 0 ; i < args[2] ; i++){
		vector<int> IDList;
		for(int j = 50 * i + 1; j <= 50 * (i + 1) + 1 ; j++){
			IDList.emplace_back(j);
		}
		IDLists.emplace_back(IDList);
	}

	clock_t start = clock();
	for(vector<int> IDList : IDLists){
		dataOwner->revoke(toRevokeWord,IDList);
	}
	clock_t end = clock();
	double duration = static_cast<double>(end - start);
	cout<< duration<<endl;

}

//update - c
void test7(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    
}

//update - d
void test8(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    
}

//compare_search_a
void test9(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
	
	string dataSetPath = string(argv[2]);
    string targetKey = "566";
	

	// 初始化server、dataowner和datauser
	vector<int> userIds;
	userIds.emplace_back(1);

	DataOwner *dataOwner = new DataOwner(&io_service,endpoint_iterator);
	Server *server = new Server(userIds,eid);
	DataUser *dataUser1 = new DataUser(1,eid,&io_service,endpoint_iterator);
	dataOwner->server = server;
	dataUser1->server = server;

	//处理数据集
    unordered_map<string,vector<int>> dataSet;
    unordered_map<int,vector<string>> dataSet_reverted;
    init_data_set(dataSetPath,dataSet,dataSet_reverted);
	auth_all(dataOwner,dataSet,1);
	auth_all(server,dataSet,1);
	update_all(dataOwner,dataSet_reverted);


	vector<string> WList = {targetKey};
	vector<int> toRevokeList;
	for(int i = 0 ; i < 20 ; i++){
        toRevokeList.emplace_back(dataSet[targetKey][i]);
    }
	dataOwner->revoke(targetKey,toRevokeList);

	clock_t start = clock();
	vector<int> Res = dataUser1->Search_batch(targetKey);
	clock_t end = clock();

    double duration = static_cast<double>(end - start) / 1000;
	cout<<duration<<endl;
}

//compare_search_b
void test10(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){

}

//compare_update_a
void test11(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
	
	string dataSetPath = "../DataSet/enron_processed";
    string targetKey = string(argv[2]);

	
	// 初始化server、dataowner和datauser
	vector<int> userIds;
	userIds.emplace_back(1);
	DataOwner *dataOwner = new DataOwner(&io_service,endpoint_iterator);
	Server *server = new Server(userIds,eid);
	DataUser *dataUser1 = new DataUser(1,eid,&io_service,endpoint_iterator);
	dataOwner->server = server;
	dataUser1->server = server;

	//处理数据集
    unordered_map<string,vector<int>> dataSet;
    unordered_map<int,vector<string>> dataSet_reverted;
    init_data_set(dataSetPath,dataSet,dataSet_reverted);
	auth_all(dataOwner,dataSet,1);
	auth_all(server,dataSet,1);
	update_all(dataOwner,dataSet_reverted);

	
	vector<string> WList = {targetKey};
	vector<int> toRevokeList;
	for(int i = 0 ; i < 40 ; i++){
        toRevokeList.emplace_back(dataSet[targetKey][i]);
    }
	clock_t start = clock();
	dataOwner->revoke(targetKey,toRevokeList);
	clock_t end = clock();

    double duration = static_cast<double>(end - start) / 1000;
	cout<<duration<<endl;
}

//compare_update_b
void test12(int argc,char* argv[],int eid,boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
	
	string dataSetPath = "../DataSet/Lab1DataSet7";
    string targetKey = "URNOIDX";
	
	// 初始化server、dataowner和datauser
	vector<int> userIds;
	userIds.emplace_back(1);
	DataOwner *dataOwner = new DataOwner(&io_service,endpoint_iterator);
	Server *server = new Server(userIds,eid);
	DataUser *dataUser1 = new DataUser(1,eid,&io_service,endpoint_iterator);
	dataOwner->server = server;
	dataUser1->server = server;

	//处理数据集
    unordered_map<string,vector<int>> dataSet;
    unordered_map<int,vector<string>> dataSet_reverted;
    init_data_set(dataSetPath,dataSet,dataSet_reverted);
	auth_all(dataOwner,dataSet,1);
	auth_all(server,dataSet,1);
	update_all(dataOwner,dataSet_reverted);

	
	vector<string> WList = {targetKey};
	vector<int> toRevokeList;
	for(int i = 0 ; i < 200 ; i++){
        toRevokeList.emplace_back(dataSet[targetKey][i]);
    }
	clock_t start = clock();
	dataOwner->revoke(targetKey,toRevokeList);
	clock_t end = clock();

    double duration = static_cast<double>(end - start) / 1000;
	cout<<duration<<endl;
}