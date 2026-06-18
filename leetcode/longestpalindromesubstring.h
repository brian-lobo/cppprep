#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace cppprep::leetcode
{
  class SolutionPalindrome {
  public:
    class Solution {
    public:
      std::string longestPalindrome(std::string s) {
        if (s.length() <= 1) return s;

        int start = 0;
        int maxLen = 1; // Any single character is a palindrome of length 1

        for (int i = 0; i < s.length(); i++) {
          // Case 1: Odd length palindromes (e.g., "aba", center is s[i])
          expandAroundCenter(s, i, i, start, maxLen);

          // Case 2: Even length palindromes (e.g., "abba", center is between s[i] and s[i+1])
          expandAroundCenter(s, i, i + 1, start, maxLen);
        }

        return s.substr(start, maxLen);
      }

    private:
      void expandAroundCenter(const std::string& s, int left, int right, int& start, int& maxLen) {
        // Expand outwards as long as characters match and boundaries are valid
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
          int currentLen = right - left + 1;
          if (currentLen > maxLen) {
            maxLen = currentLen;
            start = left;
          }
          left--;
          right++;
        }
      }
    };


  };


  class SolutionPalindromeTest
  {
    public:
      void run_test() {
        SolutionPalindrome sp;
        std::string inp1{"babad"};
        std::cout << sp.longestPalindrome(inp1) <<std::endl;

        std::string inp2{"cbbd"};
        std::cout << sp.longestPalindrome(inp2) <<std::endl;

        std::string inp3{"bb"};
        std::cout << sp.longestPalindrome(inp3) <<std::endl;
      }
  };

}

