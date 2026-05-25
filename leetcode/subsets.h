#pragma once

#include <string>
#include <vector>


class SubsetsSolution
{
  public:
    using Result = std::vector<std::vector<int>>;

    void backtrack(std::vector<int>& nums, int start, std::vector<int>& path ,Result& result) {

      result.push_back(path);

      for (int i =start,i < nums.size(); ++i)
      {
        path.push_back(nums[i]);
        backtrack(nums, i+1, path, result);
        path.pop_back();
      }
      return;
    }

    Result subsets(std::vector<int>& nums) {
      Result result;
      std::vector<int> path;

      backtrack(nums, 0, path, result );
      return result;
    }
};

class SubsetsSolutionTest
{
  public:
    void run_test() {
      SubsetsSolution ss;

      std::vector<int> nums{ 1,2,3 };
      ss.subsets(nums);
    }
};