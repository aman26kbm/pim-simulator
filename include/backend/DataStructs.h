#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_
#include <queue>
#include <vector>
#include <assert.h>

namespace pimsim{

template <typename T>
class FixedQueue : public std::queue<T, std::deque<T>> {
public:
    int maxLen;

    void push(const T& value) {
      assert(this->size() < maxLen);
      std::queue<T, std::deque<T>>::push(value);
    }

    bool is_full() {
      return (this->size() >= maxLen);
    }

    FixedQueue(int maxLen = 2) {
      this->maxLen = maxLen;
    }
};

template <typename T>
class FixedDeque : public std::deque<T> {
public:
    int maxLen;

    FixedDeque(int maxLen = 2){
      this->maxLen = maxLen;
    }

    void push(const T& value) {
      if (this->size() == maxLen) {
        assert(false);
      }
      std::deque<T>::push_back(value);
    }

    bool is_full(){
      return (this->size() >= maxLen);
    }
};

template <typename T>
class FixedVec : public std::vector<T, std::vector<T>> {
public:
    int maxLen;

    FixedVec(int maxLen = 2) {
        this->maxLen = maxLen;
    }

    void push_back(const T& value) {
      if (this->size() == maxLen) {
        assert(false);
      }
      std::vector<T, std::vector<T>>::push_back(value);
    }

    bool is_full(){
      return (this->size() >= maxLen);
    }
};

}

#endif

