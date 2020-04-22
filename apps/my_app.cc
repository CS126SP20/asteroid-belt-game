// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"
#include <cinder/app/App.h>
#include <iostream>
namespace myapp {


using cinder::app::KeyEvent;
using namespace ci;
const char kDbPath[] = "leaderboard.db";
bool gameOver = false;
bool gameStart = false;
const char kNormalFont[] = "Arial";

MyApp::MyApp() :
leaderboard_{cinder::app::getAssetPath(kDbPath).string()}
{}
template <typename C>
void PrintText(const std::string& text, const C& color, const cinder::ivec2& size,
               const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(kNormalFont, 10))
      .size(size)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}


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
  for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
    if (checkCollision(p->getLocation(), p->GetRadius())) {
      gl::clear();
      gameOver = true;
    }
  }
}

void MyApp::draw() {
/*
  if (!(gameStart)) {
    gl::clear();
    const cinder::ivec2 size = {500, 50};
    vec2 loc = vec2(getWindowWidth() / 2, getWindowHeight() / 2);
    const Color color = Color::white();
    PrintText("Enter Your Name", color, size, loc);
  }
  */
    if (gameOver) {
      gl::clear();
    } else {
      for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
        p->draw();
      }
      ship.draw();
    }

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

bool MyApp::checkCollision(vec2 loc, int radius) {
  vec2 ship_location = ship.GetLocation();
  int ship_rad = ship.GetRadius();
  float dx = ship_location.x - loc.x;
  float dy = ship_location.y - loc.y;
  float distance = sqrt(dx * dx + dy * dy);
  return distance < ship_rad + radius;
}



}  // namespace myapp

