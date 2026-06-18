#pragma once

#include <memory>
#include <cstdint>
#include <new>
#include <iostream>


namespace cppprep::realworldproblems
{

  struct foo
  {
    int index{0};
    foo(int i): index{i} {
      std::cout << "Inside the constructor of " << index << std::endl;
    }

    foo(const foo& obj) : index{obj.index} {
      std::cout << "Inside the copy constructor of " << index  << std::endl;
    }

    ~foo() {
      std::cout << "Inside the destructor of " << index  << std::endl;
    }
  };


  template<typename T>
  class Vector
  {
    public:
      using size_type = std::size_t;
      using pointer = T*;
      using reference = T&;
      using const_pointer = const T*;
      using const_reference = const T&;

      void push_back(const T& obj) {
        if (_size == _capacity) {
          reserve(getNewCapacity());
        }

        new(_buffer + _size) T(obj);
        _size++;
      }

      void reserve (size_type capacity)
      {
        T* newBufferPtr  = static_cast<T*>(::operator new (capacity * sizeof(T), std::align_val_t{CACHE_ALIGNMENT}));

        // Copy the items from old buffer into the new buffer
        int constructed{0};
        try
        {
          for ( ;constructed<_size; ++constructed)
          {
            new(newBufferPtr + constructed) T(_buffer[constructed]);
          }
        }
        catch (...)
        {
          // Rollback the changes i.e. destroy the copied objects and free the buffer
          for (int idx{0}; idx<constructed; ++idx)
          {
            (newBufferPtr + idx)->~T();
          }
          ::operator delete(newBufferPtr, std::align_val_t{CACHE_ALIGNMENT});
          throw;
        }

        // release the old buffer
        for (int idx{0}; idx<_size; ++idx)
        {
          (_buffer + idx)->~T();
        }

        if (_buffer)
        {
          ::operator delete(_buffer, std::align_val_t(CACHE_ALIGNMENT));
        }
        _capacity = capacity;
        _buffer = newBufferPtr;

      }

      ~Vector() {
        for (size_type i = 0; i < _size; ++i) {
          (_buffer + i)->~T();
        }
        ::operator delete(_buffer, std::align_val_t(CACHE_ALIGNMENT));
        _buffer = nullptr;
        _size = 0;
        _capacity = 0;
      }


      private:

        size_type getNewCapacity() {
          return (_capacity == 0) ? 1 : _capacity * 2;
        }

        T* _buffer{nullptr};
        size_type _size{0};
        size_type _capacity{0};
        size_t CACHE_ALIGNMENT{64};
  };


  class  VectorTest
  {
    public:
      void run_test()
      {
        Vector<foo> vec{};
        vec.push_back(foo{1});
        vec.push_back(foo{2});
        vec.push_back(foo{3});
      }
  };
}