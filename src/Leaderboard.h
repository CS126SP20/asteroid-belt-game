//
// Created by Anil Dara on 4/21/20.
//

#ifndef FINALPROJECT_LEADERBOARD_H
#define FINALPROJECT_LEADERBOARD_H
#include <sqlite_modern_cpp.h>

namespace leaderboard {
class LeaderBoard {
 public:
  explicit LeaderBoard(const std::string& db_path);
  void AddNameAndScoreToLeaderBoard(std::string name, int score);

 private:
  sqlite::database db_;
};
}
#endif  // FINALPROJECT_LEADERBOARD_H
