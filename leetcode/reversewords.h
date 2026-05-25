#pragma once

#include <string>

namespace cppprep::leetcode
{
  class SolutionRawPointer
  {
  public:
    char* reverseString(char* start, char* end, char* writer) {
      int i = 0, j = (end - start);
      while (i < j)
      {
        char temp = start[i];
        start[i] = start[j];
        start[j] = temp;
        i++; j--;
      }

      if (nullptr != writer)
      {
        int j = (end - start) + 1;
        for (int i=0; i < j; i++)
        {
          *writer++ = start[i];
        }
      }
      return writer;
    }

    char* reverseWords(char* s) {
      char* ps = s;
      char* pe = s + strlen(s)-1;
      char* writer = s;

      // Reverse the string
      reverseString(ps, pe, nullptr);

      // Reverse each word
      for (int i=0; i<strlen(s)-1; i++)
      {
        while (s[i] == '.' && s[i] != '\0') { i++;}
        ps = (s + i);
        while (s[i] != '.'  && s[i] != '\0') { i++;}
        pe = (s + i);
        writer = reverseString(ps, pe-1, writer);
        *writer++='.';
      }

      if (*(writer-1) == '.') { *(writer-1) = '\0'; }
      return s;
    }
  };

  class ReverseWordsSolution {
  public:
    int _marker{0};
    void reverseString(std::string& s, int start, int end, bool compress=false) {
      for (int i=start, j=end-1; i < j;)
      {
        std::swap(s[i++], s[j--]);
      }

      if (compress == true)
      {
        for (int i=start; i < end; )
        {
          s[_marker++] = s[i++];
        }
      }
    }

    std::string reverseWords(std::string &s) {

      // reverse the entire string
      std::reverse(s.begin(), s.end());

      int head{0}, tail = {0}, len = s.length();

      while (head < len)
      {
        // skip preceeding separator
        while ((tail < len) && (s[tail] == '.')) { tail++; }

        // get the end of the word
        head = tail;
        while ((head < len) && (s[head] != '.')) { head++; }

        if (tail < s.length())
        {
          reverseString(s, tail, head, true);
          s[_marker++] = '.';
          tail = head;
        }
      }
      s[_marker]='.';
      s.resize(_marker-1);
      return s;
    }
  };

  class ReverseWordsSolutionTest
  {

    public:
        void run_test() {
          ReverseWordsSolution rws;
          {
            ReverseWordsSolution rws;
            //char s[] = "i.like.this.program.very.much";
            std::string s{"i.like.this.program.very.much"};
            std::cout << rws.reverseWords(s) << std::endl;
          }

          {
            ReverseWordsSolution rws;
            std::string s{"..geeks..for.geeks."};
            std::cout << rws.reverseWords(s) << std::endl;
          }

          {
            ReverseWordsSolution rws;
            std::string s{"..home....."};
            std::cout << rws.reverseWords(s) << std::endl;
          }
        }
  };

  class SolutionRawPointerTest
  {
      public:
        void run_test_case() {
          SolutionRawPointer rws;
          {
            ReverseWordsSolution rws;
            //char s[] = "i.like.this.program.very.much";
            char s[] = {"i.like.this.program.very.much"};
            std::cout << rws.reverseWords(s) << std::endl;
          }

          {
            ReverseWordsSolution rws;
            char s[] = {"..geeks..for.geeks."};
            std::cout << rws.reverseWords(s) << std::endl;
          }

          {
            ReverseWordsSolution rws;
            char s[] = {"..home....."};
            std::cout << rws.reverseWords(s) << std::endl;
          }
        }
  };
}