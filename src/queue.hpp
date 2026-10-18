#pragma once

#include <cstddef>
template<typename T, size_t N>
class Queue {
private:
  T elements[N];
  size_t head;
  size_t tail;
  size_t num_elements;

public:
  Queue<T, N>() : head(N - 1), tail(0), num_elements(0) {
  };

  void push(T value) {
    num_elements++;
    head++;
    if (head >= N) {
      head = 0;
    }
    elements[head] = value;
  }

  T pop() {
    T tail_value = elements[tail];
    num_elements--;
    tail++;
    if (tail >= N) {
      tail = 0;
    }
    return tail_value;
  }

  T& peek_head() {
    return elements[head];
  }

  T& peek_tail() {
    return elements[tail];
  }

  bool is_empty() {
    return num_elements == 0;
  }

  bool is_full() {
    return num_elements == N;
  }

  size_t size() {
    return num_elements;
  }

  void clear() {
    head = N - 1;
    tail = 0;
    num_elements = 0;
  }
};
