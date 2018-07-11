#ifndef CONNECTION_H
#define CONNECTION_H

#include "stdint.h"

namespace Connection{

	void process();
	void handle();
	void send_message(char* message, uint8_t length);
  bool in_status();
  void out_char(char data);
  char in_char();
	void out_float(float out);
	float in_float();

};

#endif
