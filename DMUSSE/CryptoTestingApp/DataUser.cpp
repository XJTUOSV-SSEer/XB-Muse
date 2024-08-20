#include "DataUser.h"
#include <string>

#include "stdio.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <vector>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <cstring> 
#include <openssl/rand.h>

#include <iomanip>

DataUser::DataUser(int userId,int eid){
    this->userId = userId;
    this->eid = eid;
}

vector<int> DataUser::Search(string w){
    // cout<<"DataUser::Search : 1"<<endl;
    vector<int> Rw;
    vector<string> TList;
    vector<string> diff = server->Addrs[userId];

    

    return Rw;
}