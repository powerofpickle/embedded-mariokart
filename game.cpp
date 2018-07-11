#include "game.hpp"
#include "map.hpp"
#include "input.hpp"
#include "screen.hpp"
#include "colors.hpp"
#include "connection.hpp"

#ifndef linux
#include "../ST7735.h"
#endif

#if !defined linux && defined HOST
#include "../Music.h"
#define playMusic(x) songSel(x);
#else
#define playMusic(x)
#endif

#ifndef linux
#include "../ST7735.h"
#endif


Waiting waiting_state;

Menu main_menu;

//State* State::current_state = &waiting_state;
State* State::current_state = map;

Waiting::Waiting(){
	p2_ready = false;
	received_map = -1;
}

void Waiting::render(){
}

void Waiting::update(){
	screen.set_cursor(0,0);
#ifdef HOST
	screen.text("Waiting for remote...");

	if(p2_ready){
		main_menu.update_screen();
		current_state = &main_menu;
	}
#else
	screen.text("Waiting for host...");

	char c = 0x01c;
	Connection::send_message(&c, 1);
	if(received_map != -1){
		Map::set_map(received_map);
		current_state = map;
	}
#endif
}

Menu::Menu(){
	current_selection = 0;
}

void Menu::render(){

}



void Menu::update_screen(){
	screen.draw_box(0x0000, 0,0,WIDTH,HEIGHT);
	screen.set_cursor(0,0);
	switch(current_selection){
		case 0:
			screen.text("Mario Circuit");
			playMusic(FrappeSnowland);
			break;
		default:
			screen.text("Rainbow Road");
			playMusic(RainbowRoad);
			break;
	}
}

void Menu::update(){
	if(Input::item_button_action == RELEASE){
		current_selection = (current_selection + 1)%2;
		update_screen();
	}

	if(Input::acceleration > 0){
		Map::set_map(current_selection);
		current_state = map;
		char msg[2];
		msg[0] = 0x02c; // send map choice
		msg[1] = current_selection;
		Connection::send_message(msg, 2);
		Connection::send_message(msg, 2);
		Connection::send_message(msg, 2);
		Connection::send_message(msg, 2);
		Connection::send_message(msg, 2);
		Connection::send_message(msg, 2);
		Connection::send_message(msg, 2);
	}
}

#ifdef linux
#include <iostream>
void display_score(){
  return;
  std::cout << "Lap " << (int)map->player1.lap << " Node " << (int)map->player1.node_index << std::endl;
  std::cout << "Spacebar " << Input::item_button_action << std::endl;
}

#endif

void render(){
  State::current_state->render();
#ifdef EMSCRIPTEN
	screen.render();
#endif
}
int num_updates = 0;
void update(){
  Input::update();
	Connection::process();
  State::current_state->update();
	num_updates++;
}
