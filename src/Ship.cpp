//
// Created by Anil Dara on 4/21/20.
//

#include "Ship.h"

namespace ship {
void Ship::update(vec2 dir) {
  location = dir;
}

void Ship::draw() {
  gl::color(Color::white());
  gl::draw(shipImage, location);
}
}
