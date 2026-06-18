#pragma once

namespace cppprep::leetcode
{
  class MaxSlidingWindowSolution {
  public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
      vector<int> result;
      deque<int> dq;        // Stores indices in decreasing order of values

      for (int i = 0; i < nums.size(); ++i) {

        // Step 1: Remove elements out of current window
        if (!dq.empty() && dq.front() == i - k) {
          dq.pop_front();
        }

        // Step 2: Remove smaller elements from back (they can't be max anymore)
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
          dq.pop_back();
        }

        // Step 3: Add current index
        dq.push_back(i);

        // Step 4: Add maximum to result when window is full
        if (i >= k - 1) {
          result.push_back(nums[dq.front()]);
        }
      }
      
      return result;
    }
  };

  class MaxSlidingWindowSolutionTest
  {
    public:
      void run_test() {
        MaxSlidingWindowSolution s;

        {
          std::vector<int> nums{ 1,3,-1,-3,5,3,6,7 };
          int k{ 3 };

          s.maxSlidingWindow(nums, k);
        }
      }
  };
}