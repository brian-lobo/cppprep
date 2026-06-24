#pragma once

#include <iostream>
#include <thread>
#include <pthread.h>
#include <sched.h>
#include <random>

namespace cppprep::synchronization
{

  class CpuPinning
  {

      int generateRandom(int start, int end) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(start, end);

        return dis(gen);
      }

      // Function that handles ultra-low latency market data parsing
      void marketDataConsumerLoopUsingList() {
        // 1. Create a CPU set
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(2, &cpuset); // We want to pin this thread to Core 2



        // 2. Apply the CPU affinity mask to the current thread
        pthread_t current_thread = pthread_self();
        int rc = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);

        if (rc != 0) {
          std::cerr << "Error setting thread affinity to Core 2!" << std::endl;
          return;
        }

        std::list<int> items;
        for (int i{0}; i< 10000000; i++)
        {
          items.push_back(i);
        }

        std::cout << "Market Data Thread successfully pinned to Core 2!" << std::endl;

        // 3. The Hot Loop (Busy-waiting/polling for network packets)
        // int iCnt{0};
        // while (iCnt < 10000000) {
        //   // Line-rate network polling logic goes here...
        //   // Because it's pinned, L1/L2 data cache remains 100% "warm"
        //   int idx{generateRandom(1, 10000000)};
        //   std::erase_if(items, [idx](int i) {
        //     return i == idx;
        //   });
        // }

        std::cout << "Completed marketDataConsumerLoopUsingList" << std::endl;
      }

      // Function that handles ultra-low latency market data parsing
      void marketDataConsumerLoopUsingVector() {
        // 1. Create a CPU set
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(3, &cpuset); // We want to pin this thread to Core 2

        // 2. Apply the CPU affinity mask to the current thread
        pthread_t current_thread = pthread_self();
        int rc = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);

        if (rc != 0) {
          std::cerr << "Error setting thread affinity to Core 3!" << std::endl;
          return;
        }

        alignas(64) std::vector<int> items;
        for (int i{0}; i< 10000000; i++)
        {
          items.emplace_back(i);
        }

        std::cout << "Market Data Thread successfully pinned to Core 3!" << std::endl;

        // 3. The Hot Loop (Busy-waiting/polling for network packets)
        // int iCnt{0};
        // while (iCnt < 10000000) {
        //   // Line-rate network polling logic goes here...
        //   // Because it's pinned, L1/L2 data cache remains 100% "warm"
        //   int idx{generateRandom(1, 10000000)};
        //
        //   std::erase_if(items, [idx](int i) {
        //     return i == idx;
        //   });
        // }
        std::cout << "Completed marketDataConsumerLoopUsingVector" << std::endl;
      }

    public:
      void run_test()
      {
        // Spawn the critical thread
        std::thread marketDataThreadList(&CpuPinning::marketDataConsumerLoopUsingList, *this);
        std::thread marketDataThreadVector(&CpuPinning::marketDataConsumerLoopUsingVector, *this);

        // Keep main running on a default OS core (e.g. Core 0)
        marketDataThreadList.join();
        marketDataThreadVector.join();
      }
  };


}