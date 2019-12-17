#include "screen.hpp"
#include "colors.hpp"
#include "input.hpp"
#include <iostream>

Screen screen;

uint32_t fix(uint32_t color) {
  uint32_t ret = 0;
  int b = color & 0x001F;
  int g = color & 0x07E0;
  int r = color & 0xF800;
  ret |= (r >> 11);
  ret |= g;
  ret |= (b << 11);
  return ret;
}

Screen::Screen() {
  box_w = WIDTH;
  box_h = HEIGHT;

  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    tex_pixels[i] = 0x0000;
  }
  /*
  for(int i = 0; i < WIDTH; i++){
    for(int j = 0; j < HEIGHT; j++){
      pixels[j*WIDTH + i] = ((uint16_t*)grassland_img)[j*200+i];
    }
  }
  */

  if (!glfwInit()) {
    error = -1;
    return;
  }

  window = glfwCreateWindow(WIDTH * SCALE, HEIGHT * SCALE, "EE319K Game", NULL,
                            NULL);
  if (!window) {
    glfwTerminate();
    error = -1;
    return;
  }

  glfwMakeContextCurrent(window);
  glClearColor(0, 0, 1, 1);

  glGenTextures(1, &tex);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0,  GL_RGB,
  // GL_UNSIGNED_SHORT_5_6_5, tex_pixels);
}

void Screen::text(char *text) { std::cout << text << std::endl; }

void Screen::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
               GL_UNSIGNED_SHORT_5_6_5, tex_pixels);

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex2f(-1, 1);
  glTexCoord2f(0, 1);
  glVertex2f(-1, -1);
  glTexCoord2f(1, 1);
  glVertex2f(1, -1);
  glTexCoord2f(1, 0);
  glVertex2f(1, 1);
  glPopMatrix();
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Screen::set_cursor(int x, int y) {}

void Screen::draw_box(uint16_t color, int x, int y, int w, int h) {
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      tex_pixels[x + i + (y + j) * WIDTH] = fix(color);
    }
  }
}

void Screen::set_box(int x, int y, int w, int h) {
  cursor_x = 0;
  cursor_y = 0;
  box_x = x;
  box_y = y;
  box_w = w;
  box_h = h;
}

void Screen::draw_pixel(const uint16_t &pixel) {
  int index = box_x + (box_w - cursor_x - 1) + WIDTH * (box_y + cursor_y);
  pixels[index] = pixel;
  cursor_y++;
  cursor_x = (cursor_x + (cursor_y / box_h)) % box_w;
  cursor_y = cursor_y % box_h;
}

void Screen::draw_top() {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT / 4; j++) {
      tex_pixels[i + j * WIDTH] = fix(pixels[i + j * WIDTH]);
    }
  }
}

void Screen::draw_bottom() {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT / 2; j++) {
      tex_pixels[i + (j + HEIGHT / 2) * WIDTH] = fix(pixels[i + j * WIDTH]);
    }
  }
}

void Screen::skip_pixel() {
  cursor_y++;
  cursor_x = (cursor_x + (cursor_y / box_h)) % box_w;
  cursor_y = cursor_y % box_h;
}

void Screen::finish_draw() {}

bool Screen::should_exit() { return glfwWindowShouldClose(window); }

Screen::~Screen() { glfwTerminate(); }
