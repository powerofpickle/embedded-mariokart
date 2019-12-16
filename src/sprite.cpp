#include "screen.hpp"
#include "sprite.hpp"
#include "shell.c"
#include "sprites/toad.c"
#include "sprites/bowser.c"
#include "sprites/green_shell.c"
#include "sprites/extra.c"
#include "sprites/item.c"
#include "sprites/bullet_bill.c"



Sprite::Sprite(uint16_t* pixels, uint8_t width, uint8_t height){
  this->pixels = pixels;
  this->width = width;
  this->height = height;
	this->off_x = -width/2;
	this->off_y = -height;
};

Sprite::Sprite(uint16_t* pixels, uint8_t width, uint8_t height, int8_t offx, int8_t offy){
  this->pixels = pixels;
  this->width = width;
  this->height = height;
	this->off_x = offx;
	this->off_y = offy;
}

void Sprite::draw(int x, int y, float scale){
	if(scale <= 0)
		return;

  float scaled_w = width*scale;
  float scaled_h = height*scale;

	int ox = scale*(float)off_x;
	int oy = scale*(float)off_y;

  if(!((x + ox) >= 0 && (x + ox + scaled_w) < WIDTH && (y + oy + scaled_h) < HEIGHT/2 && (y + oy) >= 0))
    return;

  int ww = scaled_w;
  if(ww < 0)
    return;
  int hh = scaled_h;


  screen.set_box(x + ox, y + oy, ww, hh);
  //for(int x = 0; x < ww; x++){


	int xxx = x + ox + ww - 1;
  for(int x = ww - 1; x >= 0; x--){

		int yyy = y + oy;

    int scaled_x = (int)((float)x/scale);
    for(int y = 0; y < hh; y++){
      int scaled_y = (int)((float)y/scale);
      uint16_t color = pixels[scaled_x+(height-scaled_y-1)*width];
			if(color != 0)
				screen.pixels[xxx+yyy*WIDTH] = color;
			yyy++;
    }
		xxx--;
  }
  screen.finish_draw();
}
/*
void Sprite::draw(int x, int y, float scale){

  float scaled_w = width*scale;
  float scaled_h = height*scale;

	int ox = scale*(float)off_x;
	int oy = scale*(float)off_y;

  if(!((x + ox) >= 0 && (x + ox + scaled_w) < WIDTH && (y + oy + scaled_w) < HEIGHT/2 && (y + oy) >= 0))
    return;

  int ww = scaled_w;
  if(ww < 0)
    return;
  int hh = scaled_h;


  screen.set_box(x + ox, y + oy, ww, hh);
  //for(int x = 0; x < ww; x++){

	int xxx = x + ox + ww - 1;
  for(int x = ww - 1; x >= 0; x--){

		int yyy = y + oy;

    int scaled_x = (int)((float)x/scale);
    for(int y = 0; y < hh; y++){
      int scaled_y = (int)((float)y/scale);
      uint16_t color = pixels[scaled_x+(height-scaled_y-1)*width];
			if(color != 0)
				screen.pixels[xxx+yyy*WIDTH] = color;
			/*
      if(color != 0)
        screen.draw_pixel(color);
      else
        screen.skip_pixel();
			*/
      /*
			yyy++;
    }
		xxx--;
  }
  screen.finish_draw();
}
*/
