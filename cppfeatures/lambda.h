#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>
#include <atomic>
#include <cstring>
#include <map>
#include <set>
#include <functional>
#include <variant>
#include <type_traits>

namespace cppprep::cppfeatures
{
  template<typename T>
  bool transformMe(const T& obj) {
    std::cout << obj << std::endl;
    return true;
  }

  using TransformString = std::function<bool(const std::string& str)> ;
  using TransformDouble = std::function<bool(double d)>;
  using TransformFloat = std::function<bool(float f)>;
  using TransformInt = std::function<bool(int i)>;

  using TransformMe = std::variant<TransformString, TransformDouble, TransformFloat, TransformInt>;
  int main() {
    TransformString ts = transformMe<std::string>;
    TransformDouble td = transformMe<double>;
    TransformFloat tf = transformMe<float>;
    TransformInt ti = transformMe<int>;


    TransformMe funcs = transformMe<std::string>;
    std::vector<TransformMe> callbacks{ts, td, tf, ti};

    for (auto& callback : callbacks)
    {
      std::visit([&](auto&& func) {
        using FuncType = std::decay_t<decltype(func)>;

        if constexpr( std::is_same_v<TransformString, FuncType>)
        {
            std::string s{"hello world"};
            func(s);
        } else if constexpr ( std::is_same_v<TransformDouble, FuncType>){
          func(5.5);
        }else if constexpr ( std::is_same_v<TransformFloat, FuncType>) {
          func(6.6f);
        }else if constexpr ( std::is_same_v<TransformInt, FuncType>){
          func(5);
        }

      }, callback);
    }
  };
}