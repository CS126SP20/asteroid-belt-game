//
// Created by Anil Dara on 4/21/20.
//

#ifndef FINALPROJECT_SHIP_H
#define FINALPROJECT_SHIP_H
#include <cinder/app/App.h>
#include "cinder/gl/gl.h"

using namespace ci;
namespace ship{

class Ship {
 private:
  vec2 location;
  int radius = 5;
 public:

  void update(vec2 dir);
  void draw();
  void SetLocation(vec2 loc) {
    location = loc;
  }
  vec2 GetLocation() {
    return location;
  }

};
}

#endif //FINALPROJECT_SHIP_H
