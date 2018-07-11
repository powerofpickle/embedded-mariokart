#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include "camera.hpp"
#include "game.hpp"

class Player;

#define MAP_SCALE 1


typedef struct {
  uint16_t *pixels;
  bool solid;
  bool slow;
	bool space;
	bool item;
} Tile;

extern Tile tile_arr[];


class Map;

extern Map mario_map;

extern Map* map;

#define MAX_SHELLS 6

class Map : public State{
public:
  const uint8_t *tiles;
	uint8_t bg_tile;
  uint8_t width, height;
	uint16_t sky_color;
  Camera camera;

	LocalPlayer player1;
	RemotePlayer player2;

  GreenShell shells[MAX_SHELLS];

  int8_t* progress_nodes;
  uint8_t num_nodes;

  static Map* current;
  static Map* current_map();

	static void set_map(int num);

  Map(const uint8_t* tiles, uint8_t width, uint8_t height, int8_t* progress_nodes, uint8_t num_nodes, uint16_t sky_color, uint8_t bg_tile, uint8_t p1_x, uint8_t p1_y, uint8_t p2_x, uint8_t p2_y);
	void update();
	void render();
  uint16_t get_color(float x, float y);
  Tile* tile_at(float x, float y);
  Tile* tile_at(int x, int y);
  ~Map();
};


#endif
