
#ifndef ENTITY_H
#define ENTITY_H

#include "sprite.hpp"
#include "stdint.h"

class Entity {
public:
  float xpos;
  float ypos;
  bool initialized;

  Sprite *sprite;

  void update();
  void draw();
};

class Player;

class GreenShell : public Entity {
public:
  float xdir;
  float ydir;
  float speed;
  uint8_t index;

  uint8_t hits;
  void send_update();
  GreenShell();
  GreenShell(Player *player, uint8_t index);
  void update();
};

typedef struct {
  Sprite *north;
  Sprite *east;
  Sprite *south;
  Sprite *west;
  Sprite *left;
  Sprite *right;
} PlayerSprite;

class Player : public Entity {
public:
  float xdir;
  float ydir;
  float speed;
  float angle;
  uint16_t bullet_bill;

  PlayerSprite *sprites;

  int8_t lap;
  int8_t node_index;
  uint8_t place;

  int coord_dist(int8_t *node);
  void update_progress();
  void set_angle(float angle);
};

class RemotePlayer : public Player {
public:
  RemotePlayer(uint8_t x, uint8_t y);
  void draw();
};

class LocalPlayer : public Player {
public:
  int8_t stun;
  int8_t item;
  float acceleration;
  uint16_t boost;

  LocalPlayer(uint8_t x, uint8_t y);

  void render();
  void update();
  void use_item();
  void draw();
};

#endif
