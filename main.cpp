#include "root_certificates.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/thread.hpp>
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


double bid_offer_spot_average = 0;
double bid_offer_perpetual_average = 0;
double fund_rate = 0;
beast::flat_buffer buffer;


void websocket_message_handler( beast::error_code const & ec, std::size_t bytes_transferred)
{
    if (ec)
            std::cout << "Error!!!!" + ec.message() << std::endl;
    else
            std::cout << beast::make_printable(buffer.data()) << std::endl;
    buffer.clear();

}


void mk_websocket()
{
           cout<<"Starting" << endl;
           //"wss://stream.binance.com:9443"
           std::string host = "stream.binance.com";
           auto const  port = "9443";
           auto const  text = "some text";
           net::io_context ioc;
           ssl::context ctx{ssl::context::tlsv12_client};
           load_root_certificates(ctx);
           tcp::resolver resolver{ioc};
           websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};
           auto const results = resolver.resolve(host, port);
           auto ep = net::connect(get_lowest_layer(ws), results);
           if(! SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str()))
               throw beast::system_error(
                   beast::error_code(
                       static_cast<int>(::ERR_get_error()),
                       net::error::get_ssl_category()),
                   "Failed to set SNI Hostname");
           host += ':' + std::to_string(ep.port());
           ws.next_layer().handshake(ssl::stream_base::client);
           ws.handshake(host, "/ws/btcusdt@bookTicker");
//           ws.write(net::buffer(std::string(text)));
//           beast::flat_buffer buffer;
//           ws.read(buffer);
//           std::cout << beast::make_printable(buffer.data()) << std::endl;
//           beast::error_code ec;
//           ws.close(boost::beast::websocket::close_code::service_restart, ec);
                       //error code = 1, fail = true, but, ws_.is_open() return false which means ws have closed successfully
//                       if (ec)
//                           if ( !ws.is_open() )
//                                std::cerr << ": " << ec.message() << endl;
           while (1)
           {
               ws.async_read( buffer,  websocket_message_handler);
               ioc.run();
               std::cout<<"brbrbr" << std::endl;
               ioc.restart();

           }
           std::cout<<"ioc.run finished" << std::endl;
           return;
}

void thread1job()
{
    while (1)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        std::cout<<bid_offer_spot_average<<std::endl;
    }
}

void thread2job()
{
    mk_websocket();
//    while (1)
//    {
////        bid_offer_spot_average = bid_offer_spot_average + 1;
////        boost::this_thread::sleep(boost::posix_time::seconds(1));
//    }
}


int main()
{
    boost::thread thread1(thread1job);
    boost::thread thread2(thread2job);
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);
    pthread_setaffinity_np(thread2.native_handle(), sizeof(cpu_set_t), &cpuset);
    thread1.join();
//    while(1)
//    {

//    }
}
