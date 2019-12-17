#include "connection.hpp"
#include "game.hpp"
#include "map.hpp"

char message_buf[50];
int buf_index = -1;
int message_len = -1;

void Connection::process() {
  while (in_status()) {
    if (message_len == -1) {
      if (buf_index == -1) {
        if (in_char() == 'S') {
          buf_index = 0;
        }
      } else {
        message_len = in_char();
      }
    } else {
      message_buf[buf_index] = in_char();
      buf_index++;
      if (buf_index == message_len) {
        handle();
        message_len = -1;
        buf_index = -1;
      }
    }
  }
}

void Connection::handle() {
  switch (message_buf[0]) {
  case 0x01c:
    // p2 to p1
    waiting_state.p2_ready = true;
    break;
  case 0x02c:
    // p1 to p2 map num
    waiting_state.received_map = message_buf[1];
    break;
  case 0x03c:
    // map->player2.xpos = message_buf[1];
    // map->player2.ypos = message_buf[2];
    map->player2.xpos = ((float *)message_buf)[1];
    map->player2.ypos = ((float *)message_buf)[2];
    map->player2.set_angle(((float *)message_buf)[3]);
    break;
  case 0x04c:
    // map->player2.xpos = message_buf[1];
    // map->player2.ypos = message_buf[2];
    GreenShell *shell = &(map->shells[message_buf[1]]);
    if (message_buf[2] == 0) {
      *shell = GreenShell(&(map->player2), message_buf[1]);
    } else if (message_buf[2] == 1) {
      shell->initialized = false;
    }
    break;
  }
}

void Connection::send_message(char *message, uint8_t length) {
  out_char('S');
  out_char(length);
  for (int i = 0; i < length; i++) {
    out_char(message[i]);
  }
  if (length < 6) {
    for (int i = 0; i < (6 - length); i++) {
      out_char(0);
    }
  }
}
