#include "WebConnection.hpp"

WebConnection::WebConnection()
{
}
using boost::asio::ip::tcp;

WebConnection::~WebConnection()
{
    try
    {

        // Create the Boost.Asio io_context object
        boost::asio::io_context io_context;

        // Create a TCP resolver object to resolve the server address and port
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("localhost", "8080");

        // Create a TCP socket and attempt to connect to the server
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // Connection successful, do something here
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void WebConnection::Run()
{
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));
    while (true)
    {
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        std::array<char, 1024> buffer;
        boost::system::error_code error;
        size_t len = socket.read_some(boost::asio::buffer(buffer), error);
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.
    //    nlohmann::json j = nlohmann::json::parse(buffer.data(), buffer.data() + len);
        std::cout << "Received message: "  << std::endl;
    }
}