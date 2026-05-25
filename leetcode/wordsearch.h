#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace cppprep::leetcode
{
  class WordSearchSolution
  {
  public:
    using Matrix = std::vector<std::vector<char>>;
    bool exist(Matrix& board, std::string& word) {
      std::vector<char> permutations;

      for (int row = 0; row < board.size(); ++row)
      {
        for (int col = 0; col < board[row].size(); ++col)
        {
          if (dfs(board, word, 0, row, col))
          {
            return true;
          }
        }

      }
      return false;
    }

    bool dfs(Matrix& board, const std::string& word, int index, int row, int col) {
      if (index == word.size())
      {
        return true;
      }

      // Ensure that the row/col does not exceed the board bounds and that the board[row][col] != word[index]
      if (row < 0 || row >= board.size() || col < 0 || col >= board[row].size() || board[row][col] != word[index])
      {
        return false;
      }

      char temp = board[row][col];
      board[row][col] = '#';

      // Explore 4 directions: up, down, left, right
      bool found = dfs(board, word, index + 1, row - 1, col) ||  // up
                   dfs(board, word, index + 1, row + 1, col) ||  // down
                   dfs(board, word, index + 1, row, col - 1) ||  // left
                   dfs(board, word, index + 1, row, col + 1);    // right

      // backtrack: restore the cell
      board[row][col] = temp;

      return found;
    }

  };
}
