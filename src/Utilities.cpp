//
// Created by Anil Dara on 5/6/20.
//

#include "Utilities.h"
namespace utilities {
// Calculates game score for the player.
int Utilities::CalculateScore(int elapsed_seconds, int difficulty) {
  return elapsed_seconds * 1.5 * difficulty;
}

// Returns the integer height of the highest asteroid on the screen.
int Utilities::HighestAsteroid(list<asteroid::Asteroid> asteroids) {
  int highest_height = 800;
  for (std::list<asteroid::Asteroid>::iterator p = asteroids.begin(); p != asteroids.end(); ++p) {
    if (p->getLocation().y < highest_height) {
      highest_height = p->getLocation().y;
    }
  }
  return highest_height;
}

// Checks whether there is a collision between the laser and the space ship or not.
bool Utilities::CheckLaserCollision(vec2 top_location, vec2 bottom_location, vec2 ship_location) {
  Line laser_line = {{top_location.x, top_location.y}, {bottom_location.x, bottom_location.y}};
  vec2 loc = ship_location;
  Line ship_left_line = {{loc.x, loc.y}, {loc.x, loc.y + 35}};
  Line ship_right_line = {{loc.x + 20, loc.y}, {loc.x + 20, loc.y + 35}};
  Line ship_top_line = {{loc.x, loc.y}, {loc.x + 20, loc.y}};
  Line ship_bottom_line = {{loc.x, loc.y + 35}, {loc.x + 20, loc.y + 35}};
  if (LinesIntersect(laser_line, ship_left_line)) {
    return true;
  }
  if (LinesIntersect(laser_line, ship_right_line)) {
    return true;
  }
  if (LinesIntersect(laser_line, ship_top_line)) {
    return true;
  }
  if (LinesIntersect(laser_line, ship_bottom_line)) {
    return true;
  }
  return false;
}
// The check asteroid   collision method was inspired by :
// https://stackoverflow.com/questions/31022269/collision-detection-between-two-rectangles-in-java
bool Utilities::CheckAsteroidCollision(vec2 loc, vec2 ship_location) {
  // Top left first rectangle
  float x1 = ship_location.x;
  float y1 = ship_location.y;
  // Bottom right first rectangle
  float x2 = ship_location.x + 20;
  float y2 = ship_location.y + 35;
  // Top left second rectangle
  float xx1 = loc.x;
  float yy1 = loc.y;
  // Bottom right second rectangle
  float xx2 = loc.x + 63;
  float yy2 = loc.y + 51;
  return (x1 < xx2) && (x2 > xx1) && (y1 < yy2) && (y2 > yy1);
}


// The code below is derived from:
// https://www.tutorialspoint.com/Check-if-two-line-segments-intersect
// Returns the orientation of the 2 lines.
bool Utilities::LinesIntersect(Line l1, Line l2) {
  // Four direction for two lines and points of other line
  int dir1 = Direction(l1.p1, l1.p2, l2.p1);
  int dir2 = Direction(l1.p1, l1.p2, l2.p2);
  int dir3 = Direction(l2.p1, l2.p2, l1.p1);
  int dir4 = Direction(l2.p1, l2.p2, l1.p2);
  return dir1 != dir2 && dir3 != dir4;  // They are intersecting
}
// Returns the orientation of the 2 lines.
int Utilities::Direction(Point a, Point b, Point c){
  float val = (b.y-a.y)*(c.x-b.x)-(b.x-a.x)*(c.y-b.y);
  if (val == 0)
    return 0;     //colinear
  else if(val < 0)
    return 2;    //anti-clockwise direction
  return 1;    //clockwise direction
}

}