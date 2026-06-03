#pragma once

#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

namespace cppprep::leetcode
{
  class FindMedianOfTwoSortedArraysSolution {
    public:
      double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // Ensure nums1 is the smaller array
        if (nums1.size() > nums2.size()) {
          return findMedianSortedArrays(nums2, nums1);
        }

        int m = nums1.size();
        int n = nums2.size();
        int left = 0, right = m;

        while (left <= right) {
          int partition1 = (left + right) / 2;        // elements from nums1
          int partition2 = (m + n + 1) / 2 - partition1; // elements from nums2

          // Handle edge cases with INT_MIN and INT_MAX
          int left1  = (partition1 == 0) ? INT_MIN : nums1[partition1 - 1];
          int right1 = (partition1 == m) ? INT_MAX : nums1[partition1];

          int left2  = (partition2 == 0) ? INT_MIN : nums2[partition2 - 1];
          int right2 = (partition2 == n) ? INT_MAX : nums2[partition2];

          // Check if partition is correct
          if (left1 <= right2 && left2 <= right1) {
            // Found correct partition
            if ((m + n) % 2 == 0) {
              return (max(left1, left2) + min(right1, right2)) / 2.0;
            } else {
              return max(left1, left2);
            }
          }
          else if (left1 > right2) {
            right = partition1 - 1;   // Too many elements from nums1
          }
          else {
            left = partition1 + 1;    // Need more elements from nums1
          }
        }

        return 0.0; // Should never reach here
      }
  };

  class FindMedianOfTwoSortedArraysSolutionTest
  {
    public:
      void run_test() {

        FindMedianOfTwoSortedArraysSolution sol;

        {
          std::vector<int> nums1{1,3};
          std::vector<int> nums2{2};

          sol.findMedianSortedArrays(nums1, nums2);
        }


        {
          std::vector<int> nums1{1,2};
          std::vector<int> nums2{3,4};

          sol.findMedianSortedArrays(nums1, nums2);
        }
      }
  };

}



// #include <iostream>
// #include <algorithm>
// #include <vector>
// #include <cmath>

// class Solution {
// public:
//   double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
//     int p1{0}, p2{0};
//     int cnt{0};
//     float sum{0.0};
//     int mid1{0}, mid2{-1};
//     size_t sampleSize{(nums1.size() + nums2.size())};
//     bool isOdd = sampleSize & 1u;
//     if (isOdd){
//       mid1 = sampleSize/2;
//     } else {
//       mid2 = sampleSize/2;
//       mid1 = mid2 - 1;
//     }
//
//     std::cout << std::boolalpha << isOdd << "; mid1: " << mid1 << "; mid2: " << mid2 << std::endl;
//
//     while (true){
//       int val{0};
//       if (p1 >= nums1.size()) {
//         val = nums2[p2];
//         p2++;
//       } else if (p2 >= nums2.size()){
//         val = nums1[p1];
//         p1++;
//       } else {
//         if (nums1[p1] <= nums2[p2]){
//           val = nums1[p1];
//           p1++;
//         } else {
//           val = nums2[p2];
//           p2++;
//         }
//       }
//
//       if (isOdd && cnt == mid1){
//         return val;
//       }
//
//       if (!isOdd && (cnt == mid1) || (cnt == mid2)){
//         sum +=val;
//         if (cnt == mid2){
//           std::cout << "sum: "           << sum
//                     << "; sum/2: "       << (sum / 2.0f)
//                     << "; (sum % 2)/2: " << (std::fmod(sum,2.0f)/2.0f)
//                     << std::endl;
//           return sum/2 + std::fmod(sum,2.0f)/2;
//         }
//       }
//
//       cnt++;
//     }
//     return 0;
//   }
// };