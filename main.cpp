#include <iostream>
#include <cstdbool>
#include <cstdint>
#include <cstdlib>
#include <thread>
#include "map.hpp"
#include "colors.hpp"
#include "screen.hpp"
#include "sprite.hpp"
#include "camera.hpp"
#include "game.hpp"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#define FPS 30
#define UPS 30

int clocks_per_frame = CLOCKS_PER_SEC/FPS;
int clocks_per_update = CLOCKS_PER_SEC/UPS;


boost::asio::io_context io_context;
tcp::socket *connection_socket;

char* ip = NULL;

void host_connect(){
  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 25565));
  acceptor.accept(*connection_socket);
  std::cout << "connection!" << std::endl;
}

void remote_connect(){
  if(ip != NULL)
    std::cout << ip << std::endl;
  tcp::resolver resolver(io_context);
  tcp::resolver::results_type endpoints = resolver.resolve(ip == 0 ? "localhost" : ip, "25565");
  boost::asio::connect(*connection_socket, endpoints);
}

void game_loop(){

	/*
	Map *map = Map::current_map();
	Camera& camera = map->camera;
	while(!screen.should_exit()){
		camera.render(*map);
	}
	*/

  tcp::socket socket_local = tcp::socket(io_context);
  connection_socket = &socket_local;
#ifdef HOST
  host_connect();
#else
  remote_connect();
#endif

  screen.draw_box(0x0000, 0,0,WIDTH,HEIGHT);

  clock_t last_frame = clock() + clocks_per_frame;
  clock_t last_update = clock() + clocks_per_update;

  clock_t now;

  while(!screen.should_exit()){
    now = clock();
    if(now > last_frame){
      render();
      last_frame += clocks_per_frame;
    }
    while(now > last_update){
      update();
      last_update += clocks_per_update;
    }
  }

}

int main(int argc, char**argv){

  if(argc > 1)
    ip = argv[1];


  std::thread game_thread(game_loop);

  while(!screen.should_exit()){
    screen.render();
  }


  game_thread.join();


  return 0;
}
