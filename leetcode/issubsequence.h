#pragma once

#include <string>

namespace cppprep::leetcode
{
  class IsSubsequenceSolution {
  public:
    bool isSubsequence(std::string s, std::string t) {
      int i{0}, j{0};

      while (i < t.length() && j < s.length())
      {
        while (i < t.length() && t[i] != s[j]) { i++;}

        if (t[i] == s[j]) { i++; j++;}

        if (j == s.length()) break;
      }

      return (j == s.length());
    }
  };

  class IsSubsequenceSolutionTest
  {
      public:
        void run_test() {
          {
            std::string s{"abc"};
            std::string t{"ahbgdc"};
            Solution ss;
            std::cout << std::boolalpha << ss.isSubsequence(s, t) << std::endl;
          }

          {
            std::string s{"axc"};
            std::string t{"ahbgdc"};

            Solution ss;
            std::cout << std::boolalpha << ss.isSubsequence(s, t) << std::endl;
          }

          {
            std::string s{"aaaaaa"};
            std::string t{"bbaaaa"};

            Solution ss;
            std::cout << std::boolalpha << ss.isSubsequence(s, t) << std::endl;
          }
        }
  };

}
