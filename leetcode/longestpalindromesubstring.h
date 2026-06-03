#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace cppprep::leetcode
{
  class SolutionPalindrome {
  public:

    std::string oddCase(int pivot, std::string s )
    {
      std::string palindrome{""};
      int left{pivot-1}, right{pivot+1};
      while(left>=0 && right < s.length())
      {
        if (s[left] != s[right])
        {
          break;
        }
        palindrome = s.substr(left, (right-left)+1);
        left--; right++;
      }
      return palindrome;
    }

    std::string evenCase(int pivot, std::string s )
    {
      std::string palindrome{""};
      int left{pivot}, right{pivot+1};
      while(left >=0 && right < s.length())
      {
        if (s[left] != s[right])
        {
          break;
        }
        palindrome = s.substr(left, right-left+1);
        left--; right++;

      }
      return palindrome;
    }

    std::string longestPalindrome(std::string s) {
      std::string palindrome{""};
      for (int i{0}; i< s.length(); i++)
      {
        std::string oddString{oddCase(i, s)};

        std::string evenString{evenCase(i, s)};

        std::string maxString = (oddString.length()>evenString.length())? oddString : evenString;
        palindrome = (maxString.length() > palindrome.length())? maxString : palindrome;
      }
      return palindrome;
    }
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

