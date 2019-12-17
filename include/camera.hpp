#ifndef CAMERA_H
#define CAMERA_H

#include "entity.hpp"
#include "screen.hpp"

class Map;

#define CAMERA_DIST 1.0

class Camera {
public:
  float py; // fov
  int horizon;
  float xpos, ypos;
  float angle;
  Player *lock;

  Camera();
  void lock_player(Player *player);
  void mode7(Map *map);
  void screen_coord(float x, float y, int *rx, int *ry, float *rdist);
};

/*uint16_t interpolate(uint16_t color1, uint16_t color2, float interp){
        uint16_t r1 = (color1 & 0xF800) >> 11;
        uint16_t g1 = (color1 & 0x07E0) >> 5;
        uint16_t b1 = (color1 & 0x001F);
        uint16_t r2 = (color2 & 0xF800) >> 11;
        uint16_t g2 = (color2 & 0x07E0) >> 5;
        uint16_t b2 = (color2 & 0x001F);

        uint16_t r = r1*(1.0-interp) + r2*interp;
        uint16_t g = g1*(1.0-interp) + g2*interp;
        uint16_t b = b1*(1.0-interp) + b2*interp;

        return (r << 11) | (g << 5) | b;
}*/

#endif
