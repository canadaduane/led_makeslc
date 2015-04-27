#include <Arduino.h>
#include "SinWave.h"

class MixWave {
public:
  SinWave r, g, b;
  float ri, gi, bi;

  void init() {
    this->r.init(-20, 0, 0xFF0000, 4.5);
    this->g.init(15, 2, 0x00FF00, 3.0);
    this->b.init(8, 15, 0x0000FF, 3.8);
    
    this->ri = 0.0;
    this->gi = 0.0;
    this->bi = 0.0;
  }
  
  void set(int img[], int x, int y, int c) {
    img[ y * 15 + x ] = c;
  }
  
  void draw(int img[]) {
    for (int x = 0; x < 15; x++) {
      for (int y = 0; y < 10; y++) {
        int r1 = this->r.r_at(x, y), g1 = this->r.g_at(x, y), b1 = this->r.b_at(x, y);
        int r2 = this->g.r_at(x, y), g2 = this->g.g_at(x, y), b2 = this->g.b_at(x, y);
        int r3 = this->b.r_at(x, y), g3 = this->b.g_at(x, y), b3 = this->b.b_at(x, y);
        int color = (((r1 + r2 + r3) / 20) << 16) +
                    (((g1 + g2 + g3) / 20) << 8) +
                    (((b1 + b2 + b3) / 20) << 0);
        this->set(img, x, y, color);
      }
    }
  }
  
  void step() {
    this->r.newpos(this->ri, 30);
    this->g.newpos(this->gi, 30);
    this->b.newpos(this->bi, 25);

    this->ri += 0.05;
    this->ri += 0.05;
    this->gi += 0.03;
    this->gi += 0.03;
    this->bi += 0.06;
    this->bi += 0.06;
  }
};
