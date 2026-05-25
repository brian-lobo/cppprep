#pragma once

#include <string>
#include <type_traits>
#include <memory>
#include <assert.h>
#include <atomic>
#include <expected>


namespace cppprep::synchronization
{
  template<typename T, size_t N>
  class SPSCQueue
  {
      static_assert(N>1, "Capacity must be atleast 2");

      const size_t CAPACITY{N};

      public:
          bool push_back(const T& val) noexcept
          {
            int w = _writerIdx.load(std::memory_order_relaxed);
            int r = _readerIdx.load(std::memory_order_acquire);


            if (((w+ 1) % CAPACITY) == r)
              return false;

            _buffer[w] = val;
            _writerIdx.store((w+1)%CAPACITY, std::memory_order_release);

            return true;
          }

          bool pop(T& val) noexcept {
            int r = _readerIdx.load(std::memory_order_relaxed);
            int w = _writerIdx.load(std::memory_order_acquire);

            if (r == w)
              return false;

            _readerIdx.store((r +1) % CAPACITY, std::memory_order_release);
            val = _buffer[r];
            return true;
          }

          bool empty() noexcept {
            return (_writerIdx.load(std::memory_order_acquire) ==
                    _readerIdx.load(std::memory_order_relaxed));
          }

          size_t full() const noexcept {
            size_t w = _writerIdx.load(std::memory_order_acquire);
            size_t r = _readerIdx.load(std::memory_order_relaxed);

            return (w >= r) ? (w - r) : (CAPACITY + w - r);
          }

      private:
          alignas(64) std::atomic<size_t>      _writerIdx{0};
          alignas(64) std::atomic<size_t>      _readerIdx{0};
          alignas(64) std::array<T, N>         _buffer{};
  };

  class SPSCQueueTest
  {
    public:
      void run_test() {
        SPSCQueue<int, 15> q;
        for (int i=0; i<25; i++)
        {
          if (q.push_back(i+1))
            std::cout << "pushed : " << i << std::endl;
        }

        while (q.empty() == false)
        {
          int num{0};
          q.pop(num);
          std::cout << "popped : " << num << std::endl;
        }
      }
  };

  /*
  template <typename T>
  class SPSCQueue {
  public:
    // give queue capacity+1 size so we can distinguish
    // queue empty from queue full
    // queue empty: mReadPos == mWritePos
    // queue full: mReadPos == (mWritePos+1) % queue_size
    SPSCQueue(int capacity) : mBuffer(capacity + 1), mReadPos(0),mWritePos(0) {
      assert(capacity > 0);
      assert(capacity + 1 > 0); // Protect from integer overflow

      std::cout << "mReadPos : " << offsetof(SPSCQueue<int>, mReadPos)
                << "; mWritePos : " << offsetof(SPSCQueue<int>, mWritePos)
                //<< "; _padding : " << offsetof(SPSCQueue<int>, _padding)
                << "; mBuffer : " << offsetof(SPSCQueue<int>, mBuffer)
                << std::endl;
    }

    // Queue Full - mReadPos == increment(mWritePos)
    bool push_back(const T &item) {
      const int r = mReadPos.load(std::memory_order_acquire);
      const int w = mWritePos.load(std::memory_order_relaxed);

      //check next write position to see if queue is full
      const int next_w = increment(w);
      if (r == next_w) return false;
      // store item if not full
      mBuffer[w] = item;
      mWritePos.store(next_w, std::memory_order_release);

      return true;
    }

    // queue empty: mReadPos == mWritePos
    bool pop_front(T &item) {
      int r = mReadPos.load(std::memory_order_relaxed);
      int w = mWritePos.load(std::memory_order_acquire);
      // Queue is empty (or was empty when we checked)
      if (r == w) return false;
      item = mBuffer[r];
      mReadPos.store(increment(r), std::memory_order_release);
      return true;
    }

  private:
    inline int increment(int pos) const {
      return (pos + 1) % int(mBuffer.size());
    }

    alignas(64) std::atomic<int> mReadPos; //cacheline size 64 bytes
    alignas(64) std::atomic<int> mWritePos;
    // char _padding[64 - sizeof(mWritePos)];
    alignas(64) std::vector<T> mBuffer;
  };
  */
}
