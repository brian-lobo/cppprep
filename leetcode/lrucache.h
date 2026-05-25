#pragma once

#include <string>
#include <vector>
#include <list>
#include <pair>


class LRUCache {
  // Get
  // Return value of key if the key exists otherwise return -1

  // put : Update value of the key if the key exists. Otherwise add key-value to the cache
  //       If the number of keys exceeds the capacity, evict the least recently used key

private:
  using BackEndCache = std::list<std::pair<int, int>>;
  using NodePointer = BackEndCache::iterator;
  BackEndCache _backendCache{};
  std::unordered_map<int, NodePointer> _cacheMap;
  int _capacity{0};

public:
  LRUCache(int capacity) : _capacity(capacity){

  }

  int get(int key) {
    auto val = _cacheMap.find(key);
    if (val == _cacheMap.end()) {
      return -1;
    }

    _backendCache.splice(_backendCache.begin(), _backendCache, val->second);
    return val->second->second;
  }

  void put(int key, int value) {
    auto itr = _cacheMap.find(key);
    if (itr != _cacheMap.end()) {
      itr->second->second = value;
      _backendCache.splice(_backendCache.begin(), _backendCache, itr->second);
      return;
    }

    if (_backendCache.size() == _capacity)
    {
      int evictionKey = _backendCache.back().first;
      _cacheMap.erase(evictionKey);
      _backendCache.pop_back();
    }

    _backendCache.emplace_front(key, value);
    _cacheMap.emplace(key, _backendCache.begin());
  }
};


class LRUCacheSolutionTest
{
public:
  void run_test() {
    {
      // std::vector command{"LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"};
      // std::vector<std::vector<int>> inputs{
      //   {2},
      //   {1, 1},
      //   {2, 2},
      //   {1},
      //   {3, 3},
      //   {2},
      //   {4, 4},
      //   {1},
      //   {3},
      //   {4}
      // };

      std::vector command{"LRUCache","put","put","get","put","get","put","get","get","get"};
      std::vector<std::vector<int>> inputs{
        {2},
        {1,0},
        {2,2},
        {1},
        {3,3},
        {2},
        {4,4},
        {1},
        {3},
        {4}
      };

      LRUCache cache{0};
      for (int i =0; i < inputs.size(); i++)
      {
        if (command[i] == "LRUCache"){
          cache = LRUCache{inputs[i][0]};
        } else if (command[i] == "put"){
          cache.put(inputs[i][0], inputs[i][1]);
        } else if (command[i] == "get")
        {
          cache.get(inputs[i][0]);
        }
      }
    }
  }

};