#pragma once
#include <string>
#include <vector>

namespace cppprep::leetcode
{
  class LetterCombinationsOfAPhoneNumberSolution
  {
  private:
    void backtrack(std::string digits, int index, std::string& current, std::vector<std::string>& result, const std::vector<std::string>& mapping) {
      // Base case: if the current combination is done
      if (index == digits.length()) {
        result.push_back(current);
        return;
      }

      // Get the letters that the current digit maps to
      std::string letters = mapping[digits[index] - '0'];

      // Loop through all letters mapped to the current digit
      for (char letter : letters) {
        current.push_back(letter);       // Choose
        backtrack(digits, index + 1, current, result, mapping); // Explore
        current.pop_back();              // Unchoose (backtrack)
      }
    }

  public:
    std::vector<std::string> letterCombinations(std::string digits) {
      if (digits.empty()) {
        return {};
      }

      // Index maps directly to the keypad number
      std::vector<std::string> mapping = {
        "",     // 0
        "",     // 1
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };

      std::vector<std::string> result;
      std::string current = "";

      backtrack(digits, 0, current, result, mapping);

      return result;
    }
  };

}
