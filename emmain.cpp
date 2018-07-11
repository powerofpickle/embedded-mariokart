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
#include "emscripten.h"

#define FPS 30
#define UPS 30

int clocks_per_frame = CLOCKS_PER_SEC/FPS;
int clocks_per_update = CLOCKS_PER_SEC/UPS;

clock_t last_frame;
clock_t last_update;
clock_t now;

void game_loop(){

  now = clock();
  if(now > last_frame){
    render();
    last_frame += clocks_per_frame;
  }
  while(now > last_update){
    update();
    last_update += clocks_per_update;
  }

    screen.render();

}

int main(int argc, char**argv){


  std::cout << "hi" << std::endl;

  screen.draw_box(0x0000, 0,0,WIDTH,HEIGHT);

  last_frame = clock() + clocks_per_frame;
  last_update = clock() + clocks_per_update;
  emscripten_set_main_loop(game_loop, 30, 1);


  return 0;
}
