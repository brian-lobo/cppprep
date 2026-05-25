#pragma once

#include <stack>

namespace cppprep::leetcode
{
  class MyQueue {
  private:
    std::stack<int> _queue{};
  public:
    MyQueue() {

    }

    void push_ex(int x) {
      if (_queue.empty()){
        _queue.push(x);
        return;
      }

      auto val = _queue.top();
      _queue.pop();
      push_ex(x);
      _queue.push(val);
    }

    void push(int x) {
      push_ex(x);
    }

    int pop() {
      int val = _queue.top();
      _queue.pop();
      return val;
    }

    int peek() {
      return _queue.top();
    }

    bool empty() {
      return _queue.empty();
    }
  };

  class MyQueueTest
  {
  public:
    
  };

}