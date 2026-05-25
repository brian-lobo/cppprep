#pragma once

#include <string>
#include <vector>

class ValidSudokuSolution {
public:

  std::vector<std::string> fullJustify(std::vector<std::string>& words, int maxWidth) {

  }

  bool isValidSudoku(std::vector<std::vector<char>>& board) {
    bool rows[9][9] = {false};
    bool cols[9][9] = {false};
    bool box[9][9] = {false};

    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        if (board[i][j] == '.') continue;

        int d = board[i][j] - '1';  // 0 to 8

        // Check row
        if (rows[i][d]) return false;
        rows[i][d] = true;

        // Check column
        if (cols[j][d]) return false;
        cols[j][d] = true;

        // Check 3x3 box
        int boxIdx = (i / 3) * 3 + (j / 3);
        if (box[boxIdx][d]) return false;
        box[boxIdx][d] = true;
      }
    }
    return true;
  }
};

class ValidSudokuSolutionTest
{
  public:
    void run_test() {
      {
        ValidSudokuSolution vss;
        std::vector<std::vector<char>> board = {
          {'8','3','.','.','7','.','.','.','.'}
          ,{'6','.','.','1','9','5','.','.','.'}
          ,{'.','9','8','.','.','.','.','6','.'}
          ,{'8','.','.','.','6','.','.','.','3'}
          ,{'4','.','.','8','.','3','.','.','1'}
          ,{'7','.','.','.','2','.','.','.','6'}
          ,{'.','6','.','.','.','.','2','8','.'}
          ,{'.','.','.','4','1','9','.','.','5'}
          ,{'.','.','.','.','8','.','.','7','9'}
        };

        vss.isValidSudoku(board);
      }


      {
        ValidSudokuSolution vss;

        std::vector<std::vector<char>> board = {
          {'5','3','.','.','7','.','.','.','.'}
          ,{'6','.','.','1','9','5','.','.','.'}
          ,{'.','9','8','.','.','.','.','6','.'}
          ,{'8','.','.','.','6','.','.','.','3'}
          ,{'4','.','.','8','.','3','.','.','1'}
          ,{'7','.','.','.','2','.','.','.','6'}
          ,{'.','6','.','.','.','.','2','8','.'}
          ,{'.','.','.','4','1','9','.','.','5'}
          ,{'.','.','.','.','8','.','.','7','9'}
        };

        vss.isValidSudoku(board);
      }
    }
}