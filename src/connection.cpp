#include "connection.hpp"
#include "game.hpp"
#include "main.hpp"
#include "map.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

bool Connection::in_status() {
  boost::asio::socket_base::bytes_readable command(true);
  connection_socket->io_control(command);
  std::size_t bytes_readable = command.get();
  return bytes_readable > 0;
}

void Connection::out_char(char data) {
  boost::system::error_code ignored_error;
  boost::asio::write(*connection_socket, boost::asio::buffer(&data, 1),
                     boost::asio::transfer_all(), ignored_error);
}

char Connection::in_char() {
  char buffer[1];
  size_t bytesRead =
      boost::asio::read(*connection_socket, boost::asio::buffer(buffer, 1));
  return buffer[0];
}

void Connection::out_float(float out) {}

float Connection::in_float() { return 0; }
