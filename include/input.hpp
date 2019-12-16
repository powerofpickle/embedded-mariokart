#ifndef INPUT_H
#define INPUT_H


#define OFF 0
#define PRESS 1
#define RELEASE 2
#define HOLD 3

class Input{
public:

  static float steering_angle;
  static float acceleration;

  static int item_button_action;
  static bool item_button;


  static void update();

};

#endif
