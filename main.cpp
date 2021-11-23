#include "root_certificates.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>


namespace beast = boost::beast;                    // from <boost/beast.hpp>
namespace http = beast::http;                         // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;   // from <boost/beast/websocket.hpp>
namespace net = boost::asio;                         // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;                   // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;                       // from <boost/asio/ip/tcp.hpp>
using namespace std;

int main()
{
           cout<<"Starting" << endl;
           //"wss://stream.binance.com:9443"
           std::string host = "stream.binance.com";
           auto const  port = "9443";
           auto const  text = "some text";

           // The io_context is required for all I/O
           net::io_context ioc;

           // The SSL context is required, and holds certificates
           ssl::context ctx{ssl::context::tlsv12_client};

           // This holds the root certificate used for verification
           load_root_certificates(ctx);

           // These objects perform our I/O
           tcp::resolver resolver{ioc};
           websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

           // Look up the domain name
          cout << "resolver.resolve"<< endl;
           auto const results = resolver.resolve(host, port);

           // Make the connection on the IP address we get from a lookup
           auto ep = net::connect(get_lowest_layer(ws), results);

           // Set SNI Hostname (many hosts need this to handshake successfully)
           if(! SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str()))
               throw beast::system_error(
                   beast::error_code(
                       static_cast<int>(::ERR_get_error()),
                       net::error::get_ssl_category()),
                   "Failed to set SNI Hostname");

           // Update the host_ string. This will provide the value of the
           // Host HTTP header during the WebSocket handshake.
           // See https://tools.ietf.org/html/rfc7230#section-5.4
           host += ':' + std::to_string(ep.port());

           // Perform the SSL handshake
           cout<<"SSL handshake"<< endl;
           ws.next_layer().handshake(ssl::stream_base::client);

           // Set a decorator to change the User-Agent of the handshake
           cout<<"ws.set_option"<< endl;
           ws.set_option(websocket::stream_base::decorator(
               [](websocket::request_type& req)
               {
                   req.set(http::field::user_agent,
                       std::string(BOOST_BEAST_VERSION_STRING) +
                           " websocket-client-coro");
               }));

           // Perform the websocket handshake
           cout<<"ws.handshake"<< endl;
           ws.handshake(host, "/ws/btcusdt@aggTrade");

           // Send the message
//           ws.write(net::buffer(std::string(text)));

           // This buffer will hold the incoming message
           beast::flat_buffer buffer;

           // Read a message into our buffer
           cout<<"ws.read"<< endl;
           ws.read(buffer);

           std::cout << beast::make_printable(buffer.data()) << std::endl;
           cout<<"ws.close"<< endl;
           ws.close(websocket::close_code::normal);

           // If we get here then the connection is closed gracefully

           // The make_printable() function helps print a ConstBufferSequence
           return 0;
}
