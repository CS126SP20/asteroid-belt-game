//
// Created by Anil Dara on 5/5/20.
//

#include "Laser.h"

namespace laser {

void Laser::update(vec2 updated_top, vec2 updated_bottom) {
  top_point = updated_top;
  bottom_point = updated_bottom;
}

void Laser::draw() {
  gl::color(1, 0, 0);
  gl::drawLine(top_point, bottom_point);
}

}