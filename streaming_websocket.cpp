#include "streaming_websocket.h"

websocket::stream<beast::ssl_stream<tcp::socket>>  streaming_websocket(net::io_context &ioc, const string &host, const string &port, const string &path)
{
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
    ws.handshake(host, path);
    return ws;
};
