#pragma once

#include <string>
#include <unordered_map>

class LengthOfLongestStringSolution
{
public:
  int lengthOfLongestSubstring(std::string s) {
    if (s.length() == 0) return 0;
    if (s.length() == 1) return 1;

    int i{0}, j{0}, maxSubString{0};
    std::unordered_map<char, int> count{};
    while (j < s.length())
    {
      if (count[s[j]] == 0)
      {
        count[s[j]]++;
      } else
      {
        maxSubString = ::std::max(maxSubString, j-i);
        //forward the pointer
        while (s[i] != s[j] & i < j) {
          i++;
        }
        i++;
      }
      j++;
    }
    return maxSubString;
  }
};

class LengthOfLongestStringSolutionTest
{
  public:
    void run_test()   {
      LengthOfLongestStringSolution s;

      {
        std::string subStr{"pwwkew"};
        std::cout << subStr << " : " << s.lengthOfLongestSubstring(subStr) << std::endl;
      }

      {
        std::string subStr{"abcabcbb"};
        std::cout << subStr << " : " << s.lengthOfLongestSubstring(subStr) << std::endl;
      }

      {
        std::string subStr{"bbbbb"};
        std::cout << subStr << " : " << s.lengthOfLongestSubstring(subStr) << std::endl;
      }

      {
        std::string subStr{" "};
        std::cout << subStr << " : " << s.lengthOfLongestSubstring(subStr) << std::endl;
      }

      {
        std::string subStr{" "};
        std::cout << subStr << " : " << s.lengthOfLongestSubstring(subStr) << std::endl;
      }
    }

};