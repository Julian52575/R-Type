#include <array>
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>

using asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
  try
  {
    asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), 4444));

    for (;;)
    {
      std::array<char, 1> recv_buf;
      udp::endpoint remote_endpoint;
      socket.receive_from(asio::buffer(recv_buf), remote_endpoint);

      std::string message = make_daytime_string();
  #ifdef DEBUG
      std::cout << "Received message: " << recv_buf.data() << " from " << remote_endpoint << std::endl;
  #endif
      std::error_code ignored_error;
      socket.send_to(asio::buffer(message),
          remote_endpoint, 0, ignored_error);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}