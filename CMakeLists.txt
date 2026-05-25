cmake_minimum_required(VERSION 4.2)
project(cppprep)

set(CMAKE_CXX_STANDARD 26)

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
        leetcode/longestsubarray.h)
