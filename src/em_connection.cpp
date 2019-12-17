#include "connection.hpp"
#include "game.hpp"
#include "map.hpp"
#include <iostream>
#include <queue>

std::queue<char> receiving;
std::queue<char> sending;

extern "C" int sending_num();
extern "C" int get_sent();
extern "C" void add_receiving(int to_add);

int sending_num() { return sending.size(); }

int get_sent() {
  int r;
  r = (int)sending.front();
  sending.pop();
  return r;
}

void add_receiving(int to_add) { receiving.push((char)to_add); }

bool Connection::in_status() { return !receiving.empty(); }

void Connection::out_char(char data) { sending.push(data); }

char Connection::in_char() {
  char r;
  r = receiving.front();
  receiving.pop();
  return r;
}

void Connection::out_float(float out) {}

float Connection::in_float() { return 0; }
