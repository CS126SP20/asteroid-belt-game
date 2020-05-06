// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cinder/Rand.h>

#include <mylibrary/example.h>
#include "/Users/anildara/Downloads/NewCinder/my-projects/final-project-adara2/apps/my_app.h"


TEST_CASE("Random sanity test", "[random]") {
  const float random = cinder::randFloat();
  REQUIRE(0. <= random);
  REQUIRE(random <= 1.);
}

TEST_CASE("CalculateScore Test") {
  SECTION("Zero Score") {
    int elapsed_seconds = 0;
    int difficulty = 2;

  }

  SECTION("Positve Score") {

  }
}