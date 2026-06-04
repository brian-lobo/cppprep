#pragma once

#include <atomic>
#include <memory>      // std::default_delete
#include <utility>
#include <cstddef>
#include <new>         // placement new
#include <type_traits>

namespace cppprep::smartpointers
{

    template<typename T>
    class CTCSharedPtr
    {
      private:

        struct ControlBlockBase
        {
          std::atomic<int32_t> refCount{1};
          std::atomic<int32_t> weakCount{1};
          T* managedPtr{nullptr};

          virtual ~ControlBlockBase() = default;
          virtual void destroy_self() = 0;
          virtual void destroy_managed_object() = 0;

          void attach() {
              refCount.fetch_add(1, std::memory_order_acq_rel);
          }

          virtual void release_shared() noexcept {
            if (refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
            {
                destroy_managed_object();
                release_weak();
            }
          }

          virtual void release_weak() noexcept {
              if (weakCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
              {
                  destroy_self();
              }
          }
        };

        struct ControlBlock : public ControlBlockBase
        {
            bool heapAllocation{false};
            alignas(T) std::byte object_storage[sizeof(T)];

            template<typename... Args>
            ControlBlock(Args&&... args) {
                this->managedPtr = new (object_storage) T{std::forward<Args>(args)...};
            }

            //template<typename U>
            ControlBlock(T* ptr) {
                heapAllocation = true;
                this->managedPtr = ptr;
            }

            virtual void destroy_self() noexcept {
                delete this;
            }

            virtual void destroy_managed_object() {
                if (this->managedPtr != nullptr)
                {
                    if (heapAllocation)
                    {
                        delete(this->managedPtr)
                    }
                    else
                    {
                        this->managedPtr->~T();
                    }
                    this->managedPtr = nullptr;
                }
            }
        };

        ControlBlockBase* _cb{nullptr};

        template<typename U, typename... Args>
        friend CTCSharedPtr<U> make_shared(Args&&... args);

        explicit CTCSharedPtr(ControlBlock* cb): _cb(cb) {
        }

      public:

        // Constructors
        CTCSharedPtr() = default;

        CTCSharedPtr(T* ptr) {
           _cb = new ControlBlock{ptr};
        }

        // Destructors
        ~CTCSharedPtr() {
            if (_cb != nullptr) {
                _cb->release_shared();
                _cb = nullptr;
            }
        }

        // Copy Constructor
        CTCSharedPtr(const CTCSharedPtr& obj) {
            _cb = obj._cb;
            if (_cb != nullptr)
              _cb->attach();
        }

        // Move Constructor
        CTCSharedPtr(CTCSharedPtr&& obj) {
            _cb = obj._cb;
            obj._cb = nullptr;
        }

        // Copy Assignment
        CTCSharedPtr& operator=(CTCSharedPtr& obj) {
            if (this != &obj)
            {
                if (_cb != nullptr){
                    _cb->release_shared();
                }
                _cb = obj._cb;
                if (_cb != nullptr)
                    _cb->attach();
            }
            return *this;
        }

        // Move Assignment
        CTCSharedPtr& operator=(CTCSharedPtr&& obj) {
            if (this != &obj)
            {
                if (_cb != nullptr){
                    _cb->release_shared();
                }
                _cb = obj._cb;
                obj._cb = nullptr;
            }
            return *this;
        }

        // *operator
        T& operator*() const {
            if ((_cb != nullptr) || (_cb->managedPtr ==  nullptr))
                throw runtime_error("Managed Pointer is null");
            return *_cb->managedPtr;
        }

        // -> operator
        T* operator ->() const {
            return (_cb)? _cb->managedPtr: nullptr;
        }
    };


    template<typename T, typename... Args>
    CTCSharedPtr<T> make_shared(Args&&... args) {
        std::cout << "Inside make_shared" << std::endl;

        using ControlBlock = typename CTCSharedPtr<T>::ControlBlock;
        ControlBlock* cb = new  ControlBlock{std::forward<Args>(args)...};
        CTCSharedPtr<T> p{cb};

        return p;
    }

    struct Order
    {
        int64_t price{222};
        int64_t quantity{444};
        int32_t side{1};
    };

    class SharedPtrTest
    {
        public:
              void run_test() {
                CTCSharedPtr<Order> optr{new Order{222, 444,1}};

                auto p0 = make_shared<Order>(222, 444, 1);
                auto p1 = make_shared<int>(42);
                auto p2 = make_shared<std::string>("Hello, optimized make_shared!");
                auto p3 = make_shared<std::vector<int>>(5, 100);
              }
    };
}