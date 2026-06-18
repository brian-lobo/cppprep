#pragma once

#include <unordered_set>
#include <string>
#include <vector>
using namespace std;
namespace cppprep::leetcode
{
  class NQueensSolution {
  public:
    vector<vector<string>> solveNQueens(int n) {
      vector<vector<string>> results;

      // Create an empty board initialized with '.'
      vector<string> board(n, string(n, '.'));

      // Trackers for columns and diagonals
      unordered_set<int> cols;
      unordered_set<int> posDiag; // row + col
      unordered_set<int> negDiag; // row - col

      backtrack(0, n, board, results, cols, posDiag, negDiag);
      return results;
    }

  private:
    void backtrack(int row,
        int n,
        vector<string>& board,
        vector<vector<string>>& results,
        unordered_set<int>& cols,
        unordered_set<int>& posDiag,
        unordered_set<int>& negDiag)

    {
      // Base case: If we've successfully placed queens in all rows, record the solution
      if (row == n) {
        results.push_back(board);
        return;
      }

      // Try placing a queen in each column of the current row
      for (int col = 0; col < n; ++col) {
        // Check if the current position is under attack
        if (cols.count(col) || posDiag.count(row + col) || negDiag.count(row - col)) {
          continue;
        }

        // 1. Place the queen
        board[row][col] = 'Q';
        cols.insert(col);
        posDiag.insert(row + col);
        negDiag.insert(row - col);

        // 2. Move to the next row
        backtrack(row + 1, n, board, results, cols, posDiag, negDiag);

        // 3. Backtrack (undo the placement)
        board[row][col] = '.';
        cols.erase(col);
        posDiag.erase(row + col);
        negDiag.erase(row - col);
      }
    }

    class NQueensSolutionTest
    {
    public:
      void run_test()
      {
        NQueensSolution sol;
        sol.solveNQueens(4);
      }
    };


  };
}