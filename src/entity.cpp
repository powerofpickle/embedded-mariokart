#include "entity.hpp"
#include "screen.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "input.hpp"
#include "trig.c"
#include "connection.hpp"
#include "game.hpp"

#ifdef linux
#include <iostream>
#endif

#define TOP_SPEED 0.3f
#define SHELL_SPEED 0.4f

float dist_mod = HEIGHT/4;


Sprite* shell_sprites[] = {
	&green_shell_sprite
};

void Entity::draw(){
	if(!initialized)
		return;
	int x;
	int y;
	float dist;


		map->camera.screen_coord(xpos, ypos, &x, &y, &dist);


	sprite->draw(x,y - HEIGHT/2, dist/HEIGHT*3);
}


GreenShell::GreenShell(){
	initialized = false;
}

GreenShell::GreenShell(Player* player, uint8_t index){
	this->index = index;
	xdir = player->xdir;
	ydir = player->ydir;
	xpos = player->xpos + xdir;
	ypos = player->ypos + ydir;
	speed = SHELL_SPEED;
	hits = 0;

	sprite = &green_shell_sprite;
	initialized = true;

	if(player == &(map->player1)){
		char msg[3];
		msg[0] = 0x04c;
		msg[1] = index;
		msg[2] = 0; //spawn
		Connection::send_message(msg, 3);
	}
	//send_update();
}

void GreenShell::send_update(){
	float msg_f[3];
	char* msg = (char*)msg_f;
	msg[0] = 0x04c;
	msg[1] = 0;
	msg[2] = index;
	if(initialized){
		msg[3] = 0;
		msg_f[1] = xpos;
		msg_f[2] = ypos;
	}else{
		msg[3] = 1;
	}
	Connection::send_message(msg, 12);
}

void GreenShell::update(){
	if(!initialized)
		return;


	int txp = (int)xpos;
	int typ = (int)ypos;


	xpos += xdir*speed;
	ypos += ydir*speed;

	int tx = (int)xpos;
	int ty = (int)ypos;


	Tile* t = map->tile_at(tx, ty);
	if(t->solid){
		hits++;
		if(hits == 3){
			initialized = false;
		}

		if(txp != tx && map->tile_at(txp, ty)->solid == false){
			xdir *= -1;
			xpos += xdir*speed;
		}
		if(typ != ty && map->tile_at(tx, typ)->solid == false){
			ydir *= -1;
			ypos += ydir*speed;
		}
	}else if(t->space){
		initialized = false;
	}

	float dx = xpos-map->player1.xpos;
	float dy = ypos-map->player1.ypos;
	if((dx*dx + dy*dy) < 1.0f){
		map->player1.speed = 0;
		map->player1.stun = 30;
		initialized = false;
	}


	if(!initialized){
		char msg[3];
		msg[0] = 0x04c;
		msg[1] = index;
		msg[2] = 1; //despawn
		Connection::send_message(msg, 3);
	}
	//send_update();
}


PlayerSprite toad_sprites = {&toad_north_sprite, &toad_east_sprite, &toad_sprite, &toad_west_sprite, &toad_left_sprite, &toad_right_sprite};
PlayerSprite bowser_sprites = {&bowser_north_sprite, &bowser_east_sprite, &bowser_sprite, &bowser_west_sprite, &bowser_left_sprite, &bowser_right_sprite};
PlayerSprite bullet_bill_sprites = {&bullet_bill_north_sprite, &bullet_bill_east_sprite, &bullet_bill_sprite, &bullet_bill_west_sprite, &bullet_bill_left_sprite, &bullet_bill_right_sprite};

RemotePlayer::RemotePlayer(uint8_t x, uint8_t y){
	place = 1;
	initialized = true;
	xpos = x;
	ypos = y;
	node_index = 0;
	lap = 1;
#ifdef HOST
	sprites = &toad_sprites;
#else
	sprites = &bowser_sprites;
#endif
	set_angle(0);

}

void RemotePlayer::draw(){
	int x;
	int y;
	float dist;


	map->camera.screen_coord(xpos, ypos, &x, &y, &dist);

	Sprite* s;
	s = sprites->south;

	s->draw(x,y - HEIGHT/2, dist/HEIGHT*3);
}

LocalPlayer::LocalPlayer(uint8_t x, uint8_t y){
	boost = 0;
	initialized = true;
	place = 1;
	xpos = x;
	ypos = y;
	speed = 0;
	acceleration = 0;
	lap = 1;
	node_index = 0;
	stun = 0;
	item = -1;
	bullet_bill = 0;

#ifdef HOST
	sprites = &bowser_sprites;
#else
	sprites = &toad_sprites;
#endif

	set_angle(0);

}

void Player::set_angle(float angle){

	while(angle < -pi)
		angle += circumf;
	while(angle > pi)
		angle -= circumf;

	this->angle = angle;
	this->xdir = -sin_fast(angle);
	this->ydir = -cos_fast(angle);
}

int Player::coord_dist(int8_t* node){
	int dx = xpos - node[0];
	int dy = ypos - node[1];
	return dx*dx + dy*dy;
}

void Player::update_progress(){
	int next_i = (node_index + 1)%map->num_nodes;
	int prev_i = node_index == 0 ? map->num_nodes - 1 : node_index - 1;

	int dist = coord_dist(map->progress_nodes + node_index*2);
	int next_dist = coord_dist(map->progress_nodes + next_i*2);
	int prev_dist = coord_dist(map->progress_nodes + prev_i*2);
	//std::cout << prev_dist << " " << dist << " " << next_dist << std::endl;

	if(next_dist < dist){
		if(next_i == 0){ //passing finish line forwards
			lap++;
		}
		node_index = next_i;
	}else if(prev_dist < dist){
		if(node_index == 0){ //passing finish line backwards
			lap--;
		}
		node_index = prev_i;
	}
}

void LocalPlayer::update(){
	set_angle(angle + Input::steering_angle*(speed+0.05f));

	speed += Input::acceleration;
	if(speed > TOP_SPEED)
		speed = TOP_SPEED;
	if(speed < 0)
		speed = 0;
	
	if(boost != 0){
		speed = TOP_SPEED*2;
		boost--;
	}

	if(stun > 0){
		speed = 0;
		stun--;
	}

	if(bullet_bill != 0){

		int next_i = (node_index + 1)%map->num_nodes;
		int8_t* next_node = (map->progress_nodes + next_i*2);

		float dx = (float)(next_node[0]) - xpos;
		float dy = (float)(next_node[1]) - ypos;

		//xpos += ((float)(next_node[0]) - xpos)*0.05f;
		//ypos += ((float)(next_node[1]) - ypos)*0.05f;
		float an = atanf(dx/dy);
		if(dy > 0)
			an += pi;
		set_angle(an);
		xpos += xdir*TOP_SPEED*2.0f;
		ypos += ydir*TOP_SPEED*2.0f;
		bullet_bill--;
	}else{



		int txp = (int)xpos;
		int typ = (int)ypos;

	  xpos = (xpos + speed*xdir);
	  ypos = (ypos + speed*ydir);


		int tx = (int)xpos;
		int ty = (int)ypos;


		Tile* t = map->tile_at(tx, ty);
		if(t->slow && speed > TOP_SPEED*0.5f){
			speed = TOP_SPEED*0.5f;
		}


		if(t->solid){
			if(tx != txp)
				xpos -= (speed*xdir);
			if(ty != typ)
				ypos -= (speed*ydir);
			speed *= 0.5f;
		}else if(t->space){
			int8_t* node = (map->progress_nodes + node_index*2);
			this->xpos = node[0];
			this->ypos = node[1];
			this->stun = 40;
		}else if(t->item && (txp != tx || typ != ty) && this->item == -1){
			this->item = (num_updates%3);
		}

		if(Input::item_button_action == RELEASE){
			use_item();
		}
	}

	update_progress();
	map->player2.update_progress();


	if(lap != map->player2.lap){
		place = (lap > map->player2.lap) ? 1 : 2;
	}else if(node_index != map->player2.node_index){
		place = (node_index > map->player2.node_index) ? 1 : 2;
	}else{
		int8_t* next_node = map->progress_nodes + ((node_index+1)%map->num_nodes)*2;
		int hd = coord_dist(next_node);
		int rd = map->player2.coord_dist(next_node);
		place = (hd < rd) ? 1 : 2;
	}



	float msg[4];
	((char*)msg)[0] = 0x03c;
	msg[1] = xpos;
	msg[2] = ypos;
	msg[3] = angle;

	if(num_updates%6==0)
		Connection::send_message((char*)msg, 16);
}

void LocalPlayer::use_item(){
	switch(this->item){
		case -1:
			break;
		case 0:
			for(int i = 0; i < MAX_SHELLS; i++){
				if(map->shells[i].initialized == false){
					map->shells[i] = GreenShell(this, i);
					break;
				}
			}
			break;
		case 1:
			bullet_bill = 150;
			break;
		case 2:
			boost = 90;
		default:
			break;
	}
	this->item = -1;

}

void LocalPlayer::draw(){
	int x;
	int y;
	float dist;


	map->camera.screen_coord(xpos, ypos, &x, &y, &dist);
	PlayerSprite* sprites = (bullet_bill == 0) ? this->sprites : &bullet_bill_sprites;
	Sprite* s;
	s = Input::steering_angle > 0.01 ? sprites->left : (Input::steering_angle < -0.01 ? sprites->right : sprites->south);

	s->draw(x,y - HEIGHT/2, dist/HEIGHT*3);
}
