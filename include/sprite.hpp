#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

class Sprite {
public:
  uint16_t *pixels;
  uint8_t width, height;
  int8_t off_x, off_y;

  Sprite(uint16_t *pixels, uint8_t width, uint8_t height);
  Sprite(uint16_t *pixels, uint8_t width, uint8_t height, int8_t offx,
         int8_t offy);
  void draw(int x, int y, float scale);
};

extern Sprite toad_sprite;
extern Sprite toad_left_sprite;
extern Sprite toad_right_sprite;
extern Sprite toad_east_sprite;
extern Sprite toad_west_sprite;
extern Sprite toad_north_sprite;

extern Sprite bowser_sprite;
extern Sprite bowser_left_sprite;
extern Sprite bowser_right_sprite;
extern Sprite bowser_east_sprite;
extern Sprite bowser_west_sprite;
extern Sprite bowser_north_sprite;

extern Sprite green_shell_sprite;
extern Sprite lap_sprite;
extern Sprite lap_one_sprite;
extern Sprite lap_two_sprite;
extern Sprite lap_three_sprite;

extern Sprite first_place_sprite;
extern Sprite second_place_sprite;

extern Sprite bullet_bill_sprite;
extern Sprite bullet_bill_left_sprite;
extern Sprite bullet_bill_right_sprite;
extern Sprite bullet_bill_east_sprite;
extern Sprite bullet_bill_west_sprite;
extern Sprite bullet_bill_north_sprite;

extern Sprite item_sprites[];

#endif
