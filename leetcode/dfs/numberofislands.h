#pragma once

#include <ios>
#include <vector>

namespace leetcode::dfs
{

  class NumberOfIslandsSolution
  {
    public:
      int numIslands(std::vector<std::vector<char>>& grid) {
        int numIslands{0};

        for (int i=0; i < grid.size(); ++i)
        {
          for (int j=0; j < grid.size(); ++j)
          {
            if (grid[i][j] == '1')
            {
              numIslands++;
              dfs(grid, i, j);
            }
          }
        }
        return numIslands;
      }

      void dfs(std::vector<std::vector<char>>& grid, int row, int col) {

        if ((row < 0 || row >=grid.size())
            || (col < 0 || col > grid[0].size())
            || grid[row][col] == '0'
            )
        {
          return;
        }

        grid[row][col] = '0';
        dfs(grid, row-1, col);
        dfs(grid, row+1, col);
        dfs(grid, row, col-1);
        dfs(grid, row, col+1);
      }
  };

  class NumberOfIslandsSolutionTest
  {
  public:
    void run_test() {

      NumberOfIslandsSolution sol;
      {
        std::vector<std::vector<char>> grid{
            {'1','1','1','1','0'},
            {'1','1','0','1','0'},
            {'1','1','0','0','0'},
            {'0','0','0','0','0'}
        };
        std::cout << std::boolalpha << "Passed : " << (sol.numIslands(grid) == 1) << std::endl;
      }

      {
        std::vector<std::vector<char>> grid{
          {'1','1','0','0','0'},
          {'1','1','0','0','0'},
          {'0','0','1','0','0'},
          {'0','0','0','1','1'}
        };
        std::cout << std::boolalpha << "Passed : " << (sol.numIslands(grid) == 3) << std::endl;
      }

    }

  };


}
