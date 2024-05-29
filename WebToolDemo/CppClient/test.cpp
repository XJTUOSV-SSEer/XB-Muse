#include <iostream>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <bitset>

#define BIT_SIZE 256
#define SERVICE_IP "192.168.1.109"
#define SERVICE_PORT "12346"

using namespace std;

// void setD_asio(boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator,int userId,std::string w,std::bitset<BIT_SIZE> bits){

//     // 连接到服务器
//     boost::asio::ip::tcp::socket socket(io_service);
//     boost::asio::connect(socket, endpoint_iterator);


//     std::stringstream ss;
//     boost::property_tree::ptree pt;
//     pt.put("userId", userId);
//     pt.put("w", w);
//     pt.put("bit_array", bits.to_string());

//     // 将 property_tree 序列化为 JSON 字符串
//     boost::property_tree::write_json(ss, pt);
//     std::string json_str = ss.str();

//     // 构建 HTTP GET 请求报文
//     std::string request =
//         "POST /setD HTTP/1.1\r\n"
//         "Host: 192.168.1.109:12346\r\n"
//         "Content-Type: application/json\r\n"
//         "Content-Length: " + std::to_string(json_str.length()) + "\r\n"
//         "\r\n" +
//         json_str;

//     cout<<"Set request:"<<endl;
//     cout<<request<<endl<<endl;
//     // 发送 HTTP 请求报文
//     boost::asio::write(socket, boost::asio::buffer(request));
//     socket.close();
// }

void setD(boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator,int userId,std::string w,std::bitset<BIT_SIZE> bits){

    // 连接到服务器
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);


    std::stringstream ss;
    boost::property_tree::ptree pt;
    pt.put("userId", userId);
    pt.put("w", w);
    pt.put("bit_array", bits.to_string());

    // 将 property_tree 序列化为 JSON 字符串
    boost::property_tree::write_json(ss, pt);
    std::string json_str = ss.str();

    // 构建 HTTP GET 请求报文
    std::string request =
        "POST /setD HTTP/1.1\r\n"
        "Host: "+ string(SERVICE_IP) +":"+ string(SERVICE_PORT) +"\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + std::to_string(json_str.length()) + "\r\n"
        "\r\n" +
        json_str;

    cout<<"Set request:"<<endl;
    cout<<request<<endl<<endl;
    // 发送 HTTP 请求报文
    boost::asio::write(socket, boost::asio::buffer(request));

    boost::asio::streambuf response;
    boost::asio::read(socket, response, boost::asio::transfer_at_least(1));//不加这一行就异步了，容易出现读写不一致的问题
    cout<<"------------------"<<endl;
    socket.close();
}

std::bitset<BIT_SIZE> getD(boost::asio::io_service &io_service, boost::asio::ip::tcp::resolver::iterator endpoint_iterator, int userId, std::string w) {
    // 连接到服务器
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    std::stringstream ss;
    boost::property_tree::ptree pt;
    pt.put("userId", userId);
    pt.put("w", w);

    // 将 property_tree 序列化为 JSON 字符串
    boost::property_tree::write_json(ss, pt);
    std::string json_str = ss.str();

    // 构建 HTTP POST 请求报文
    std::string request =
        "GET /getD HTTP/1.1\r\n"
        "Host: "+ string(SERVICE_IP) +":"+ string(SERVICE_PORT) +"\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + std::to_string(json_str.length()) + "\r\n"
        "\r\n" +
        json_str;

    cout << "Get request:" << endl;
    cout << request << endl << endl;

    // 发送 HTTP 请求报文
    boost::asio::write(socket, boost::asio::buffer(request));

    // 读取响应报文
    boost::asio::streambuf response;
    boost::system::error_code error;
    size_t bytes_transferred = 0;
    std::string response_body;

    // 分段读取响应报文
    do {
        bytes_transferred = boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error);
        response_body.append(boost::asio::buffers_begin(response.data()), boost::asio::buffers_begin(response.data()) + bytes_transferred);
        response.consume(bytes_transferred);
    } while (!error);

    if (error != boost::asio::error::eof) {
        std::cout << "Error: " << error.message() << std::endl;
        socket.close();
        return std::bitset<BIT_SIZE>();
    }

    size_t body_start = response_body.find("\r\n\r\n");
    if (body_start == std::string::npos) {
        std::cout << "Error: Invalid HTTP response format" << std::endl;
        socket.close();
        return std::bitset<BIT_SIZE>();
    }

    // 提取响应体
    std::string response_data = response_body.substr(body_start + 4);
    std::string pattern = "bits_array";
    size_t pos = response_data.find(pattern);

    size_t pos1 = response_data.find(':', pos);
    size_t pos2 = response_data.find('"', pos1 + 1);
    size_t pos3 = response_data.find('"', pos2 + 1);
    std::string bit_string = response_data.substr(pos2 + 1 + 2, pos3 - pos2 - 1 - 3);
    cout << "response_data: " << response_data << endl;
    cout << "bit_string: " << bit_string << endl;
    cout << bit_string.size() << endl;

    socket.close();

    return std::bitset<BIT_SIZE>(std::stoull(bit_string, 0, 16));
}

int main(int argc, char* argv[]) {
    try {

        boost::asio::io_service io_service;

        // 解析主机名和端口
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(SERVICE_IP, SERVICE_PORT);
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        std::bitset<BIT_SIZE> bits;
        bits.set(1);
        // cout<<"-----------------------------------------------------------------------------------------------------"<<endl;
        setD(io_service,endpoint_iterator,1,"hello",bits);
        // sleep(1);
        std::bitset<BIT_SIZE> result = getD(io_service,endpoint_iterator,1,"hello");
        // std::cout<<result<<std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}