// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include "../src/Asteroid.h"
#include "../src/Ship.h"
#include <list>


namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
 private:
  vec2 ship_start_location = vec2(getWindowWidth() / 2, 600);
  std::list<asteroid::Asteroid> asteroid_list;
  ship::Ship ship;

};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

