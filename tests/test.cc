// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <cinder/Rand.h>

#include <catch2/catch.hpp>

#include "../src/Utilities.h"

utilities::Utilities utilities1;

TEST_CASE("Random sanity test", "[random]") {
  const float random = cinder::randFloat();
  REQUIRE(0. <= random);
  REQUIRE(random <= 1.);
}

TEST_CASE("CalculateScore Test") {
  SECTION("Zero Score") {
    int elapsed_seconds = 0;
    int difficulty = 2;
    REQUIRE(utilities1.CalculateScore(elapsed_seconds, difficulty) == 0);
  }
  SECTION("Positive Score") {
    int elapsed_seconds = 5;
    int difficulty = 3;
    REQUIRE(utilities1.CalculateScore(elapsed_seconds, difficulty) == 22);
  }
}

TEST_CASE("HighestAsteroid Test") {
  std::list<asteroid::Asteroid> asteroid_list;
  REQUIRE(utilities1.HighestAsteroid(asteroid_list) == 800);
}


TEST_CASE("CheckLaserCollision Test") {
  SECTION("There is a Collision") {
    vec2 top_location = vec2(10, 10);
    vec2 bottom_location = vec2(20, 20);
    vec2 ship_location = vec2(15, 15);
    REQUIRE(utilities1.CheckLaserCollision(top_location, bottom_location, ship_location) == true);
  }

  SECTION("There isn't Collision") {
    vec2 top_location = vec2(10, 10);
    vec2 bottom_location = vec2(20, 20);
    vec2 ship_location = vec2(100, 100);
    REQUIRE(utilities1.CheckLaserCollision(top_location, bottom_location, ship_location) == false);
  }

}

TEST_CASE("LinesIntersect Test") {
  SECTION("They interesct") {
    REQUIRE(utilities1.LinesIntersect({{20, 20}, {40, 40}}, {{40, 20}, {20, 40}}) == true);
  }
  SECTION("They don't intersect") {
    REQUIRE(utilities1.LinesIntersect({{10, 20}, {10, 5}}, {{100, 80}, {96, 100}}) == false);
  }
}

TEST_CASE("Direction Test") {
  SECTION("Clockwise direction") {
    REQUIRE(utilities1.Direction({5, 5}, {6, 7}, {8, 9}) == 1);
  }
  SECTION("Colinear") {
    REQUIRE(utilities1.Direction({5, 5}, {5, 5}, {5, 5}) == 0);
  }
}

TEST_CASE("CheckAsteroidCollision Test") {
  SECTION("They interesct") {
    vec2 asteroid_loc = vec2(100, 100);
    vec2 ship_loc = vec2(100, 100);
    REQUIRE(utilities1.CheckAsteroidCollision(asteroid_loc, ship_loc) == true);
  }
  SECTION("They don't intersect") {
    vec2 asteroid_loc = vec2(100, 100);
    vec2 ship_loc = vec2(400, 400);
    REQUIRE(utilities1.CheckAsteroidCollision(asteroid_loc, ship_loc) == false);
  }
}