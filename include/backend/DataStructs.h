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
    void push(const T& value);
    bool is_full();
    FixedQueue();
    FixedQueue(int maxLen);
};

template <typename T>
class FixedDeque : public std::deque<T> {
public:
    int maxLen;
    void push(const T& value);
    bool is_full();
    FixedDeque();
    FixedDeque(int maxLen);
};

template <typename T>
class FixedVec : public std::vector<T, std::vector<T>> {
public:
    int maxLen;
    void push_back(const T& value);
    bool is_full();
    FixedVec();
    FixedVec(int maxLen);
};

}

#endif