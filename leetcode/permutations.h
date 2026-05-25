#pragma once
#include <vector>
#include <bits/exception_ptr.h>

namespace cppprep::leetcode
{
  class PermutationsSolution
  {
  public:
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
      std::vector<std::vector<int>> result;
      std::vector<int> path;
      std::vector<bool> used(nums.size(), false);

      backtrack(nums, path, used, result);
      return result;
    }

  private:
    void backtrack(const std::vector<int>& nums,
                   std::vector<int>& path,
                   std::vector<bool>& used,
                   std::vector<std::vector<int>>& result) {

      // Base case: we have a complete permutation
      if (path.size() == nums.size()) {
        result.push_back(path);   // copy the current permutation
        return;
      }

      // Try every number that hasn't been used yet
      for (int i = 0; i < nums.size(); ++i) {
        if (used[i]) continue;

        // Choose nums[i]
        used[i] = true;
        path.push_back(nums[i]);

        // Recurse to build the next position
        backtrack(nums, path, used, result);

        // Backtrack (undo choice)
        path.pop_back();
        used[i] = false;
      }
    }

    // decltype(auto) permute(std::vector<int>& nums) {
    //   std::vector<std::vector<int>> result;
    //   backtrack(nums, 0, result);
    //
    //   return (result);
    // }

    // void backtrack(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result) {
    //   // Goal Reached
    //   if (start == nums.size())
    //   {
    //     result.push_back(nums);
    //     return;
    //   }
    //
    //   for (int i=start; i<nums.size(); i++)
    //   {
    //     // Swap the current element with the element at the 'start' index
    //     std::swap(nums[start], nums[i]);
    //
    //     // Move to the next index to place the next element
    //     backtrack(nums, start + 1, result);
    //
    //     // Backtrack: swap back to restore the original array store
    //     std::swap(nums[start], nums[i]);
    //   }
    // }
  };
}