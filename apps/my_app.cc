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
bool showRules = false;
bool showHomeScreen = false;
const char kNormalFont[] = "Impact";
const char timesFont[] = "Times";
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
    for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
      p->draw();
    }
    ship.draw();

    std::string elapsed_timer = std::to_string((int) getElapsedSeconds());
    std::string score = std::to_string(calculate_score(getElapsedSeconds(), difficulty));
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
    cinder::gl::draw(textures, vec2(400, 50));

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

  if ((x1 < xx2) && (x2 > xx1) && (y1 < yy2) && (y2 > yy1)) {
    return true;
  } else {
    return false;
  }

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

}  // namespace myapp

