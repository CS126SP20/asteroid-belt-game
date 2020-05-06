// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <gflags/gflags_declare.h>

#include <iostream>

#include "../src/Laser.h"
namespace myapp {


using cinder::app::KeyEvent;
using namespace ci;
using namespace std;
const char kDbPath[] = "leaderboard.db";
bool gameOver = false;
bool gameStart = false;
bool changeList = false;
bool showRules = false;
bool showHomeScreen = false;
const char kNormalFont[] = "Impact";
const char timesFont[] = "Times";
int difficulty = 1;
// laser::Laser laser = laser::Laser(vec2(50,20), vec2(70,40));

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
      .font(cinder::Font(kNormalFont, 40))
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
    Url url = Url("https://www.1001fonts.com/cursive-fonts.html", false);
    // DataSourceRef dataSource = DataSource("assets/Precious.ttf", url);
    // Font cursive = Font(fontcurs, 10);
  }
  for (int i = 0; i < 8; i++) {
    int rand_height = rand() % 600 + 1;
    int top_point_x = 50 + (i * 100);
    int top_point_y = rand_height;
    int bottom_point_x = 70 + (i * 100);
    int bottom_point_x_alt = 70 + (i * 100) - 40;
    int bottom_point_y = rand_height + 20;
    vec2 bottom_point;
    if (i % 2 == 0) {
      bottom_point = vec2(bottom_point_x, bottom_point_y);
    } else {
      bottom_point = vec2(bottom_point_x_alt, bottom_point_y);
    }
    vec2 top_point = vec2(top_point_x, top_point_y);
    laser_list.push_back(laser::Laser(top_point, bottom_point));
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
      laser_list.clear();
      for (int i = 0; i < 8; i++) {
        int rand_height = rand() % 600 + 1;
        int top_point_x = 50 + (i * 100);
        int top_point_y = rand_height;
        int bottom_point_x = 70 + (i * 100);
        int bottom_point_x_alt = 70 + (i * 100) - 40;
        int bottom_point_y = rand_height + 20;
        vec2 bottom_point;
        if (i % 2 == 0) {
          bottom_point = vec2(bottom_point_x, bottom_point_y);
        } else {
          bottom_point = vec2(bottom_point_x_alt, bottom_point_y);
        }
        vec2 top_point = vec2(top_point_x, top_point_y);
        laser_list.push_back(laser::Laser(top_point, bottom_point));
      }
      changeList = false;
    }



    if (highest_asteroid(asteroid_list) == 800) {
      changeList = true;
    }

    // laser.update(vec2(laser.GetTopPoint().x + 4, laser.GetTopPoint().y + 4), vec2(laser.GetBottomPoint().x + 4, laser.GetBottomPoint().y + 4));
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

    for (std::list<laser::Laser>::iterator p = laser_list.begin(); p != laser_list.end(); ++p) {
      int updated_top_point_x;
      int updated_top_point_y;
      int updated_bottom_point_x;
      int updated_bottom_point_y;
      if (p->GetTopPoint().x > p->GetBottomPoint().x) {
        updated_top_point_x = p->GetTopPoint().x - difficulty;
        updated_top_point_y = p->GetTopPoint().y + difficulty;
        updated_bottom_point_x = p->GetBottomPoint().x - difficulty;
        updated_bottom_point_y = p->GetBottomPoint().y + difficulty;
      } else {
        updated_top_point_x = p->GetTopPoint().x + difficulty;
        updated_top_point_y = p->GetTopPoint().y + difficulty;
        updated_bottom_point_x = p->GetBottomPoint().x + difficulty;
        updated_bottom_point_y = p->GetBottomPoint().y + difficulty;
      }
      p->update(vec2(updated_top_point_x, updated_top_point_y), vec2(updated_bottom_point_x, updated_bottom_point_y));
    }
    for (std::list<laser::Laser>::iterator p = laser_list.begin(); p != laser_list.end(); ++p) {
      if (checkLaserColision(p->GetTopPoint(), p->GetBottomPoint())) {
        gameOver = true;
        gameStart = false;
      }
    }
  }
}

void MyApp::draw() {

  if ((showHomeScreen) || (!(gameStart) && !(gameOver))) {
    myImage = gl::Texture2d::create(loadImage(loadAsset("starbackground.jpg")));
    gl::clear();
    gl::draw(myImage, getWindowBounds());
    gl::Texture2dRef space_ship = gl::Texture2d::create(loadImage(loadAsset("spaceship4.png")));
    gl::draw(space_ship, vec2(300, 200));
    const cinder::ivec2 size = {600, 50};
    vec2 loc = vec2(getWindowWidth() / 2, getWindowHeight() / 2);
    const Color color = Color::white();
    // PrintText("Click space to continue...", color, size, loc);
    PrintText("Travel Through the Asteroid Belt!", color, size, vec2(getWindowWidth() / 2, 100));
    // PrintText("Press 'e', 'm', or 'h' to set your difficulty to easy, medium, or hard!!!", color, size, vec2(getWindowWidth() / 2, 500));
    gl::Texture2dRef asteroid = gl::Texture2d::create(loadImage(cinder::app::loadAsset("asteroid3.png")));
    auto box = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(kNormalFont, 30))
        .size(size)
        .color(color)
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Click space to continue...");
    const auto box_size = box.getSize();
    const auto surface = box.render();
    const auto texture = cinder::gl::Texture::create(surface);
    cinder::gl::draw(texture, vec2(100, 600));
    gl::Texture2dRef asteroidclip  = gl::Texture2d::create(loadImage(cinder::app::loadAsset("asteroidclip2.png")));
    gl::draw(asteroidclip, vec2(530, 240));
    gl::Texture2dRef asteroid5  = gl::Texture2d::create(loadImage(cinder::app::loadAsset("asteroid5.png")));
    gl::draw(asteroid5, vec2(20, 270));

    auto boxy = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(kNormalFont, 15))
        .size(size)
        .color(color)
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Press r for the game rules!");
    const auto box_sizes = boxy.getSize();
    const auto surfaces = boxy.render();
    const auto textures = cinder::gl::Texture::create(surfaces);
    cinder::gl::draw(textures, vec2(100, 700));


  }

  if (showRules) {
    gl::clear();
    gl::draw(myImage, getWindowBounds());
    auto boxy = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(timesFont, 40))
        .size({600, 50})
        .color(Color::white())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Rules:");
    const auto box_sizes = boxy.getSize();
    const auto surfaces = boxy.render();
    const auto textures = cinder::gl::Texture::create(surfaces);
    cinder::gl::draw(textures, vec2(100, 100));

    auto box = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(timesFont, 20))
        .size({600, 100})
        .color(Color::white())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("- 'W,A,S,D' to move \n"
              "\n"
              "- 'E, M, or H to toggle between the easy, medium, or hard difficulties \n"
              "\n"
              "- Avoid the asteroids to survive and higher your score");
    const auto box_size = box.getSize();
    const auto surface = box.render();
    const auto texture = cinder::gl::Texture::create(surface);
    cinder::gl::draw(texture, vec2(100, 300));

    auto box1 = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(timesFont, 20))
        .size({600, 50})
        .color(Color::white())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Press space to start!");
    const auto box_size1 = box1.getSize();
    const auto surface1 = box1.render();
    const auto texture1 = cinder::gl::Texture::create(surface1);
    cinder::gl::draw(texture1, vec2(100, 700));


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
    //PrintText("You Lost!", color, size, loc);
    std::string text = "Highest Score: " + std::to_string(leaderboard_.highest_score());
    PrintText(text, color, size, vec2(getWindowWidth() / 2, 300));
    gl::Texture2dRef gameoverscreen  = gl::Texture2d::create(loadImage(cinder::app::loadAsset("gameoverscreenv2.png")));
    gl::draw(gameoverscreen, vec2(150, 50));
  }



  if (gameStart && !(gameOver)) {
    gl::clear();
    gl::disableDepthRead();
    gl::disableDepthRead();
    gl::enableAlphaBlending();
    gl::color(Color::white());
    gl::draw(myImage, getWindowBounds());
    // laser.draw();
    for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
      p->draw();
    }
    for (std::list<laser::Laser>::iterator p = laser_list.begin(); p != laser_list.end(); ++p) {
      p->draw();
    }
    ship.draw();

    std::string elapsed_timer = std::to_string((int) timer.getSeconds());
    std::string score = std::to_string(calculate_score(timer.getSeconds() , difficulty));
    auto boxy = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(timesFont, 20))
        .size({600, 50})
        .color(Color::white())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Elapsed Seconds: " + elapsed_timer + "\n"
                                                    "Score: " + score);
    const auto box_sizes = boxy.getSize();
    const auto surfaces = boxy.render();
    const auto textures = cinder::gl::Texture::create(surfaces);
    cinder::gl::draw(textures, vec2(400, 30));

  }
}


void MyApp::keyDown(KeyEvent event) {
  if (event.getChar() == 'd') {
    ship.SetLocation(vec2(ship.GetLocation().x + 8, ship.GetLocation().y));
  }
  if (event.getChar() == 'a') {
    ship.SetLocation(vec2(ship.GetLocation().x - 8, ship.GetLocation().y));
  }
  if (event.getChar() == 's') {
    ship.SetLocation(vec2(ship.GetLocation().x, ship.GetLocation().y + 8));
  }
  if (event.getChar() == 'w') {
    ship.SetLocation(vec2(ship.GetLocation().x, ship.GetLocation().y - 8));
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
  if (event.getChar() == 'r') {
    showRules = true;
  }
  if (event.getChar() == 'b') {
    showHomeScreen = true;
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

  return (x1 < xx2) && (x2 > xx1) && (y1 < yy2) && (y2 > yy1);
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
  return elapsed_seconds * 1.5 * difficulty;
}


bool MyApp::checkLaserColision(vec2 top_location, vec2 bottom_location) {
  Line laser_line = {{top_location.x, top_location.y}, {bottom_location.x, bottom_location.y}};
  vec2 loc = ship.GetLocation();
  Line ship_left_line = {{loc.x, loc.y}, {loc.x, loc.y + 35}};
  Line ship_right_line = {{loc.x + 20, loc.y}, {loc.x + 20, loc.y + 35}};
  Line ship_top_line = {{loc.x, loc.y}, {loc.x + 20, loc.y}};
  Line ship_bottom_line = {{loc.x, loc.y + 35}, {loc.x + 20, loc.y + 35}};
  if (linesIntersect(laser_line, ship_left_line)) {
    return true;
  }
  if (linesIntersect(laser_line, ship_right_line)) {
    return true;
  }
  if (linesIntersect(laser_line, ship_top_line)) {
    return true;
  }
  if (linesIntersect(laser_line, ship_bottom_line)) {
    return true;
  }
  return false;
}

// The code below is derived from:
// https://www.tutorialspoint.com/Check-if-two-line-segments-intersect

bool MyApp::onLine(Line l1, Point p) {
  if(p.x <= max(l1.p1.x, l1.p2.x) && p.x <= min(l1.p1.x, l1.p2.x) &&
     (p.y <= max(l1.p1.y, l1.p2.y) && p.y <= min(l1.p1.y, l1.p2.y)))
    return true;
  return false;
}

int MyApp::direction(Point a, Point b, Point c) {
  float val = (b.y-a.y)*(c.x-b.x)-(b.x-a.x)*(c.y-b.y);
  if (val == 0)
    return 0;     //colinear
  else if(val < 0)
    return 2;    //anti-clockwise direction
  return 1;    //clockwise direction
}

bool MyApp::linesIntersect(Line l1, Line l2) {
  //four direction for two lines and points of other line
  int dir1 = direction(l1.p1, l1.p2, l2.p1);
  int dir2 = direction(l1.p1, l1.p2, l2.p2);
  int dir3 = direction(l2.p1, l2.p2, l1.p1);
  int dir4 = direction(l2.p1, l2.p2, l1.p2);

  if(dir1 != dir2 && dir3 != dir4)
    return true; //they are intersecting

  return false;
}

}  // namespace myapp

