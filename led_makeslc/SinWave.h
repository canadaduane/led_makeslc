#include <Arduino.h>

class SinWave {
public:
  int ix, iy;
  int x, y, color;
  float divisor;
  
  void init(int x, int y, int color, float divisor) {
    this->ix = x;
    this->iy = y;
    this->x = x;
    this->y = y;
    this->color = color;
    this->divisor = divisor;
  }
  
  float dist(int nx, int ny) {
    return sqrt(
      (this->x - nx) * (this->x - nx) +
      (this->y - ny) * (this->y - ny)
    );
  }
  
  int color_at(int x, int y) {
    int r = (this->color & 0xFF0000) >> 16;
    int g = (this->color & 0x00FF00) >> 8;
    int b = (this->color & 0x0000FF) >> 0;
    float cr = (sin(this->dist(x, y)/this->divisor) + 1) * r / 2;
    float cg = (sin(this->dist(x, y)/this->divisor) + 1) * g / 2;
    float cb = (sin(this->dist(x, y)/this->divisor) + 1) * b / 2;
    return ((int)cr << 16) + ((int)cg << 8) + ((int)cb);
  }
  
  int r_at(int x, int y) {
    int r = (this->color & 0xFF0000) >> 16;
    return (sin(this->dist(x, y)/this->divisor) + 1) * r / 2;
  }

  int g_at(int x, int y) {
    int g = (this->color & 0x00FF00) >> 8;
    return (sin(this->dist(x, y)/this->divisor) + 1) * g / 2;
  }

  int b_at(int x, int y) {
    int b = (this->color & 0x0000FF) >> 0;
    return (sin(this->dist(x, y)/this->divisor) + 1) * b / 2;
  }
  
  void newpos(float f, float range) {
    this->x = this->ix + (int)(sin(f) * range);
    this->y = this->iy + (int)(sin(f) * range);
  }
};
