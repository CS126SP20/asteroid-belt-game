// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"
#include "cinder/gl/gl.h"
#include <cinder/app/App.h>

namespace myapp {


using cinder::app::KeyEvent;
using namespace ci;
const char kDbPath[] = "leaderboard.db";

MyApp::MyApp() :
leaderboard_{cinder::app::getAssetPath(kDbPath).string()}
{}

void MyApp::setup() {
  for (int i = 0; i < 14; i++) {
    int x = 40;
    int y = 20;
    int radius = 20;
    asteroid_list.push_back(asteroid::Asteroid(vec2(x + (i * 60), y), radius));
    ship.SetLocation(ship_start_location);
    leaderboard_.AddNameAndScoreToLeaderBoard("amish", 2);
  }
}

void MyApp::update() {
  for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
    vec2 center = vec2(getWindowWidth() / 2, getWindowHeight() / 2);
    gl::clear();
    p->update(vec2(p->getLocation().x, p->getLocation().y + 1), 1);
  }
}

void MyApp::draw() {
  for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
    p->draw();
  }
  ship.draw();
}

void MyApp::keyDown(KeyEvent event) {
  if (event.getChar() == 'd') {
    ship.SetLocation(vec2(ship.GetLocation().x + 5, ship.GetLocation().y));
  }
  if (event.getChar() == 'a') {
    ship.SetLocation(vec2(ship.GetLocation().x - 5, ship.GetLocation().y));
  }
  if (event.getChar() == 's') {
    ship.SetLocation(vec2(ship.GetLocation().x, ship.GetLocation().y + 5));
  }
  if (event.getChar() == 'w') {
    ship.SetLocation(vec2(ship.GetLocation().x, ship.GetLocation().y - 5));
  }
}

}  // namespace myapp

