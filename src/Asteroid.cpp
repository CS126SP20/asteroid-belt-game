//
// Created by Anil Dara on 4/21/20.
//

#include "Asteroid.h"
namespace asteroid {
void Asteroid::update(vec2 dir, float speed) {
  location = dir;
}
void Asteroid::draw() {
  gl::color(0, 1, 0);
  gl::drawSolidCircle(location, radius);
}
}
