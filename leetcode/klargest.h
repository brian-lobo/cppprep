#pragma once

#include <vector>
#include <queue>

namespace cppprep::leetcode
{
  using namespace std;

  class KthLargestSolution {
  public:
    int findKthLargest(vector<int>& nums, int k) {

      std::priority_queue<int, std::vector<int>, std::greater<int>> q;

      for (int num : nums) {
        q.push(num);

        if (q.size() > k) {
          q.pop();
        }
      }
      return q.top();
    }
  };

  class KthLargestSolutionTest
  {
    public:
      void run_test() {
        std::vector<int> nums{ 3, 2, 1, 5, 6, 4 };
        int k{ 2 };

        Solution sol;
        sol.findKthLargest(nums, k);
      }
  };

}