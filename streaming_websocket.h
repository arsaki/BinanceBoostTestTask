#ifndef STREAMING_WEBSOCKET_H
#define STREAMING_WEBSOCKET_H
#include "root_certificates.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <string>

namespace beast = boost::beast;                    // from <boost/beast.hpp>
namespace http = beast::http;                         // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;   // from <boost/beast/websocket.hpp>
namespace net = boost::asio;                         // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;                   // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;                       // from <boost/asio/ip/tcp.hpp>
using namespace std;

websocket::stream<beast::ssl_stream<tcp::socket>>  streaming_websocket(net::io_context &ioc, const string &host, const string &port, const string &path);


#endif // STREAMING_WEBSOCKET_H
