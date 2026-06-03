#pragma once

#include <vector>

class CanJumpSolution {
public:
  bool canJump(std::vector<int>& nums) {

    int maxReach{0};
    for (int i=0; i <=maxReach && i < nums.size(); i++)
    {
      maxReach = std::max(maxReach, i + nums[i]);
      if (maxReach >= nums.size()-1)
        return true;
    }
    return false;
  }
};

class CanJumpSolutionTest
{
  public:
      void run_test() {
        {
          CanJumpSolution cjs;
          std::vector<int> nums = {2,3,1,1,4};
          std::cout << std::boolalpha << cjs.canJump(nums) << std::endl;
        }

        {
          CanJumpSolution cjs;
          std::vector<int> nums = {3,2,1,0,4};
          std::cout << std::boolalpha << cjs.canJump(nums) << std::endl;
        }
      }

};