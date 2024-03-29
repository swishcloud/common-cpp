#ifndef HTTP_H
#define HTTP_H
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/locale.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include "common.h"
namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace boost_http = beast::http; // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
namespace ssl = net::ssl;           // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
namespace http
{
    class value
    {
    private:
        const int vt{};
        int integer_v{};
        char *str_v{};

    public:
        value(const value &a);
        value(int v);
        value(const char *v);
        ~value();

    public:
        const char *str();
    };

    class UrlValues
    {
    private:
    public:
        std::string str;
        void add(std::string name, value value);
    };
    struct data_block
    {
        const char *name;
        value v;
        data_block(const char *name, value value);
    };
    class http_client
    {
    public:
        http_client();
        bool post(const char *host, const char *port, const char *target, std::vector<http::data_block> data, const char *token, std::string &resp_text);
    };
}
namespace common
{
    class http_client
    {

    public:
        http_client(std::string host, std::string port, std::string target, std::string token);
        http_client(std::string host, int port);
        ~http_client() noexcept(false);

        void GET();
        void GET(std::string target, std::string parameters, std::string token);
        void POST(std::vector<http::data_block> data);
        void POST(std::string target, std::string parameters, std::string token);
        void PUT(std::string target, std::string parameters, std::string token);
        std::string resp_text;
        common::error error;

    private:
        std::string host;
        std::string port;
        std::string target;
        std::string token;
        std::unique_ptr<beast::ssl_stream<beast::tcp_stream>> stream;
        boost_http::request<boost_http::string_body> req;
        beast::flat_buffer buffer;
        boost_http::response<boost_http::dynamic_body> res;
        net::io_context ioc;

        void connect();
        void resolve();
        void handshake();

        void send_request();

        void receive_response(std::size_t length);
    };
} // namespace common
#endif