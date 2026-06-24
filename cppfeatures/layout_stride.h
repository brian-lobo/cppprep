#pragma once

namespace cppprep::cppfeatures
{

  class LayoutStride
  {
  public:
      void run_test() {
          // 1. Create a flat 1D array representing a 4x4 Matrix
          // Matrix indices:
          // [0,0]=10,  [0,1]=11,  [0,2]=12,  [0,3]=13
          // [1,0]=20,  [1,1]=21,  [1,2]=22,  [1,3]=23
          // [2,0]=30,  [2,1]=31,  [2,2]=32,  [2,3]=33
          // [3,0]=40,  [3,1]=41,  [3,2]=42,  [3,3]=43
          std::vector<double> data = {
              10, 11, 12, 13,
              20, 21, 22, 23,
              30, 31, 32, 33,
              40, 41, 42, 43
          };

          // 2. We want to view a 2x2 sub-matrix starting at row 1, col 1:
          // [21, 22]
          // [31, 32]

          // Define the dimensions (extents) of our sub-matrix: 2 rows, 2 columns
          using SubExtents = std::extents<size_t, 2, 2>;

          // Define the custom strides:
          // To move down 1 row in the original matrix, we must jump 4 elements.
          // To move right 1 column, we jump 1 element.
          std::array<size_t, 2> strides{ 4, 1 };

          // Create the layout mapping
          std::layout_stride::mapping<SubExtents> sub_map(SubExtents{}, strides);

          // 3. Bind the layout to the pointer starting at data[5] (which is '21')
          auto sub_view = std::mdspan<double, SubExtents, std::layout_stride>(
              &data[5],
              sub_map
          );

          // 4. Access it naturally as a 2x2 matrix
          std::cout << "Sub-view [0, 0]: " << sub_view[0, 0] << "\n"; // Outputs 21
          std::cout << "Sub-view [0, 1]: " << sub_view[0, 1] << "\n"; // Outputs 22
          std::cout << "Sub-view [1, 0]: " << sub_view[1, 0] << "\n"; // Outputs 31
          std::cout << "Sub-view [1, 1]: " << sub_view[1, 1] << "\n"; // Outputs 32
      }
  };


}
