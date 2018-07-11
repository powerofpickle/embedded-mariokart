#ifndef SCREEN_H
#define SCREEN_H

#define PRESCALE 1
#define WIDTH (160*PRESCALE)
#define HEIGHT (128*PRESCALE)
#define SCALE (4/PRESCALE)

#include <stdint.h>

#ifdef linux
#include <GLFW/glfw3.h>
#endif

class Screen;
extern Screen screen;

class Screen {

public:

#ifdef linux
  GLFWwindow* window;
  GLuint tex;
  int cursor_x = 0, cursor_y = 0;
  int box_x = 0, box_y = 0;
  int box_w = 0, box_h = 0;
  uint16_t tex_pixels[WIDTH*HEIGHT];
#endif

	uint16_t pixels[WIDTH*HEIGHT/2];

  int error;


  Screen();

	void text(char* text);

  void render();

	void set_cursor(int x, int y);

  void draw_box(uint16_t color, int x, int y, int w, int h);

  void set_box(int x, int y, int w, int h);

  void draw_pixel(const uint16_t &pixel);

	void draw_bottom();

	void draw_top();

  void skip_pixel();

  void finish_draw();

  bool should_exit();

  ~Screen();

};

#endif
