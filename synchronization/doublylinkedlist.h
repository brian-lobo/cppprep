#pragma once
#include <atomic>
#include <utility>

template<typename T>
class DoublyLinkedList
{
  private:
    struct Node
    {
      explicit Node(T val) : data(std::move(val)) {}

      std::atomic<Node*> prev{nullptr};
      std::atomic<Node*> next{nullptr};
      T data;
    };

    alignas(64) std::atomic<Node*> head{nullptr};
    alignas(64) std::atomic<Node*> tail{nullptr};

  public:
    // Non-copyable for concurrent safety
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;
    DoublyLinkedList() = default;

    void push_back(T val) {
      Node* newNode = new Node(std::move(val));

      while (true)
      {
        Node* oldTail = tail.load(std::memory_order_acquire);

        if (oldTail == nullptr)
        {
          if (tail.compare_exchange_weak(oldTail, newNode,
                                         std::memory_order_acq_rel,
                                         std::memory_order_acquire))
          {
            head.store(newNode, std::memory_order_release);
            return;
          }
          continue; // tail changed, retry
        }

        newNode->prev.store(oldTail, std::memory_order_relaxed);

        Node* expectedNext = nullptr;
        if (oldTail->next.compare_exchange_strong(expectedNext, newNode,
                                                  std::memory_order_release,
                                                  std::memory_order_relaxed))
        {
          // Successfully linked -> swing the tail (Best effort)
          tail.compare_exchange_strong(oldTail, newNode, std::memory_order_acq_rel);
          return;
        }
      }
    }

    void pop_back() {
      while (true)
      {
        Node* oldTail = tail.load(std::memory_order_acquire);
        if (oldTail == nullptr) return; // Empty list

        Node* prevNode = oldTail->prev.load(std::memory_order_acquire);

        // Verify oldTail is still the actual tail (next must be nullptr)
        Node* expectedNext = nullptr;
        if (oldTail->next.compare_exchange_strong(expectedNext, nullptr,
                                                  std::memory_order_acq_rel,
                                                  std::memory_order_relaxed))
        {
          if (prevNode == nullptr) // It was the only node in the list
          {
            if (tail.compare_exchange_strong(oldTail, nullptr, std::memory_order_acq_rel))
            {
              head.store(nullptr, std::memory_order_release);
              // Note: In true concurrency, deleting here requires Hazard Pointers
              delete oldTail;
              return;
            }
          }
          else
          {
            // Cut the link from the previous node
            prevNode->next.store(nullptr, std::memory_order_release);

            // Attempt to move tail backwards
            if (tail.compare_exchange_strong(oldTail, prevNode, std::memory_order_acq_rel))
            {
              // Note: In true concurrency, deleting here requires Hazard Pointers
              delete oldTail;
              return;
            }
          }
        }
      }
    }

    ~DoublyLinkedList() {
      // Destructor runs single-threaded. No CAS or complex atomics needed.
      Node* curr = head.load(std::memory_order_relaxed);
      while (curr != nullptr)
      {
        Node* nextNode = curr->next.load(std::memory_order_relaxed);
        delete curr;
        curr = nextNode;
      }
      head.store(nullptr, std::memory_order_relaxed);
      tail.store(nullptr, std::memory_order_relaxed);
    }
};