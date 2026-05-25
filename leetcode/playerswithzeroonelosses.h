#pragma once

#include <vector>
#include <string>

class PlayersWithZeroOneLossesSolution {
public:
  using WinnerLoserList = std::vector<std::vector<int>>;

  WinnerLoserList findWinners(WinnerLoserList& matches) {

    WinnerLoserList result(2);

    int size = matches.size();
    std::vector<int> wins(100001,0);
    std::vector<int> losses(100001,0);

    for (int i=0; i < matches.size(); i++)
    {
      wins[matches[i][0]]++;
      losses[matches[i][1]]++;
    }

    for (int i=0; i < wins.size(); i++)
    {
      if (wins[i]>0 && losses[i]==0)
      {
        result[0].push_back(i);
      }

      if (losses[i]==1)
      {
        result[1].push_back(i);
      }
    }
    return result;
  }
};

class PlayersWithZeroOneLossesSolution
{
  public:
      void run_test() {
        {
          PlayersWithZeroOneLossesSolution::WinnerLoserList wll = {
            {1,3},
            {2,3},
            {3,6},
            {5,6},
            {5,7},
            {4,5},
            {4,8},
            {4,9},
            {10,4},
            {10,9}
          };
          PlayersWithZeroOneLossesSolution zos;
          zos.findWinners(wll);
        }

        {
          PlayersWithZeroOneLossesSolution::WinnerLoserList wll = {
            {2,3},
            {1,3},
            {5,4},
            {6,4}
          };
          PlayersWithZeroOneLossesSolution zos;
          zos.findWinners(wll);
        }
    }
};