//
// Created by Anil Dara on 4/21/20.
//

#include "Leaderboard.h"

namespace leaderboard {

LeaderBoard::LeaderBoard(const std::string& db_path) : db_{db_path} {
  db_ << "CREATE TABLE if not exists leaderboard (\n"
         "  name TEXT NOT NULL,\n"
         "  score INTEGER NOT NULL\n"
         ");";
}

void LeaderBoard::AddNameAndScoreToLeaderBoard(std::string name, int score) {
  db_ << "insert into leaderboard (name, score) values (?, ?);"
      << name
      << score;
}

int LeaderBoard::highest_score() {
  auto highest = db_ << "SELECT MAX(score) FROM leaderboard";
  int highestScore;
  highest >> highestScore;
  return highestScore;
}


}