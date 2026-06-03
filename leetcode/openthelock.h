#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <queue>

namespace cppprep::leetcode
{
class OpenTheLockSolution {
public:
  int openLock(std::vector<std::string>& deadends, std::string target) {
    std::unordered_set<std::string> visited(deadends.begin(), deadends.end());

    if (visited.count("0000")) return -1;  // Start is deadend

    std::queue<std::pair<std::string, int>> q;  // {current, steps}
    q.push({"0000", 0});
    visited.insert("0000");

    while (!q.empty()) {
      auto [curr, steps] = q.front();
      q.pop();

      if (curr == target) {
        return steps;
      }

      // Try all 8 possible moves
      for (int i = 0; i < 4; i++) {
        for (int dir = -1; dir <= 1; dir += 2) {  // -1 and +1
          std::string next = curr;
          int digit = next[i] - '0';
          int newDigit = (digit + dir + 10) % 10;  // Handle wrap around
          next[i] = newDigit + '0';

          if (visited.find(next) == visited.end()) {
            visited.insert(next);
            q.push({next, steps + 1});
          }
        }
      }
    }

    return -1;
  }
};

  class OpenTheLockSoluvtionTest
  {
    public:
        void run_Test() {
              OpenTheLockSolution otl{};
              std::vector<std::string> deadends = {"0201","0101","0102","1212","2002"};
              std::string target = "0202";
              otl.openLock(deadends, target);
        }
  };

}