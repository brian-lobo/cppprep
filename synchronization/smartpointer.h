#pragma once

#include <atomic>
#include <iostream>
#include <ostream>
#include <memory>
#include <utility>

namespace synchronization
{
  template<typename T>
  class CustomSharedPointer
  {
  private:
    struct ControlBlock
    {
      using Reference = T&;
      using Pointer   = T*;

      T* _ptr{nullptr};
      std::atomic<int> _refCount{0};
      std::atomic<int> _weakCount{0};

      template< typename ... Args>
      explicit ControlBlock(Args ... args) : _ptr(new T(std::forward<Args>(args)...)),
                                             _refCount(1),
                                             _weakCount(0) {
      }

      void increment() {
        _refCount.fetch_add(1, std::memory_order_acq_rel);
      }

      void decrement() {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
          delete _ptr;
          delete this;
        }
      }



    };

    ControlBlock* _controlBlock{nullptr};


  public:
    template<typename ... Args>
    explicit CustomSharedPointer(Args ... args) : _controlBlock(new ControlBlock(args...)) {
    }

    ~CustomSharedPointer() {
      if (_controlBlock)
        _controlBlock->decrement();
    }

    CustomSharedPointer(const CustomSharedPointer& other) : _controlBlock(other._controlBlock) {
      if (_controlBlock)
        _controlBlock->increment();
    }

    CustomSharedPointer& operator = (const CustomSharedPointer& other) {
      if (&other != this)
      {
        if (_controlBlock)
          _controlBlock->decrement();

        _controlBlock = other._controlBlock;
        if (_controlBlock)
          _controlBlock->increment();
      }
      return *this;
    }

    CustomSharedPointer (CustomSharedPointer&& other) : _controlBlock(other._controlBlock) {
      if (_controlBlock) _controlBlock->increment();
    }

    CustomSharedPointer& operator = (const CustomSharedPointer&& other) {
      if (&other != this)
      {
        if (_controlBlock)
          _controlBlock->decrement();

        _controlBlock = std::exchange(other._controlBlock, nullptr);
      }
      return *this;
    }

    [[nodiscard]] T& operator*() const noexcept{
      return *(_controlBlock->_ptr);
    }

    [[nodiscard]] T* operator->() const noexcept{
      return _controlBlock->_ptr;
    }
  };



  struct Message
  {
    unsigned int messageType{0};
    std::string content{""};

    friend std::ostream& operator << (std::ostream&, const Message&);
  };

  std::ostream& operator << (std::ostream& os, const Message& msg)
  {
    os << msg.content << " ; " << msg.messageType << std::endl;
    return os;
  }


  class CustomSharedPointerTest
  {
  public:
    void run_test() {
      // Shared Pointer holding an integer
      CustomSharedPointer<int> sp(1);
      std::cout << *sp << std::endl;

      CustomSharedPointer<Message> sp1(1u, "FixMessage");
      std::cout << *sp1 << std::endl;
      std::cout << "Content : " << sp1->content
                << " ;MessageType : " << std::endl;
    }
  };
}