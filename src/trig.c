#include "math.h"

static const float pi = 3.14159265359;
static const float circumf = 2 * pi;

static float sint = 0;
static float sin_fast(float x) {

  if (x < -3.14159265f) {
    x += 6.28318531f;
  } else {
    if (x > 3.14159265f)
      x -= 6.28318531f;
  }
  if (x < 0)
    return x * (1.27323954f + 0.405284735f * x);
  else
    return x * (1.27323954f - 0.405284735f * x);
  /*
      x = x - (int)(x/M_PI/2);

      float x2 = x*x;
      float x3 = x*x2;
      float x5 = x3*x2;
      float x7 = x5*x2;
      return x - x3/6.0 + x5/120.0 - x7/5040.0;

      int index = (int)(x*10000.0);
      return sin_arr[index%62831];
  */
}

static float cos_fast(float x) {
  return (sin_fast(x + pi / 2));

  /*
  x = x - (int)(x/M_PI/2);
  float x2 = x*x;
  float x4 = x2*x2;
  float x6 = x4*x2;
  return 1 - x2/2.0 + x4/24.0 - x6/720.0;

  int index = (int)(x*10000.0);
  return cos_arr[index%62831];
  */
}
