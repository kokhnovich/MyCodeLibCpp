#include <iostream>
#include <cassert>
#include <string>
#include <typeinfo>
#include <vector>
#include <utility>

using namespace std;

// #define SKIP_SINGLE

template<typename T>
class UniquePtr {
 public:
  explicit UniquePtr(T* value = nullptr) : obj(value) {}

  ~UniquePtr() {
    delete obj;
  }

  UniquePtr(const UniquePtr& other) = delete;

  UniquePtr& operator=(const UniquePtr& other) = delete;

  UniquePtr(UniquePtr&& other) noexcept {
    obj = other.obj;
    other.obj = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& other) noexcept {
    delete obj;
    obj = other.obj;
    other.obj = nullptr;
    return *this;
  }

  const T* Get() const {
    return obj;
  }

  T* Get() {
    return obj;
  }

  void Release() {
    obj = nullptr;
  }

  T& operator*() {
    return *obj;
  }

  T* operator->() {
    return obj;
  }

  const T& operator*() const {
    return *obj;
  }

  const T* operator->() const {
    return obj;
  }

 private:
  T* obj;
};

// #define SKIP_ARRAY

template<typename T>
class UniquePtr<T[]> {
 public:
  explicit UniquePtr(T* value = nullptr) : obj(value) {}

  ~UniquePtr() {
    delete[] obj;
  }

  UniquePtr(const UniquePtr& other) = delete;

  UniquePtr& operator=(const UniquePtr& other) = delete;

  UniquePtr(UniquePtr&& other) noexcept {
    obj = other.obj;
    other.obj = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& other) noexcept {
    delete[] obj;
    obj = other.obj;
    other.obj = nullptr;
    return *this;
  }

  const T* Get() const {
    return obj;
  }

  T* Get() {
    return obj;
  }

  void Release() {
    obj = nullptr;
  }

  T& operator*() {
    return *obj;
  }

  T* operator->() {
    return obj;
  }

  const T& operator*() const {
    return *obj;
  }

  const T* operator->() const {
    return obj;
  }

  const T& operator[](int value) const {
    return obj[value];
  }

  T& operator[](int value) {
    return obj[value];
  }

 private:
  T* obj;
};

#ifdef IGNORE_MAIN
int skipped_main() {
#else
int main() {
#endif  // IGNORE_MAIN
#ifndef SKIP_SINGLE
  {
    UniquePtr<int> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(typeid(i_ptr.Get()) == typeid(int*));
  }
  {
    const UniquePtr<int> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(typeid(i_ptr.Get()) == typeid(const int*));
  }
  {
    auto* i = new int(42);
    UniquePtr<int> i_ptr(i);
    assert(i_ptr.Get() == i);
    i_ptr.Release();
    assert(i_ptr.Get() == nullptr);
    delete i;
  }
  {
    auto* i = new int(42);
    UniquePtr<int> i_ptr(i);

    assert(*i_ptr == 42);

    *i = 123;
    assert(*i_ptr == 123);

    assert(typeid(*i_ptr) == typeid(int&));
    assert(typeid(i_ptr.operator->()) == typeid(int*));

    const auto& const_i_ptr = i_ptr;
    assert(typeid(*const_i_ptr) == typeid(const int&));
    assert(typeid(const_i_ptr.operator->()) == typeid(const int*));
  }
  {
    auto* s = new string("Hello, ");
    UniquePtr<string> s_ptr(s);

    assert(s_ptr->length() == 7);
    assert(*s_ptr == "Hello, ");

    *s_ptr += "world";
    assert(s_ptr->length() == 12);
    assert(*s_ptr == "Hello, world");

    *s += "!";
    assert(s_ptr->length() == 13);
    assert(*s_ptr == "Hello, world!");

    assert(typeid(*s_ptr) == typeid(string&));
    assert(typeid(s_ptr.operator->()) == typeid(string*));
  }
  {
    auto* s = new string("Hello");
    const UniquePtr<string> s_ptr(s);

    assert(s_ptr->length() == 5);
    assert(*s_ptr == "Hello");

    *s += "!";
    assert(s_ptr->length() == 6);
    assert(*s_ptr == "Hello!");

    assert(typeid(*s_ptr) == typeid(const string&));
    assert(typeid(s_ptr.operator->()) == typeid(const string*));
  }
  {
    auto* s = new vector<string>({"Hello", "World"});
    const UniquePtr<vector<string>> s_ptr(s);

    assert(s_ptr->size() == 2);
    assert((*s_ptr)[0] == "Hello");
    assert((*s_ptr)[1] == "World");

    assert(typeid((*s_ptr)[0]) == typeid(const string&));
  }
  {
    UniquePtr<string> ptr1(new string("42"));
    assert(*ptr1 == "42");

    UniquePtr<string> ptr2(move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(*ptr2 == "42");
  }
  {
    UniquePtr<string> ptr1(new string("42"));
    assert(*ptr1 == "42");

    UniquePtr<string> ptr2(new string("11111"));
    assert(*ptr1 == "42");
    assert(*ptr2 == "11111");

    ptr2 = std::move(ptr1);
    assert(ptr1.Get() == nullptr);
    assert(*ptr2 == "42");
  }
  {
    UniquePtr<string> ptr1(new string("42"));
    assert(*ptr1 == "42");

    UniquePtr<string> ptr2(new string("11111"));
    assert(*ptr1 == "42");
    assert(*ptr2 == "11111");

    UniquePtr<string> ptr3 = move(ptr2 = std::move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() == nullptr);
    assert(*ptr3 == "42");
  }
  cout << "OK: single object" << endl;
#else
  cout << "Skipped: single object" << endl;
#endif  // SKIP_SINGLE

#ifndef SKIP_ARRAY
  {
    UniquePtr<int[]> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(typeid(i_ptr.Get()) == typeid(int*));
  }
  {
    const UniquePtr<int[]> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(typeid(i_ptr.Get()) == typeid(const int*));
  }
  {
    auto* i = new int[42];
    UniquePtr<int> i_ptr(i);
    assert(i_ptr.Get() == i);
    i_ptr.
        Release();
    assert(i_ptr.Get() == nullptr);
    delete[]
        i;
  }
  {
    auto* i = new int[4];
    i[0] = 1;
    i[2] = 654321;

    UniquePtr<int[]> i_ptr(i);
    assert(*i_ptr == 1);
    assert(i_ptr[2] == 654321);

    i[2] = 11111;
    assert(i_ptr[2] == 11111);

    i_ptr[2] /= 100;
    assert(i_ptr[2] == 111);

    assert(typeid(*i_ptr) == typeid(int&));
    assert(typeid(i_ptr.operator->()) == typeid(int*));
    assert(typeid(i_ptr.operator[](2)) == typeid(int&));
  }
  {
    auto* s = new string[4];
    s[0] = "First";
    s[2] = "Hi ";

    UniquePtr<string[]> s_ptr(s);
    assert(*s_ptr == "First");
    assert(s_ptr[2] == "Hi ");

    s[2] += "World";
    assert(s_ptr[2] == "Hi World");

    s_ptr[2] += "!!!";
    assert(s_ptr[2] == "Hi World!!!");

    assert(typeid(*s_ptr) == typeid(string&));
    assert(typeid(s_ptr.operator->()) == typeid(string*));
    assert(typeid(s_ptr.operator[](2)) == typeid(string&));
  }
  {
    auto* i = new int[4];
    i[0] = 1;
    i[2] = 654321;

    const UniquePtr<int[]> i_ptr(i);
    assert(*i_ptr == 1);
    assert(i_ptr[2] == 654321);

    i[2] = 11111;
    assert(i_ptr[2] == 11111);

    assert(typeid(*i_ptr) == typeid(const int&));
    assert(typeid(i_ptr.operator->()) == typeid(const int*));
    assert(typeid(i_ptr.operator[](2)) == typeid(const int&));
  }
  {
    auto* s = new string[4];
    s[0] = "First";
    s[2] = "Hi ";

    const UniquePtr<string[]> s_ptr(s);
    assert(*s_ptr == "First");
    assert(s_ptr[2] == "Hi ");

    s[2] += "World";
    assert(s_ptr[2] == "Hi World");

    assert(typeid(*s_ptr) == typeid(const string&));
    assert(typeid(s_ptr.operator->()) == typeid(const string*));
    assert(typeid(s_ptr.operator[](2)) == typeid(const string&));
  }
  {
    UniquePtr<string[]> ptr1(new string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<string[]> ptr2(move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() != nullptr);
  }
  {
    UniquePtr<string[]> ptr1(new string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<string[]> ptr2(new string[2]);
    assert(ptr1.Get() != nullptr);
    assert(ptr2.Get() != nullptr);

    ptr2 = std::move(ptr1);
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() != nullptr);
  }
  {
    UniquePtr<string[]> ptr1(new string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<string[]> ptr2(new string[2]);
    assert(ptr1.Get() != nullptr);
    assert(ptr2.Get() != nullptr);

    UniquePtr<string[]> ptr3 = move(ptr2 = std::move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() == nullptr);
    assert(ptr3.Get() != nullptr);
  }
  cout << "OK: array" << endl;
#else
  cout << "Skipped: array" << endl;
#endif  // SKIP_ARRAY

  return 0;
}
