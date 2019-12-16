#include "input.hpp"
#include "screen.hpp"
#include "game.hpp"

#include <GLFW/glfw3.h>



float Input::steering_angle = 0;
float Input::acceleration = 0;

int Input::item_button_action = 0;
bool Input::item_button = false;

#if defined EMSCRIPTEN && defined REMOTE
#define FORWARD_B GLFW_KEY_I
#define LEFT_B GLFW_KEY_J
#define RIGHT_B GLFW_KEY_L
#define ITEM_B GLFW_KEY_COMMA
#else
#define FORWARD_B GLFW_KEY_W
#define LEFT_B GLFW_KEY_A
#define RIGHT_B GLFW_KEY_D
#define ITEM_B GLFW_KEY_SPACE
#endif

void Input::update(){

  Input::acceleration = (glfwGetKey(screen.window, FORWARD_B) == GLFW_PRESS) ? 0.005f : -0.005f;



  float left_steer = (glfwGetKey(screen.window, LEFT_B) == GLFW_PRESS) ? 0.25f : 0;
  float right_steer = (glfwGetKey(screen.window, RIGHT_B) == GLFW_PRESS) ? -0.25f : 0;

  Input::steering_angle = left_steer + right_steer;

  bool item_button_next = (glfwGetKey(screen.window, ITEM_B) == GLFW_PRESS);

  Input::item_button_action = Input::item_button*2 + item_button_next;
  Input::item_button = item_button_next;

}
