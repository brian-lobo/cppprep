#pragma once

#include <string>
#include <unordered_map>

namespace cppprep::leetcode
{
  class FractionToRecurringDecimalSolution {
  public:
    std::string fractionToDecimal(long long numerator, long long denominator) {
      if (numerator == 0) return "0";

      std::string result = "";

      // Handle negative sign
      bool negative = (numerator < 0) ^ (denominator < 0);
      if (negative) result += "-";

      // Make both positive
      numerator = abs(numerator);
      denominator = abs(denominator);

      // Integer part
      result += std::to_string(numerator / denominator);

      long long remainder = numerator % denominator;
      if (remainder == 0) return result;  // No decimal part

      result += ".";

      std::unordered_map<long long, int> remMap;  // remainder -> position
      std::string decimal = "";
      int pos = 0;

      while (remainder != 0) {
        if (remMap.find(remainder) != remMap.end()) {
          // Repeating starts
          int start = remMap[remainder];
          decimal.insert(start, "(");
          decimal += ")";
          break;
        }

        remMap[remainder] = pos++;

        remainder *= 10;
        decimal += std::to_string(remainder / denominator);
        remainder %= denominator;
      }

      result += decimal;
      return result;
    }
  };

}