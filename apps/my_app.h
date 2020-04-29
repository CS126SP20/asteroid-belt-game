// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>

#include <list>

#include "../src/Asteroid.h"
#include "../src/Leaderboard.h"
#include "../src/Ship.h"
#include <cinder/Font.h>
#include <cinder/Text.h>
#include <chrono>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

namespace myapp {
using cinder::Timer;

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
 private:
  vec2 ship_start_location = vec2(getWindowWidth() / 2, 785);
  std::list<asteroid::Asteroid> asteroid_list;
  ship::Ship ship;
  leaderboard::LeaderBoard leaderboard_;
  bool checkCollision(vec2 loc, int radius);
  std::string input = "";
  const std::string player_name_;
  int highest_asteroid(std::list<asteroid::Asteroid> asteroids);
  gl::Texture2dRef myImage;
  cinder::Timer timer;
  int calculate_score(int elapsed_seconds, int difficulty);
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

