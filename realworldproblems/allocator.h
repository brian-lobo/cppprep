#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>
#include <memory>   // for std::addressof (optional)



namespace cppprep::realworldproblems
{
    template <typename T, std::size_t Alignment = 64>
    class cache_aligned_allocator
    {
    public:
        using value_type                             = T;
        using size_type                              = std::size_t;
        using difference_type                        = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal                        = std::true_type;

        template <typename U>
        struct rebind {
            using other = cache_aligned_allocator<U, Alignment>;
        };

        constexpr cache_aligned_allocator() noexcept = default;

        template <typename U>
        constexpr cache_aligned_allocator(const cache_aligned_allocator<U, Alignment>&) noexcept {}

        [[nodiscard]] T* allocate(std::size_t n) {
            if (n == 0) {
                return nullptr;
            }

            const std::size_t bytes = n * sizeof(T);

            // Round up to multiple of Alignment (required by aligned_alloc)
            const std::size_t aligned_bytes =
                ((bytes + Alignment - 1) / Alignment) * Alignment;

            void* ptr = std::aligned_alloc(Alignment, aligned_bytes);
            if (!ptr) {
                throw std::bad_alloc();
            }

            return static_cast<T*>(ptr);
        }

        void deallocate(T* p, std::size_t /*n*/) noexcept {
            if (p) {
                std::free(p);
            }
        }

        // Optional: equality operators (stateless allocator)
        template <typename U, std::size_t A>
        friend constexpr bool operator==(const cache_aligned_allocator<T, Alignment>&,
                                         const cache_aligned_allocator<U, A>&) noexcept {
            return true;
        }

        template <typename U, std::size_t A>
        friend constexpr bool operator!=(const cache_aligned_allocator<T, Alignment>&,
                                         const cache_aligned_allocator<U, A>&) noexcept {
            return false;
        }
    };



}
