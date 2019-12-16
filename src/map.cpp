#include "stdlib.h"
#include "math.h"

#include "map.hpp"
#include "colors.hpp"
#include "game.hpp"
#include "sprite.hpp"

#include "tile_arr.c"

#define TILE_WIDTH 8

#ifdef linux
#include <iostream>
#endif



int8_t map1_nodes[] = {
	8,31,
	11,18,
	25,9,
	39,17,
	42,36,
	59,45,
	77,36,
	86,17,
	108,13,
	116,28,
	115,51,
	95,61,
	78,64,
	52,68,
	53,80,
	73,80,
	97,78,
	116,92,
	114,106,
	95,117,
	68,111,
	49,101,
	34,104,
	21,112,
	14,111,
	8,100,
	7,63,
	8,33
};

int8_t map2_nodes[] = {
	5,38,
	6,16,
	22,6,
	43,6,
	84,6,
	101,7,
	107,20,
	106,33,
	105,41,
	99,44,
	94,41,
	85,31,
	70,29,
	57,29,
	42,30,
	35,47,
	41,57,
	63,57,
	87,60,
	88,82,
	86,105,
	73,114,
	60,119,
	35, 120,
	10,113,
	5,100,
	5,70,
	5,39
};

Map map1(map1_tiles, 128, 128, map1_nodes, 28, SKY_BLUE, 1, 6, 35, 10, 35);
Map map2(map2_tiles, 128, 128, map2_nodes, 28, 0x0000, 10, 6, 35, 10, 35);

Map* map = &map2;

Map* Map::current = 0;

void Map::set_map(int num){
	switch(num){
		case 0:
			map = &map1;
			break;
		case 1:
			map = &map2;
			break;
		default:
			map = &map2;
			break;
	}
	screen.draw_box(map->sky_color, 0, 0, WIDTH, HEIGHT);
}


Map::Map(const uint8_t* tiles, uint8_t width, uint8_t height, int8_t* progress_nodes, uint8_t num_nodes, uint16_t sky_color, uint8_t bg_tile, uint8_t p1_x, uint8_t p1_y, uint8_t p2_x, uint8_t p2_y)
#ifdef HOST
: player1(p1_x, p1_y), player2(p2_x, p2_y)
#else
: player1(p2_x, p2_y), player2(p1_x, p1_y)
#endif
{

	this->tiles = tiles;
	this->bg_tile = bg_tile;
	this->width = width;
	this->height = height;
	this->progress_nodes = progress_nodes;
	this->num_nodes = num_nodes;
	this->sky_color = sky_color;



}

void Map::update(){
	player1.update();
	this->camera.lock_player(&(this->player1));


	for(int i = 0; i < MAX_SHELLS; i++){
		shells[i].update();
	}
}

void Map::render(){
	camera.mode7(this);

	for(int i = 0; i < MAX_SHELLS; i++){
		shells[i].draw();
	}
	player2.draw();
	player1.draw();


	screen.draw_bottom();

	for(int i = 0; i < WIDTH*HEIGHT/4; i++){
		screen.pixels[i] = map->sky_color;
	}

	lap_sprite.draw(WIDTH/2,2,1.0f);

	switch(player1.lap){
		case 3:
		lap_three_sprite.draw(WIDTH/2,2,1.0f);
			break;
		case 2:
			lap_two_sprite.draw(WIDTH/2,2,1.0f);
			break;
		default:
			lap_one_sprite.draw(WIDTH/2,2,1.0f);
			break;
	};

	switch(player1.place){
		case 1:
			first_place_sprite.draw(WIDTH-2, 2, 1.0f);
			break;
		case 2:
			second_place_sprite.draw(WIDTH-2, 2, 1.0f);
			break;
		default:
			break;
	}



	for(int i = 2; i < 27; i++){
		for(int j = 2; j < 27; j++){
			screen.pixels[i+j*WIDTH] = 0x0000;
		}
	}

	if(player1.item != -1){
		item_sprites[player1.item].draw(14,14,1.0f);
	}


	screen.draw_top();


}


uint16_t Map::get_color(float x, float y){
  x*= MAP_SCALE;
  y *= MAP_SCALE;


	int tile_index;
  if(!(x >= 0 && x < width && y >= 0 && y < height)){
    tile_index = bg_tile;
  }else{
		tile_index = tiles[((int)y)*width + ((int)x)];
	}

	//y = y - (int)y;
	int yy = (int)(y*8.0f) % 8;

	if(yy < 0)
		yy = 8 + yy;

	//x = x - (int)x;
	int xx = (int)(x*8.0f) % 8;
	if(xx < 0)
		xx = 8 + xx;
	int pixel_index = (yy)*TILE_WIDTH + (xx);
	return tile_arr[tile_index].pixels[pixel_index];


}

Tile* Map::tile_at(float x, float y){
	x*= MAP_SCALE;
  y *= MAP_SCALE;


	int tile_index;
  if(!(x >= 0 && x < width && y >= 0 && y < height)){
    tile_index = bg_tile;
  }else{
		tile_index = this->tiles[((int)y)*width + ((int)x)];
	}

	return tile_arr + tile_index;
}

Tile* Map::tile_at(int x, int y){
	x*= MAP_SCALE;
  y *= MAP_SCALE;

	int tile_index;
  if(!(x >= 0 && x < width && y >= 0 && y < height)){
    tile_index = bg_tile;
  }else{
		tile_index = this->tiles[(y)*width + (x)];
	}

	return tile_arr + tile_index;
}

Map* Map::current_map(){
  return Map::current;
}

Map::~Map(){

}
