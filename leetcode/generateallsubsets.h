#pragma once

#include <iterator>
#include <vector>
#include <string>

namespace cppprep::leetcode
{
  class GenerateAllSubsetsSolution
  {
    public:

      std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
        std::vector<std::vector<int>> result;
        std::vector<int> path;
        backtrack(nums, 0, path, result);

        for (auto elt : result)
        {
          std::copy(elt.begin(), elt.end(), std::ostream_iterator<int>(std::cout, ","));
          std::cout << std::endl;
        }

        return result;
      }

    private:

      void backtrack(const std::vector<int>& nums, int start,
                     std::vector<int>& path, std::vector<std::vector<int>>& result) {
        // Base case
        result.push_back(path);   // copy current path

        for (int i=start; i< nums.size(); i++)
        {
          path.push_back(nums[i]);
          backtrack(nums, i + 1, path, result);
          path.pop_back();   // backtrack (undo)
        }
      }

      void backtrack1(const std::vector<int>& nums, int i,
                     std::vector<int>& path, std::vector<std::vector<int>>& result) {
        // Base case
        if (i == nums.size()) {
          result.push_back(path);   // copy current path
          return;
        }

        // Choice 1: Exclude nums[i]
        backtrack(nums, i + 1, path, result);

        // Choice 2: Include nums[i]
        path.push_back(nums[i]);
        backtrack(nums, i + 1, path, result);
        path.pop_back();   // backtrack (undo)
      }
  };

}