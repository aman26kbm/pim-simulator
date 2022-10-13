#include "backend/DataStructs.h"

using namespace pimsim;
/////////////////FixedQueue/////////////////

template <typename T>
FixedQueue<T>::FixedQueue(int maxLen){
    this->maxLen = maxLen;
}

template <typename T>
FixedQueue<T>::FixedQueue(){
    this->maxLen =2;
}

template <typename T>
void FixedQueue<T>::push(const T& value) {
    if (this->size() == maxLen) {
        assert(false);
    }
    std::queue<T, std::deque<T>>::push(value);
}
template <typename T>
bool FixedQueue<T>::is_full(){
    return (this->size() >= maxLen);
}

/////////////////FixedDeQue/////////////////

template <typename T>
FixedDeque<T>::FixedDeque(int maxLen){
    this->maxLen = maxLen;
}

template <typename T>
FixedDeque<T>::FixedDeque(){
    this->maxLen =2;
}

template <typename T>
void FixedDeque<T>::push(const T& value) {
    if (this->size() == maxLen) {
        assert(false);
    }
    std::deque<T>::push_back(value);
}
template <typename T>
bool FixedDeque<T>::is_full(){
    return (this->size() >= maxLen);
}
/////////////////FixedVec/////////////////

template <typename T>
FixedVec<T>::FixedVec(int maxLen){
    this->maxLen = maxLen;
}

template <typename T>
FixedVec<T>::FixedVec(){
    this->maxLen =2;
}

template <typename T>
void FixedVec<T>::push_back(const T& value) {
    if (this->size() == maxLen) {
        assert(false);
    }
    std::vector<T, std::vector<T>>::push_back(value);
}
template <typename T>
bool FixedVec<T>::is_full(){
    return (this->size() >= maxLen);
}