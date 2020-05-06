//
// Created by Anil Dara on 5/5/20.
//

#ifndef FINALPROJECT_LASER_H
#define FINALPROJECT_LASER_H

#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"
#include <cinder/app/App.h>

using namespace ci;
namespace laser {
class Laser {
 private:
  vec2 top_point;
  vec2 bottom_point;

 public:
  Laser(vec2 top, vec2 bottom) {
    top_point = top;
    bottom_point = bottom;
  }

  void update(vec2 updated_top, vec2 updated_bottom);
  void draw();
  vec2 GetTopPoint() {
    return top_point;
  }
  vec2 GetBottomPoint() {
    return bottom_point;
  }
};


} // namespace laser
#endif  // FINALPROJECT_LASER_H
