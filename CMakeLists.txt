cmake_minimum_required(VERSION 3.5)

project(BinanceBoostTestTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BinanceBoostTestTask main.cpp)

if (OPENSSL_FOUND)
    GroupSources(include/boost/beast beast)
    GroupSources(example/common common)
    GroupSources(example/websocket/client/sync-ssl "/")

    add_executable (websocket-client-sync-ssl
        ${BOOST_BEAST_FILES}
        ${PROJECT_SOURCE_DIR}/root_certificates.hpp
        Jamfile
        websocket_client_sync_ssl.cpp
    )

    set_property(TARGET websocket-client-sync-ssl PROPERTY FOLDER "example-websocket-client")

    target_link_libraries (websocket-client-sync-ssl
        OpenSSL::SSL OpenSSL::Crypto
        lib-asio
        lib-asio-ssl
        lib-beast
        )

endif()
