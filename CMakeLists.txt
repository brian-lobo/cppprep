cmake_minimum_required(VERSION 4.2)
project(cppprep)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE INTERNAL "")



add_executable(cppprep main.cpp
        cppfeatures/structuredbinding.h
        CommonIncludes.h
        cppfeatures/tuple.h
        cppfeatures/invoke.h
        cppfeatures/constexpr.h
        synchronization/SPSCQueue.h
        sockets/epoll.h
        smartpointers/Wallet.h
        leetcode/permutations.h
        leetcode/wordsearch.h
        leetcode/lettercombinationsofaphonenumber.h
        leetcode/generateallsubsets.h
        cppfeatures/lambda.h
        leetcode/validsudoku.h
        leetcode/textjustification.h
        leetcode/reversewords.h
        leetcode/samekeycard.h
        leetcode/playerswithzeroonelosses.h
        leetcode/canjump.h
        leetcode/lrucache.h
        leetcode/lengthoflongeststring.h
        leetcode/issubsequence.h
        leetcode/queueusingstacks.h
        leetcode/subsets.h
        leetcode/longestsubarray.h
        metaprogramming/gcd.h
        synchronization/doublylinkedlist.h
        synchronization/smartpointer.h
        designpatterns/behavioural/strategy.h
        designpatterns/behavioural/crtp.h
        designpatterns/structural/decorator.h
        leetcode/longestpalindromesubstring.h
        leetcode/openthelock.h
        leetcode/fractiontorecurringdecimal.h
        realworldproblems/OrderBook.h
        leetcode/trappingrainwater.h
        leetcode/medianoftwosortedarrays.h
        smartpointers/smartpointer.h
        leetcode/dfs/numberofislands.h
        leetcode/slidingwindow/maxslidingwindow.h
        leetcode/dfs/nqueens.h
        leetcode/klargest.h
        realworldproblems/OrderBook.cpp
        realworldproblems/vector_impl.h
        realworldproblems/vector_with_allocator.h
        realworldproblems/allocator.h)

target_compile_options(cppprep PRIVATE -O0 -g)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(cppprep PRIVATE -stdlib=libc++)
    target_link_options(cppprep PRIVATE -stdlib=libc++)
endif()