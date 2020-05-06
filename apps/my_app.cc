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
const char kNormalFont[] = "Impact";
const char timesFont[] = "Times";
const char papyrus[] = "Papyrus";
bool changeList = false;
int difficulty = 1;
// laser::Laser laser = laser::Laser(vec2(50,20), vec2(70,40));

DECLARE_string(name);

MyApp::MyApp() :
leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
player_name_{FLAGS_name},
state_{GameState::kTitleScreen}
{}
template <typename C>
void PrintText(const std::string& text, const C& color, const cinder::ivec2& size,
               const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(papyrus, 40))
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
  // Initializes the asteroid list by pushing back asteroids of random heights.
  PopulateAsteroidList();
  // Initializes the laser list by pushing back lasers of random heights and
  // alternating orientations
  PopulateLaserList();
  // Loads in audio file.
  cinder::audio::SourceFileRef source_file = cinder::audio::load
      (cinder::app::loadAsset("Travis-Scott-Cant-Say-Instrumental.mp3"));
  mVoice = cinder::audio::Voice::create(source_file);
  ship.SetLocation(ship_start_location);

}

void MyApp::update() {
  if (state_ == GameState::kPlaying) {
    if (changeList) {
      asteroid_list.clear();
      PopulateAsteroidList();
      laser_list.clear();
      PopulateLaserList();
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
        score = calculate_score((int) timer.getSeconds(), difficulty);
        leaderboard_.AddNameAndScoreToLeaderBoard(player_name_, score);
        timer.stop();
        state_ = GameState::kGameOver;
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
        score = calculate_score((int) timer.getSeconds(), difficulty);
        leaderboard_.AddNameAndScoreToLeaderBoard(player_name_, score);
        timer.stop();
        state_ = GameState::kGameOver;
      }
    }
  }
}

void MyApp::draw() {

  if (state_ == GameState::kTitleScreen) {
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
        .font(cinder::Font(papyrus, 30))
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
        .font(cinder::Font(papyrus, 15))
        .size(size)
        .color(color)
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Press r for the game rules!");
    const auto box_sizes = boxy.getSize();
    const auto surfaces = boxy.render();
    const auto textures = cinder::gl::Texture::create(surfaces);
    cinder::gl::draw(textures, vec2(100, 700));


  }

  if (state_ == GameState::kRules) {
    gl::clear();
    gl::draw(myImage, getWindowBounds());
    gl::Texture2dRef scroll  = gl::Texture2d::create(loadImage(cinder::app::loadAsset("scroll.png")));
    gl::draw(scroll, vec2(90, 100));
    auto boxy = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(papyrus, 40))
        .size({600, 50})
        .color(Color::black())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Rules:");
    const auto box_sizes = boxy.getSize();
    const auto surfaces = boxy.render();
    const auto textures = cinder::gl::Texture::create(surfaces);
    cinder::gl::draw(textures, vec2(100, 215));

    auto box = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(papyrus, 20))
        .size({600, 300})
        .color(Color::black())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("- 'W,A,S,D' to move \n"
              "\n"
              "- 'E, M, or H to toggle between the \n "
              "easy, medium, or hard difficulties \n "
              "\n"
              "- Avoid the asteroids to survive \n "
              "and higher your score");
    const auto box_size = box.getSize();
    const auto surface = box.render();
    const auto texture = cinder::gl::Texture::create(surface);
    cinder::gl::draw(texture, vec2(80, 300));

    auto box1 = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(papyrus, 30))
        .size({600, 50})
        .color(Color::white())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Press space to start!");
    const auto box_size1 = box1.getSize();
    const auto surface1 = box1.render();
    const auto texture1 = cinder::gl::Texture::create(surface1);
    cinder::gl::draw(texture1, vec2(100, 720));


  }


  if (state_ == GameState::kGameOver) {
    gl::clear();
    mVoice->stop();
    gl::disableDepthRead();
    gl::disableDepthRead();
    gl::enableAlphaBlending();
    gl::color(Color::white());
    gl::draw(myImage, getWindowBounds());

    gl::Texture2dRef gameoverscreen  = gl::Texture2d::create(loadImage(cinder::app::loadAsset("gameoverscreenv2.png")));
    gl::draw(gameoverscreen, vec2(150, 50));

    auto box1 = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(papyrus, 40))
        .size({600, 100})
        .color(Color::white())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Thanks for playing " + player_name_ + "!");
    const auto box_size1 = box1.getSize();
    const auto surface1 = box1.render();
    const auto texture1 = cinder::gl::Texture::create(surface1);
    cinder::gl::draw(texture1, vec2(110, 300));

    auto boxy = TextBox()
        .alignment(TextBox::CENTER)
        .font(cinder::Font(papyrus, 20))
        .size({600, 50})
        .color(Color::white())
        .backgroundColor(ColorA(0, 0, 0, 0))
        .text("Highest Score: " + std::to_string(leaderboard_.highest_score()));
    const auto box_sizes = boxy.getSize();
    const auto surfaces = boxy.render();
    const auto textures = cinder::gl::Texture::create(surfaces);
    cinder::gl::draw(textures, vec2(110, 400));
    DisplayText(papyrus, 600, 100, 20, Color::white(), "Your Score: " + std::to_string(score), vec2(110, 450));
    int elapsed_seconds = score / (difficulty * 1.5);
    int score_per_minute;
    if (elapsed_seconds == 0) {
      score_per_minute = 0;
    } else {
      score_per_minute = (60 / elapsed_seconds) * score;
    }

    DisplayText(papyrus, 600, 100, 20, Color::white(), "Elapsed seconds: " + std::to_string(elapsed_seconds), vec2(110, 500));
    DisplayText(papyrus, 600, 100, 20, Color::white(), "Score per minute: " + std::to_string(score_per_minute), vec2(110, 550));
    DisplayText(papyrus, 600, 100, 20, Color::white(), "Asteroids Dodged: " + std::to_string(asteroid_counter * 14 + 7), vec2(110, 600));
    DisplayText(papyrus, 600, 100, 20, Color::white(), "Lasers Dodged: " + std::to_string(laser_counter * 8 + 4) , vec2(110, 650));



  }



  if (state_ == GameState::kPlaying) {
    gl::clear();
    mVoice->start();
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
    state_ = GameState::kPlaying;
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
    state_ = GameState::kRules;
  }
}

// The check collision method was inspired by :
// https://stackoverflow.com/questions/31022269/collision-detection-between-two-rectangles-in-java

bool MyApp::checkCollision(vec2 loc) {

  // Top left first rectangle
  float x1 = ship.GetLocation().x;
  float y1 = ship.GetLocation().y;

  // Bottom right first rectangle
  float x2 = ship.GetLocation().x + 20;
  float y2 = ship.GetLocation().y + 35;


  // Top left second rectangle
  float xx1 = loc.x;
  float yy1 = loc.y;

  // Bottom right second rectangle
  float xx2 = loc.x + 63;
  float yy2 = loc.y + 51;


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

void MyApp::DisplayText(const char font[], int textbox_width, int textbox_height,
                 int font_size, ColorA color, std::string text, vec2 location) {
  auto textbox = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(font, font_size))
      .size({textbox_width, textbox_height})
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);
  const auto surface = textbox.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, location);
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

void MyApp::PopulateLaserList() {
  for (int i = 0; i < laser_number; i++) {
    int rand_height = rand() % random_height_range + 1;
    int top_point_x = laser_x_start_point + (i * laser_spacing);
    int top_point_y = rand_height;
    int bottom_point_x = laser_y_start_point + (i * laser_spacing);
    int bottom_point_x_alt = laser_y_start_point + (i * laser_spacing) - 40;
    int bottom_point_y = rand_height + 20;
    vec2 bottom_point;
    // Alternates the orientation of the lasers.
    if (i % 2 == 0) {
      bottom_point = vec2(bottom_point_x, bottom_point_y);
    } else {
      bottom_point = vec2(bottom_point_x_alt, bottom_point_y);
    }
    vec2 top_point = vec2(top_point_x, top_point_y);
    laser_list.push_back(laser::Laser(top_point, bottom_point));
  }
}

void MyApp::PopulateAsteroidList() {
  for (int i = 0; i < asteroid_number; i++) {
    int rand_height = rand() % random_height_range + 1;
    asteroid_list.push_back(asteroid::Asteroid(vec2(asteroid_width
    + (i * asteroid_spacing), rand_height), asteroid_radius));
  }
}



// The code below is derived from:
// https://www.tutorialspoint.com/Check-if-two-line-segments-intersect

bool MyApp::onLine(Line l1, Point p) {
  return p.x <= max(l1.p1.x, l1.p2.x) && p.x <= min(l1.p1.x, l1.p2.x) &&
         (p.y <= max(l1.p1.y, l1.p2.y) && p.y <= min(l1.p1.y, l1.p2.y));
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

