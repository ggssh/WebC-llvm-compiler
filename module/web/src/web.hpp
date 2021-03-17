//
// Created by 金韬 on 2021/3/8.
//

#ifndef SYSYPLUS_COMPILER_WEB_HPP
#define SYSYPLUS_COMPILER_WEB_HPP

#include <map>

#include <boost/beast.hpp>
#include <boost/asio.hpp>

/// 客户端状态码
#define NOT_INITIALIZED -1
#define RESOLVE_FAILED -2
#define CONNECT_FAILED -3
#define SOCKET_NOT_EXISTS -3
#define NOT_CONNECTED -4
/// 服务器状态码
#define PORT_IS_IN_USE -5
/// 通用状态码
#define ROK 0

using namespace std;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace beast = boost::beast;

extern boost::asio::io_context *_web_io_context;
extern tcp::resolver *_web_resolver;
extern map<int, tcp::socket *> _web_tcp_socket_map;

// Keep 函数名，方便Linking
extern "C" {

/**
 * 初始化网络功能
 * @return 如上的返回值
 */
int _web_init();

/**
 * 获取一个Socket
 * @return
 */
int _web_getSocket();

/**
 * 连接Web
 * @param socketId socketId
 * @param baseUrl eg: kingtous.cn
 * @param port 端口号
 * @return 连接状态
 */
int _web_connectSocket(int socketId, const char *baseUrlOrIp, const char *port);

/**
 * 关闭socket
 * @param socketId
 * @return
 */
int _web_closeSocket(int socketId);

/**
 * socket是否连接
 * @param socketId
 * @return
 */
int _web_isSocketConnected(int socketId);

/**
 * 通过GET请求发送HTTP数据
 * @param host
 * @param socketId
 * @param path eg: /index
 * @return
 */
const char *_web_callGetRequest(int socketId, char *host, char *path);

/**
 * 获取一个本地HTTP服务器的ID
 * @param addr 地址
 * @param port 端口号
 * @return server id
 */
int _web_getServerId(const char *addr, int port);

/**
 * @param sId server id
 * @param path URL path，如：/index
 * @param method 方法
 * @param handler 函数指针，用于返回一个const char*内容
 * @return 状态
 */
int _web_addUrlHandler(int sId, const char *method, const char *path, const char *handler());

/**
 * 开启服务器，程序进入阻塞状态
 * @param sId server id
 * @return 状态
 */
int _web_startServe(int sId);
}

typedef const char *HandlerFunction();

/**
 * handler结构
 */
typedef struct web_http_handler {
    int sId;
    const char *method;
    const char *path;
    HandlerFunction function;
} web_http_handler;

class _web_HttpServer {
public:
    _web_HttpServer(_web_HttpServer const &) = delete;

    _web_HttpServer &operator=(_web_HttpServer const &) = delete;

    _web_HttpServer(tcp::acceptor &acceptor, const string &basePath);

    /**
     * 接收下一个客户
     */
    void accept();

private:
    tcp::acceptor &acceptor;
//    using request_body_t = http::basic_dynamic_body<beast::flat_static_buffer<1024 * 1024>>;
    using request_body_t = http::string_body;
    std::string base_path;
    tcp::socket socket{acceptor.get_executor()};
    boost::optional<http::request_parser<http::string_body>> parser;
    beast::flat_static_buffer<8192> buffer;
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> request_deadline{
            acceptor.get_executor(), (std::chrono::steady_clock::time_point::max) ()};

    void process_request(http::request<request_body_t> const &request);
};


boost::beast::string_view mime_type(boost::beast::string_view path) {
    using boost::beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (pos == boost::beast::string_view::npos)
            return boost::beast::string_view{};
        return path.substr(pos);
    }();
    if (iequals(ext, ".htm")) return "text/html";
    if (iequals(ext, ".html")) return "text/html";
    if (iequals(ext, ".php")) return "text/html";
    if (iequals(ext, ".css")) return "text/css";
    if (iequals(ext, ".txt")) return "text/plain";
    if (iequals(ext, ".js")) return "application/javascript";
    if (iequals(ext, ".json")) return "application/json";
    if (iequals(ext, ".xml")) return "application/xml";
    if (iequals(ext, ".swf")) return "application/x-shockwave-flash";
    if (iequals(ext, ".flv")) return "video/x-flv";
    if (iequals(ext, ".png")) return "image/png";
    if (iequals(ext, ".jpe")) return "image/jpeg";
    if (iequals(ext, ".jpeg")) return "image/jpeg";
    if (iequals(ext, ".jpg")) return "image/jpeg";
    if (iequals(ext, ".gif")) return "image/gif";
    if (iequals(ext, ".bmp")) return "image/bmp";
    if (iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff")) return "image/tiff";
    if (iequals(ext, ".tif")) return "image/tiff";
    if (iequals(ext, ".svg")) return "image/svg+xml";
    if (iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}

#endif //SYSYPLUS_COMPILER_WEB_HPP
