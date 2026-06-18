#pragma once

#include <algorithm>
#include <iterator>
#include <memory>

#include "allocator.h"

namespace cppprep::realworldproblems
{
  // template <typename T, typename Alloc = std::allocator<T>>
  template <typename T, typename Alloc = cache_aligned_allocator<T>>
  class vector_with_allocator
  {
  private:
    using AllocTraits = std::allocator_traits<Alloc>;

    Alloc _alloc;
    T* _buffer{nullptr};;
    std::size_t _size{0};
    std::size_t _capacity{0};

  public:
    using value_type = T;
    using allocator_type = Alloc;


    void push_back(const T& val) {
      if (_size == _capacity)
      {
        reserve(newCapacity());
      }

      AllocTraits::construct(_alloc, (_buffer + _size), val);
      _size++;
    }

    void reserve(std::size_t capacity)
    {
      T* newBuffer = AllocTraits::allocate(_alloc, capacity);

      int i {0};
      try
      {
        for ( ;i<_size; ++i)
        {
          AllocTraits::construct(_alloc, (newBuffer+i), _buffer[i]);
        }
      }
      catch (...)
      {
        releaseBuffer(newBuffer, i, capacity);
        throw;
      }

      releaseBuffer(_buffer, _size, _capacity);

      _buffer = newBuffer;
      _capacity = capacity;
    }



    ~vector_with_allocator() {
      releaseBuffer(_buffer, _size, _capacity);
    }

  private:
    std::size_t newCapacity() const {
      // return (_capacity == 0) ? 1 : _capacity * 2;
      std::size_t cap = (_capacity == 0) ? 1 : _capacity * 2;
      constexpr std::size_t cache_line = 64;
      std::size_t elements_per_line = cache_line / sizeof(T);
      if (elements_per_line > 1) {
        cap = ((cap + elements_per_line - 1) / elements_per_line) * elements_per_line;
      }
      return cap;

    }

    void releaseBuffer(T* buffer, std::size_t size, size_t capacity) {
      if (buffer == nullptr) return;

      std::for_each_n (buffer, size, [this](auto& obj) {
        AllocTraits::destroy(_alloc, std::addressof(obj));
      });
      AllocTraits::deallocate(_alloc, buffer, capacity);
    }



  };

  class vector_with_allocator_test
  {
    public:
      void run_test() {
        vector_with_allocator<int> vec;
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
        vec.push_back(4);

      }
  };


}