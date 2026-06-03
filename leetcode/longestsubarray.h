#pragma once

#include <string>
#include <deque>
#include <vector>

class LongestSubArraySolution {
public:

  // better implementation
  int longestSubarray(vector<int>& nums, int limit) {
    multiset<int> window;        // Keeps elements sorted
    int left = 0;
    int maxLen = 0;

    for (int right = 0; right < nums.size(); ++right) {
      window.insert(nums[right]);

      // Shrink window if max - min > limit
      while (*window.rbegin() - *window.begin() > limit) {
        window.erase(window.find(nums[left]));
        left++;
      }

      maxLen = max(maxLen, right - left + 1);
    }

    return maxLen;
  }

  int longestSubarray_1(vector<int>& nums, int limit) {
    int n = nums.size();
    int left = 0;
    int maxLength = 0;

    std::deque<int> maxDeque;  // Stores indices in decreasing order of values
    std::deque<int> minDeque;  // Stores indices in increasing order of values

    for (int right = 0; right < n; ++right) {

      // Maintain maxDeque (front has the index of maximum element)
      while (!maxDeque.empty() && nums[maxDeque.back()] <= nums[right]) {
        maxDeque.pop_back();
      }
      maxDeque.push_back(right);

      // Maintain minDeque (front has the index of minimum element)
      while (!minDeque.empty() && nums[minDeque.back()] >= nums[right]) {
        minDeque.pop_back();
      }
      minDeque.push_back(right);

      // Shrink the window from left if max - min > limit
      while (nums[maxDeque.front()] - nums[minDeque.front()] > limit) {
        left++;

        // Remove elements that are out of the current window
        if (!maxDeque.empty() && maxDeque.front() < left) {
          maxDeque.pop_front();
        }
        if (!minDeque.empty() && minDeque.front() < left) {
          minDeque.pop_front();
        }
      }

      maxLength = max(maxLength, right - left + 1);
    }

    return maxLength;
  }
};

class LongestSubarraySolutionTest
{
  public:
      void run_test() {
        LongestSubArraySolution sol;
        {
          std::vector<int> nums{8,2,4,7};
          int limit{4};

          sol.longestSubarray(nums, limit);
        }

        {
          std::vector<int> nums{10,1,2,4,7,2};
          int limit{5};
          sol.longestSubarray(nums, limit);
        }

        {
          std::vector<int> nums{4,2,2,2,4,4,2,2};
          int limit{0};
          sol.longestSubarray(nums, limit);
        }
      }
};