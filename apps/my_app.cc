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
const char papyrus[] = "Papyrus";
bool changeList = false;
int difficulty = 1;
DECLARE_string(name);
MyApp::MyApp() :
leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
player_name_{FLAGS_name},
state_{GameState::kTitleScreen}
{}
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
  background = gl::Texture2d::create(loadImage(loadAsset("starbackground.jpg")));
}
void MyApp::update() {
  if (state_ == GameState::kPlaying) {
    // Repopulates asteroid and laser lists after they go off the screen
    if (changeList) {
      asteroid_counter++;
      laser_counter++;
      asteroid_list.clear();
      PopulateAsteroidList();
      laser_list.clear();
      PopulateLaserList();
      changeList = false;
    }
    if (utilities.HighestAsteroid(asteroid_list) == window_height) {
      changeList = true;
    }
    // Updates asteroid movement
    for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
      gl::clear();
      p->update(vec2(p->getLocation().x, p->getLocation().y + difficulty), 1);
    }
    // Checks for collisions among asteroids and spaceship
    for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
      if (utilities.CheckAsteroidCollision(p->getLocation(), ship.GetLocation())) {
        score = utilities.CalculateScore((int) timer.getSeconds(), difficulty);
        leaderboard_.AddNameAndScoreToLeaderBoard(player_name_, score);
        timer.stop();
        state_ = GameState::kGameOver;
      }
    }
    //Updates laser movement
    for (std::list<laser::Laser>::iterator p = laser_list.begin(); p != laser_list.end(); ++p) {
      int updated_top_point_x;
      int updated_bottom_point_x;
      // Lasers move in the direction that they're oriented towards.
      if (p->GetTopPoint().x > p->GetBottomPoint().x) {
        updated_top_point_x = p->GetTopPoint().x - difficulty;
        updated_bottom_point_x = p->GetBottomPoint().x - difficulty;
      } else {
        updated_top_point_x = p->GetTopPoint().x + difficulty;
        updated_bottom_point_x = p->GetBottomPoint().x + difficulty;
      }
      int updated_top_point_y =  p->GetTopPoint().y + difficulty;
      int updated_bottom_point_y = p->GetBottomPoint().y + difficulty;

      p->update(vec2(updated_top_point_x, updated_top_point_y), vec2(updated_bottom_point_x, updated_bottom_point_y));
    }
    // Checks for collisions among lasers and spaceship
    for (std::list<laser::Laser>::iterator p = laser_list.begin(); p != laser_list.end(); ++p) {
      if (utilities.CheckLaserCollision(p->GetTopPoint(), p->GetBottomPoint(), ship.GetLocation())) {
        score = utilities.CalculateScore((int) timer.getSeconds(), difficulty);
        leaderboard_.AddNameAndScoreToLeaderBoard(player_name_, score);
        timer.stop();
        state_ = GameState::kGameOver;
      }
    }
  }
}
void MyApp::draw() {
  // The title screen
  if (state_ == GameState::kTitleScreen) {
    gl::clear();
    gl::draw(background, getWindowBounds());
    gl::Texture2dRef space_ship = gl::Texture2d::create(loadImage(loadAsset("spaceship4.png")));
    gl::draw(space_ship, vec2(300, 200));
    // Prints title text.
    DisplayText(papyrus, 600, 50, 40, Color::white(),
        "Travel Through the Asteroid Belt!", vec2(100, 100));
    gl::Texture2dRef asteroid = gl::Texture2d::create
        (loadImage(cinder::app::loadAsset("asteroid3.png")));

    DisplayText(papyrus, 600, 50, 30, Color::white()
        , "Click space to continue...", vec2(100, 600));
    // Prints asteroid clipart on title screen.
    gl::Texture2dRef asteroidclip  = gl::Texture2d::create(loadImage(cinder::app::loadAsset("asteroidclip2.png")));
    gl::draw(asteroidclip, vec2(530, 240));
    gl::Texture2dRef asteroid5  = gl::Texture2d::create(loadImage(cinder::app::loadAsset("asteroid5.png")));
    gl::draw(asteroid5, vec2(20, 270));
    DisplayText(papyrus, 600, 50, 15, Color::white(), "Press r for the game rules!", vec2(100, 700));
  }
  // The rules screen.
  if (state_ == GameState::kRules) {
    gl::clear();
    gl::draw(background, getWindowBounds());
    gl::Texture2dRef scroll  = gl::Texture2d::create(loadImage
        (cinder::app::loadAsset("scroll.png")));
    gl::draw(scroll, vec2(90, 100));
    DisplayText(papyrus, 600, 50, 40, Color::black(),
        "Rules:", vec2(100, 215));
    // Prints out the rules formatted on the scroll clipart.
    std::string rules = "- 'W,A,S,D' to move \n"
                        "\n"
                        "- 'E, M, or H to toggle between the \n "
                        "easy, medium, or hard difficulties \n "
                        "\n"
                        "- Avoid the asteroids to survive \n "
                        "and higher your score";
    DisplayText(papyrus, 600, 300, 20,
        Color::black(), rules, vec2(80, 300));
    DisplayText(papyrus, 600, 50,
        30, Color::white(), "Press space to start!", vec2(100, 720));
  }
// The game over screen.
  if (state_ == GameState::kGameOver) {
    gl::clear();
    mVoice->stop();
    gl::disableDepthRead();
    gl::disableDepthRead();
    gl::enableAlphaBlending();
    gl::color(Color::white());
    gl::draw(background, getWindowBounds());
    gl::Texture2dRef gameoverscreen  = gl::Texture2d::create(loadImage
        (cinder::app::loadAsset("gameoverscreenv2.png")));
    gl::draw(gameoverscreen, vec2(150, 50));
    // Draws game over screen text.
    DisplayText(papyrus, 600, 100, 40, Color::white()
        ,"Thanks for playing " + player_name_ + "!", vec2(110, 300));
    DisplayText(papyrus, 600, 40, 20, Color::white()
        , "Highest Score: " + std::to_string(leaderboard_.highest_score()), vec2(110, 400));
    DisplayText(papyrus, 600, 100, 20, Color::white()
        , "Your Score: " + std::to_string(score), vec2(110, 450));
    // Calculates game stats.
    int elapsed_seconds = score / (difficulty * 1.5);
    int score_per_minute;
    if (elapsed_seconds == 0) {
      score_per_minute = 0;
    } else {
      score_per_minute = (60 / elapsed_seconds) * score;
    }
    // Draws game stats.
    DisplayText(papyrus, 600, 100, 20, Color::white(),
        "Elapsed seconds: " + std::to_string(elapsed_seconds), vec2(110, 500));
    DisplayText(papyrus, 600, 100, 20, Color::white(),
        "Score per minute: " + std::to_string(score_per_minute), vec2(110, 550));
    DisplayText(papyrus, 600, 100, 20, Color::white(),
        "Asteroids Dodged: " + std::to_string(asteroid_counter * 14 + 7), vec2(110, 600));
    DisplayText(papyrus, 600, 100, 20, Color::white(),
        "Lasers Dodged: " + std::to_string(laser_counter * 8 + 4) , vec2(110, 650));
  }
  if (state_ == GameState::kPlaying) {
    gl::clear();
    mVoice->start();
    gl::disableDepthRead();
    gl::disableDepthRead();
    gl::enableAlphaBlending();
    gl::color(Color::white());
    gl::draw(background, getWindowBounds());
    // Draws asteroids.
    for (std::list<asteroid::Asteroid>::iterator p = asteroid_list.begin(); p != asteroid_list.end(); ++p) {
      p->draw();
    }
    // Draws lasers.
    for (std::list<laser::Laser>::iterator p = laser_list.begin(); p != laser_list.end(); ++p) {
      p->draw();
    }
    ship.draw();
    // Calculates real time elapsed seconds and score
    std::string elapsed_timer = std::to_string((int) timer.getSeconds());
    std::string score = std::to_string(utilities.CalculateScore(timer.getSeconds(), difficulty));
    std::string text = "Elapsed Seconds: " + elapsed_timer + "\n" "Score: " + score;
    DisplayText(papyrus, 600, 60, 20, Color::white(),
        text, vec2(400, 30));

  }
}
void MyApp::keyDown(KeyEvent event) {
  // Moves ship with 'W', 'A', 'S', D'
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
  // Starts game with space button
  if (event.getCode() == KeyEvent::KEY_SPACE) {
    state_ = GameState::kPlaying;
    timer.start();
  }
  // Change difficulty with 'e','m', h'
  if (event.getChar() == 'e') {
    difficulty = 2;
  }
  if (event.getChar() == 'm') {
    difficulty = 3;
  }
  if (event.getChar() == 'h') {
    difficulty = 5;
  }
  // Display rules screen with 'r'
  if (event.getChar() == 'r') {
    state_ = GameState::kRules;
  }
}

// Prints text.
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

// Populates the laser list.
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
// Populates the aseroid list.
void MyApp::PopulateAsteroidList() {
  for (int i = 0; i < asteroid_number; i++) {
    int rand_height = rand() % random_height_range + 1;
    asteroid_list.push_back(asteroid::Asteroid(vec2(asteroid_width
    + (i * asteroid_spacing), rand_height), asteroid_radius));
  }
}


}  // namespace myapp

