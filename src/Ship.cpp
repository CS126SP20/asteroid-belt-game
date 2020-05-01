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
  shipImage = gl::Texture2d::create(loadImage(cinder::app::loadAsset("spaceship2.png")));
  gl::draw(shipImage, location);
}
}
