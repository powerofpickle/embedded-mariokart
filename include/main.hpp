#ifndef MAIN_H
#define MAIN_H


#include <boost/asio.hpp>


using boost::asio::ip::tcp;

extern tcp::socket *connection_socket;
#endif
