#include <iostream>
#include <cassert>
#include <string>
#include <vector>

using namespace std;


void InBound(int &value, const int max_) {
  if (value == -1) {
    value = max_ - 1;
  } else if (value == max_) {
    value = 0;
  }
}

class Deque {
 public:
  //    Принимает число - максимально возможное количство элементов, одновременно
  //    находящихся в деке.
  //
  explicit Deque(const int MaxSize) {
    data_.assign(MaxSize, 0);
    size_ = 0;
    head_ = 0;
    tail_ = MaxSize - 1;
  }

  int Size() const {
    return size_;
  }
  int MaxSize() const {
    return data_.size();
  }

  bool IsEmpty() const {
    return size_ == 0;
  }
  bool IsFull() const {
    return size_ == MaxSize();
  }

  // Returns false if the operation is impossible
  bool PushBack(int value) {
    if (!IsFull()) {
      data_[head_] = value;
      head_++;
      InBound(head_, MaxSize());
      size_++;
      return true;
    } else {
      return false;
    }
  }

  int Back(int value = 0) const {
    if (!IsEmpty()) {
      int temp = head_ - 1;
      InBound(temp, MaxSize());
      return data_[temp];
    } else {
      return value;
    }
  }

  bool PopBack() {
    if (!IsEmpty()) {
      head_--;
      InBound(head_, MaxSize());
      size_--;
      return true;
    } else {
      return false;
    }
  }

  bool PushFront(int value) {
    if (!IsFull()) {
      data_[tail_] = value;
      tail_--;
      InBound(tail_, MaxSize());
      size_++;
      return true;
    } else {
      return false;
    }
  }
  
  int Front(int value = 0) const {
    if (!IsEmpty()) {
      int temp = tail_ + 1;
      InBound(temp, MaxSize());
      return data_[temp];
    } else {
      return value;
    }
  }

  bool PopFront() {
    if (!IsEmpty()) {
      tail_++;
      InBound(tail_, MaxSize());
      size_--;
      return true;
    } else {
      return false;
    }
  }

 private:
  vector<int> data_;
  int head_, tail_, size_;
};

class Queue {
 public:
  explicit Queue(const  int max_size) : data_(max_size) {}
 
  bool Push(int value) {
    return data_.PushBack(value);
  }
  
  bool Pop() {
    return data_.PopFront();
  }
  
  int Front(int _default = 0) const {
    return data_.Front(_default);
  }

  bool IsEmpty() const {
    return data_.Size() == 0;
  }
  
  bool IsFull() const {
    return data_.Size() == data_.MaxSize();
  }

  int Size() const {
    return data_.Size();
  }

  int MaxSize() const {
    return data_.MaxSize();
  }

 private:
  Deque data_;
};

class Stack {
 public:
  explicit Stack(const  int max_size) : data_(max_size) {}

  bool Push(int value) {
    return data_.PushBack(value);
  }

  bool Pop() {
    return data_.PopBack();
  }

  int Top(int _default = 0) const {
    return data_.Back(_default);
  }

  int Size() const {
    return data_.Size();
  }

  int MaxSize() const {
    return data_.MaxSize();
  }

  bool IsEmpty() const {
    return data_.IsEmpty();
  }

  bool IsFull() const {
    return data_.IsFull();
  }

 private:
  Deque data_;
};

#ifndef IGNORE_MAIN
int main() {
  {
    const Deque deque(5);
    assert(deque.IsEmpty());
    assert(!deque.IsFull());
    assert(deque.Size() == 0);
    assert(deque.MaxSize() == 5);
    assert(deque.Front() == 0);
    assert(deque.Front(0) == 0);
    assert(deque.Back() == 0);
    assert(deque.Back(0) == 0);
    cout << "PASSED: Deque > Const" << endl;
  }
  {
    Deque deque(5);

    assert(!deque.PopBack());

    assert(deque.Size() == 0);
    assert(deque.Back() == 0);
    assert(deque.Back(42) == 42);
    assert(deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PushBack(13));
    assert(deque.PushBack(123));
    assert(deque.PushBack(123456));

    assert(deque.Size() == 3);
    assert(deque.Back() == 123456);
    assert(deque.Back(42) == 123456);
    assert(!deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PopBack());

    assert(deque.Size() == 2);
    assert(deque.Back() == 123);
    assert(deque.Back(42) == 123);
    assert(!deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PushBack(3));
    assert(deque.PushBack(4));
    assert(deque.PushBack(5));

    assert(deque.Size() == 5);
    assert(deque.Back() == 5);
    assert(deque.Back(42) == 5);
    assert(!deque.IsEmpty());
    assert(deque.IsFull());

    assert(!deque.PushBack(99));

    assert(deque.Size() == 5);
    assert(deque.Back() == 5);
    assert(deque.Back(42) == 5);
    assert(!deque.IsEmpty());
    assert(deque.IsFull());

    cout << "PASSED: Deque > Back" << endl;
  }
  {
    Deque deque(5);

    assert(!deque.PopFront());

    assert(deque.Size() == 0);
    assert(deque.Front() == 0);
    assert(deque.Front(42) == 42);
    assert(deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PushFront(13));
    assert(deque.PushFront(123));
    assert(deque.PushFront(123456));

    assert(deque.Size() == 3);
    assert(deque.Front() == 123456);
    assert(deque.Front(42) == 123456);
    assert(!deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PopFront());

    assert(deque.Size() == 2);
    assert(deque.Front() == 123);
    assert(deque.Front(42) == 123);
    assert(!deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PushFront(3));
    assert(deque.PushFront(4));
    assert(deque.PushFront(5));

    assert(deque.Size() == 5);
    assert(deque.Front() == 5);
    assert(deque.Front(42) == 5);
    assert(!deque.IsEmpty());
    assert(deque.IsFull());

    assert(!deque.PushFront(99));

    assert(deque.Size() == 5);
    assert(deque.Front() == 5);
    assert(deque.Front(42) == 5);
    assert(!deque.IsEmpty());
    assert(deque.IsFull());

    cout << "PASSED: Deque > Front" << endl;
  }
  {
    Deque deque(3);

    assert(deque.PushFront(1));

    assert(deque.Size() == 1);
    assert(deque.Back() == 1);
    assert(deque.Front() == 1);

    assert(deque.PushBack(2));

    assert(deque.Size() == 2);
    assert(deque.Back() == 2);
    assert(deque.Front() == 1);

    assert(deque.PushFront(0));

    assert(deque.Size() == 3);
    assert(deque.Back() == 2);
    assert(deque.Front() == 0);

    assert(deque.PopFront());

    assert(deque.Size() == 2);
    assert(deque.Back() == 2);
    assert(deque.Front() == 1);

    assert(deque.PushBack(3));

    assert(deque.Size() == 3);
    assert(deque.Back() == 3);
    assert(deque.Front() == 1);

    assert(deque.PopFront());

    assert(deque.Size() == 2);
    assert(deque.Back() == 3);
    assert(deque.Front() == 2);

    assert(deque.PushBack(4));

    assert(deque.Size() == 3);
    assert(deque.Back() == 4);
    assert(deque.Front() == 2);

    assert(deque.PopBack());

    assert(deque.Size() == 2);
    assert(deque.Back() == 3);
    assert(deque.Front() == 2);

    cout << "PASSED: Deque > Mixed" << endl;
  }

  {
    Queue queue(2);
    assert(queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Size() == 0);
    assert(queue.MaxSize() == 2);
    assert(queue.Front() == 0);
    assert(queue.Front(0) == 0);
    cout << "PASSED: Queue > Const" << endl;
  }
  {
    Queue queue(2);

    assert(queue.Size() == 0);
    assert(queue.MaxSize() == 2);
    assert(queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Front() == 0);
    assert(queue.Front(42) == 42);

    assert(!queue.Pop());

    assert(queue.Size() == 0);
    assert(queue.MaxSize() == 2);
    assert(queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Front() == 0);
    assert(queue.Front(42) == 42);

    assert(queue.Push(1));

    assert(queue.Size() == 1);
    assert(queue.MaxSize() == 2);
    assert(!queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Front() == 1);
    assert(queue.Front(42) == 1);

    assert(queue.Push(2));

    assert(queue.Size() == 2);
    assert(queue.MaxSize() == 2);
    assert(!queue.IsEmpty());
    assert(queue.IsFull());
    assert(queue.Front() == 1);
    assert(queue.Front(42) == 1);

    assert(!queue.Push(3));

    assert(queue.Size() == 2);
    assert(queue.MaxSize() == 2);
    assert(!queue.IsEmpty());
    assert(queue.IsFull());
    assert(queue.Front() == 1);
    assert(queue.Front(42) == 1);

    assert(queue.Pop());

    assert(queue.Size() == 1);
    assert(queue.MaxSize() == 2);
    assert(!queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Front() == 2);
    assert(queue.Front(42) == 2);

    cout << "PASSED: Queue > Main" << endl;
  }
  {
    Stack stack(2);
    assert(stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Size() == 0);
    assert(stack.MaxSize() == 2);
    assert(stack.Top() == 0);
    assert(stack.Top(0) == 0);
    cout << "PASSED: Stack > Const" << endl;
  }
  {
    Stack stack(2);

    assert(stack.Size() == 0);
    assert(stack.MaxSize() == 2);
    assert(stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Top() == 0);
    assert(stack.Top(42) == 42);

    assert(!stack.Pop());

    assert(stack.Size() == 0);
    assert(stack.MaxSize() == 2);
    assert(stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Top() == 0);
    assert(stack.Top(42) == 42);

    assert(stack.Push(1));

    assert(stack.Size() == 1);
    assert(stack.MaxSize() == 2);
    assert(!stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Top() == 1);
    assert(stack.Top(42) == 1);

    assert(stack.Push(2));

    assert(stack.Size() == 2);
    assert(stack.MaxSize() == 2);
    assert(!stack.IsEmpty());
    assert(stack.IsFull());
    assert(stack.Top() == 2);
    assert(stack.Top(42) == 2);

    assert(!stack.Push(3));

    assert(stack.Size() == 2);
    assert(stack.MaxSize() == 2);
    assert(!stack.IsEmpty());
    assert(stack.IsFull());
    assert(stack.Top() == 2);
    assert(stack.Top(42) == 2);

    assert(stack.Pop());

    assert(stack.Size() == 1);
    assert(stack.MaxSize() == 2);
    assert(!stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Top() == 1);
    assert(stack.Top(42) == 1);

    cout << "PASSED: Stack > Main" << endl;
  }

  return 0;
}
#endif
