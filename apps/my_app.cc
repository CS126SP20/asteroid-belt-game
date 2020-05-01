// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <gflags/gflags_declare.h>



#include <iostream>
namespace myapp {


using cinder::app::KeyEvent;
using namespace ci;
const char kDbPath[] = "leaderboard.db";
bool gameOver = false;
bool gameStart = false;
bool changeList = false;
const char kNormalFont[] = "Times";
int difficulty = 1;

DECLARE_string(name);

MyApp::MyApp() :
leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
player_name_{FLAGS_name}
{}
template <typename C>
void PrintText(const std::string& text, const C& color, const cinder::ivec2& size,
               const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(kNormalFont, 20))
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
    int rand_height = rand() % 600 + 1;
    asteroid_list.push_back(asteroid::Asteroid(vec2(x + (i * 60), rand_height), radius));
    ship.SetLocation(ship_start_location);
  }

}

void MyApp::update() {
  if (gameStart && !(gameOver)) {
    if (changeList) {
      asteroid_list.clear();
      for (int i = 0; i < 14; i++) {
        int x = 40 + (i * 60);
        int radius = 20;
        int rand_height = rand() % 600 + 1;
        /*
        if (checkCollision(vec2(x, rand_height), radius)) {
          rand_height += 50;
        }*/
        asteroid_list.push_back(asteroid::Asteroid(vec2(x, rand_height), radius));
      }
      changeList = false;
    }
    if (highest_asteroid(asteroid_list) == 800) {
      changeList = true;
    }
    for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
      vec2 center = vec2(getWindowWidth() / 2, getWindowHeight() / 2);
      gl::clear();
      p->update(vec2(p->getLocation().x, p->getLocation().y + difficulty), 1);
    }


    for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {

      if (checkCollision(p->getLocation())) {
        timer.stop();
        leaderboard_.AddNameAndScoreToLeaderBoard(player_name_, calculate_score((int) timer.getSeconds(), difficulty));
        gameOver = true;
        gameStart = false;
      }
    }
  }
}

void MyApp::draw() {

  if (!(gameStart) && !(gameOver)) {
    myImage = gl::Texture2d::create(loadImage(loadAsset("starbackground.jpg")));
    gl::clear();
    gl::draw(myImage, getWindowBounds());
    const cinder::ivec2 size = {500, 50};
    vec2 loc = vec2(getWindowWidth() / 2, getWindowHeight() / 2);
    const Color color = Color::white();
    PrintText("Click space to continue...", color, size, loc);
    PrintText("Travel Through the Asteroid Belt!", color, size, vec2(getWindowWidth() / 2, 100));
    PrintText("Press 'e', 'm', or 'h' to set your difficulty to easy, medium, or hard!!!", color, size, vec2(getWindowWidth() / 2, 500));
    gl::Texture2dRef asteroid = gl::Texture2d::create(loadImage(cinder::app::loadAsset("asteroid3.png")));


  }
  if (gameOver && !(gameStart)) {
    gl::clear();
    gl::disableDepthRead();
    gl::disableDepthRead();
    gl::enableAlphaBlending();
    gl::color(Color::white());
    gl::draw(myImage, getWindowBounds());
    const cinder::ivec2 size = {500, 50};
    vec2 loc = vec2(getWindowWidth() / 2, getWindowHeight() / 2);
    const Color color = Color::white();
    PrintText("You Lost!", color, size, loc);
    std::string text = "Highest Score: " + std::to_string(leaderboard_.highest_score());
    PrintText(text, color, size, vec2(getWindowWidth() / 2, 300));
  }
  if (gameStart && !(gameOver)) {
    gl::clear();
    gl::disableDepthRead();
    gl::disableDepthRead();
    gl::enableAlphaBlending();
    gl::color(Color::white());
    gl::draw(myImage, getWindowBounds());
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
  if (event.getCode() == KeyEvent::KEY_SPACE) {
    gameStart = true;
    timer.start();
  }
  if (event.getChar() == 'e') {
    difficulty = 2;
  }
  if (event.getChar() == 'm') {
    difficulty = 3;
  }
  if (event.getChar() == 'h') {
    difficulty = 5;
  }
}

bool MyApp::checkCollision(vec2 loc) {

  // Top left first rectangle
  int x1 = ship.GetLocation().x;
  int y1 = ship.GetLocation().y;

  // Bottom right first rectangle
  int x2 = ship.GetLocation().x + 20;
  int y2 = ship.GetLocation().y + 35;




  // Top left second rectangle
  int xx1 = loc.x;
  int yy1 = loc.y;

  // Bottom right second rectangle
  int xx2 = loc.x + 63;
  int yy2 = loc.y + 51;

  if ((x1 < xx2) && (x2 > xx1) && (y1 < yy2) && (y2 > yy1)) {
    return true;
  } else {
    return false;
  }


  /*
  if (x1 >= xx2 || xx1 >= x2) {
    return false;
  }

  if (y1 <= yy2 || yy1 <= y2) {
    return false;
  }
  return true;
*/
}

int MyApp::highest_asteroid(std::list<asteroid::Asteroid> asteroids) {
  int highest_height = 800;
  for (std::list<asteroid::Asteroid>::iterator p = asteroids.begin(); p != asteroids.end(); ++p) {
    if (p->getLocation().y < highest_height) {
      highest_height = p->getLocation().y;
    }
  }
  return highest_height;
}

int MyApp::calculate_score(int elapsed_seconds, int difficulty) {
  return elapsed_seconds * difficulty;
}

}  // namespace myapp

