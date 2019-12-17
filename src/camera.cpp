#include "camera.hpp"

#include <math.h>

#include "colors.hpp"
#include "map.hpp"
#include "screen.hpp"
#include "sprite.hpp"
#include "trig.c"

#define CAMERA_DISTANCE 1.6f

Camera::Camera() {
  py = 100; // fov
  // horizon = HEIGHT/8;
  horizon = 0;
  xpos = 0;
  ypos = 0;
  angle = 0;
}

void Camera::lock_player(Player *player) {
  // horizon = (horizon + 1)%10;
  this->lock = player;
  this->xpos = player->xpos - CAMERA_DISTANCE * player->xdir;
  this->ypos = player->ypos - CAMERA_DISTANCE * player->ydir;
  this->angle = player->angle;
};

int hd = HEIGHT / 2;

void Camera::mode7(Map *map) {

  screen.set_box(0, hd - horizon, WIDTH, hd + horizon);

  float cosv = cosf(angle);
  float sinv = sinf(angle);

  const float xend = WIDTH / 2;

  const float yend = HEIGHT / 2 + horizon;

  int x = WIDTH - 1;
  for (float px = xend - 1; px >= -xend; px--) {
    int y = 0;
    for (float pz = 0; pz < yend; pz++) {
      float sx = px / pz;
      float sy = py / pz;

      float tsx = sx;

      sx = sx * cosv - sy * sinv + xpos;
      sy = -(tsx * sinv + sy * cosv) + ypos;

      uint16_t color = map->get_color(sx, sy);

      screen.pixels[x + (y)*WIDTH] = color;
      y++;
    }
    x--;
  }
  screen.finish_draw();
}

void Camera::screen_coord(float x, float y, int *rx, int *ry, float *rdist) {
  x = x - xpos;
  y = -(y - ypos);
  float tx = x;
  float cosv = cosf(-angle);
  float sinv = sinf(-angle);
  x = x * cosv - y * sinv;
  y = tx * sinv + y * cosv;

  float pz = py / y;
  *rdist = pz;
  float px = x * pz;

  *rx = ((int)px) + WIDTH / 2;
  *ry = ((int)(pz - horizon)) + HEIGHT / 2;
}
