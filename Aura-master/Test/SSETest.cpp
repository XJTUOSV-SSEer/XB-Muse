#include "Core/SSEClientHandler.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <unordered_map>

using namespace std;

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

void test0(int argc,char* argv[]){
    SSEClientHandler client;

//    cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() << endl;
    for (int i = 0; i < 200; ++i) {
        client.update(INS, "test", i);
    }
//    cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() << endl;
    for (int i = 0; i < 10; ++i) {
        client.update(DEL, "test", i);
    }

    vector<int> results = client.search("test");
    for (int res : results) {
        cout << res << endl;
    }
}

//compare_search_a
void test1(int argc,char* argv[]){

    string dataSetPath = "../../DataSet/enron_processed";
    string targetKey = string(argv[2]);


    SSEClientHandler client;
    unordered_map<string,vector<int>> dataSet;
    unordered_map<int,vector<string>> dataSet_reverted;
    init_data_set(dataSetPath,dataSet,dataSet_reverted);

    for(auto& pair:dataSet){
        for(int value:pair.second){
            client.update(INS,pair.first,value);
        }
    }

    for(int i = 0 ; i < 20 ; i++){
        client.update(DEL,targetKey,dataSet[targetKey][i]);
    }

    clock_t start = clock();
	vector<int> Res = client.search(targetKey);
	clock_t end = clock();

    double duration = static_cast<double>(end - start) / 1000;
	cout<<duration<<endl;
}

//compare_search_b
void test2(int argc,char* argv[]){
    
}

//compare_update_a
void test3(int argc,char* argv[]){

    string dataSetPath = "../../DataSet/enron_processed";
    string targetKey = string(argv[2]);

    SSEClientHandler client;
    unordered_map<string,vector<int>> dataSet;
    unordered_map<int,vector<string>> dataSet_reverted;
    init_data_set(dataSetPath,dataSet,dataSet_reverted);

    for(auto& pair:dataSet){
        for(int value:pair.second){
            client.update(INS,pair.first,value);
        }
    }
    clock_t start = clock();
    for(int i = 0 ; i < 40 ; i++){
        client.update(DEL,targetKey,dataSet[targetKey][i]);
    }

    clock_t end = clock();

    double duration = static_cast<double>(end - start) / 1000;
	cout<<duration<<endl;
}

//compare_update_b
void test4(int argc,char* argv[]){
    
}

int main(int argc,char* argv[]) {

    int status = atoi(argv[1]);

	if(status == 0){
		test0(argc,argv);
	}else if(status == 1){
		test1(argc,argv);
	}else if(status == 2){
		test2(argc,argv);
	}else if(status == 3){
		test3(argc,argv);
	}else{
		test4(argc,argv);
	}

    return 0;
}