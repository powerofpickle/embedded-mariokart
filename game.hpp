#ifndef GAME_H
#define GAME_H

#include "stdint.h"

#if 1
#define REMOTE 1
#else
#define HOST 1
#endif



class State{
public:
	static State* current_state;
	virtual void update(){};
	virtual void render(){};

};

class Waiting : public State{
	public:
		bool p2_ready;
		int8_t received_map;
		Waiting();
		void update();
		void render();

};
extern Waiting waiting_state;

class Menu : public State{
public:
	int current_selection;
	Menu();
	void update_screen();
	void update();
	void render();
};

extern State* current_state;
extern int num_updates;

extern "C" void render();
extern "C" void update();


#endif
