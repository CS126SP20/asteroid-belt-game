//
// Created by Anil Dara on 5/6/20.
//

#ifndef FINALPROJECT_UTILITIES_H
#define FINALPROJECT_UTILITIES_H
#include "Asteroid.h"

using namespace std;
namespace utilities {
class Utilities {
  struct Point {
    float x, y;
  };
  struct Line {
    Point p1, p2;
  };
 public:
  int CalculateScore(int elapsed_seconds, int difficulty);
  int HighestAsteroid(list<asteroid::Asteroid> asteroids);
  bool CheckLaserCollision(vec2 top_location, vec2 bottom_location, vec2 ship_location);
  bool LinesIntersect(Line l1, Line l2);
  int Direction(Point a, Point b, Point c);
  bool CheckAsteroidCollision(vec2 loc, vec2 ship_location);


};
}
#endif  // FINALPROJECT_UTILITIES_H
