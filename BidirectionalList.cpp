#include <bits/stdc++.h>

using namespace std;

template<typename T>
class BiDirectionalList {
 protected:
  struct Node;

 public:
  class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    T& operator*() const {
      try {
        if (node_ == nullptr) throw logic_error("nullptr");
        return node_->value_;
      } catch (logic_error&) {
        cerr << "nullptr&" << endl;
        return *(new T);
      }
    }
    T* operator->() const {
      try {
        if (node_ == nullptr)
          return node_->value_;
      } catch (logic_error&) {
        cerr << "nullptr*" << endl;
      }
    }

    Iterator& operator++() {
      try {
        if (node_ == nullptr) throw logic_error("increment nullptr");
        node_ = node_->next_node_;
      } catch (logic_error&) {
        cerr << "incremented nullptr" << endl;
      }
      return *this;
    }

    Iterator& operator+(int value) {
      while (value-- && node_ != nullptr) {
        ++(*this);
      }
      return *this;
    }
    Iterator& operator-(int value) {
      while (value--) {
        this->operator--();
      }
      return *this;
    }

    const Iterator operator++(int) {
      auto original = *this;
      ++(*this);
      return original;
    }

    Iterator& operator--() {
      if (node_ == nullptr) {
        node_ = list_->last_;
      } else {
        node_ = node_->previous_node_;
      }
      return *this;
    }
    const Iterator operator--(int) {
      auto original = *this;
      --(*this);
      return *this;
    }

    bool operator==(const Iterator& other) const {
      return this->node_ == other.node_;
    }
    bool operator!=(const Iterator& other) const {
      return !this->operator==(other);
    }

   private:
    friend class BiDirectionalList;

    const BiDirectionalList* const list_;
    Node* node_;

    Iterator(const BiDirectionalList* const list, Node* node)
        : list_(list), node_(node) {}
  };

  class ConstIterator :
      public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    const T& operator*() const {
      return node_->value_;
    }
    const T* operator->() const {
      return node_->value_;
    }

    ConstIterator& operator++() {
      node_ = node_->next_node_;
      return *this;
    }
    const ConstIterator operator++(int) {
      auto original = *this;
      ++(*this);
      return *this;
    }

    ConstIterator& operator--() {
      node_ = node_->previous_node_;
      return *this;
    }
    const ConstIterator operator--(int) {
      auto original = *this;
      --(*this);
      return *this;
    }

    bool operator==(const ConstIterator& other) const {
      return node_ == other.node_;
    }
    bool operator!=(const ConstIterator& other) const {
      return !this->operator==(other);
    }

   private:
    friend class BiDirectionalList;

    const BiDirectionalList* const list_;
    const Node* node_;

    ConstIterator(const BiDirectionalList* const list, Node* node)
        : list_(list), node_(node) {}
  };

  BiDirectionalList() : first_(nullptr), last_(nullptr) {}

  ~BiDirectionalList() { Clear(); }

  bool IsEmpty() const {
    return first_ == nullptr && last_ == nullptr;
  }

  void Clear() {
    while (first_ != nullptr) {
      Erase(first_);
    }
  }

  Iterator begin() {
    return Iterator(this, first_);
  }
  Iterator end() {
    return Iterator(this, nullptr);
  }

  ConstIterator begin() const {
    return ConstIterator(this, first_);
  }
  ConstIterator end() const {
    return ConstIterator(this, nullptr);
  }

  std::vector<T> AsArray() const {
    vector<T> ans;
    Node* current = first_;
    while (current != nullptr) {
      ans.push_back(current->value_);
      current = current->next_node_;
    }
    return ans;
  }

  /*
   *  inserts work only after/before existing element
   *  nullptr cause logic_error
   */
  void InsertBefore(Iterator position, const T& value) {
    InsertBefore(position.node_, new Node(value));
  }
  void InsertBefore(Iterator position, T&& value) {
    InsertBefore(position.node_, new Node(value));
  }

  void InsertAfter(Iterator position, const T& value) {
    InsertAfter(position.node_, new Node(value));
  }
  void InsertAfter(Iterator position, T&& value) {
    InsertAfter(position.node_, new Node(value));
  }

  void PushBack(const T& value) {
    if (last_ == nullptr) {
      last_ = new Node(value);
      first_ = last_;
    } else {
      InsertAfter(last_, new Node(value));
    }
  }
  void PushBack(T&& value) {
    if (last_ == nullptr) {
      last_ = new Node(value);
      first_ = last_;
    } else {
      InsertAfter(last_, new Node(value));
    }
  }

  void PushFront(const T& value) {
    if (first_ == nullptr) {
      last_ = new Node(value);
      first_ = last_;
    } else {
      InsertBefore(first_, new Node(value));
    }
  }
  void PushFront(T&& value) {
    if (first_ == nullptr) {
      last_ = new Node(value);
      first_ = last_;
    } else {
      InsertBefore(first_, new Node(value));
    }
  }

  void Erase(Iterator position) {
    Erase(position.node_);
  }

  void PopFront() {
    Erase(first_);
  }
  void PopBack() {
    Erase(last_);
  }

  Iterator Find(const T& value) {
    Node* current = first_;
    while (current != nullptr) {
      if (current->value_ == value) return Iterator(this, current);
      current = current->next_node_;
    }
    return Iterator(this, nullptr);
  }
  ConstIterator Find(const T& value) const {
    Node* current = first_;
    while (current != nullptr) {
      if (current->value_ == value) return ConstIterator(this, current);
      current = current->next_node_;
    }
    return ConstIterator(this, nullptr);
  }

  Iterator Find(std::function<bool(const T&)> predicate) {
    Node* current = first_;
    while (current != nullptr) {
      if (predicate(current->value_)) return Iterator(this, current);
      current = current->next_node_;
    }
    return Iterator(this, nullptr);
  }
  ConstIterator Find(std::function<bool(const T&)> predicate) const {
    Node* current = first_;
    while (current != nullptr) {
      if (predicate(current->value_)) return ConstIterator(this, current);
      current = current->next_node_;
    }
    return ConstIterator(this, nullptr);
  }

 protected:
  struct Node {
    explicit Node(const T& value)
        : value_(value), next_node_(nullptr), previous_node_
        (nullptr) {}
    explicit Node(T&& value)
        : value_(value), next_node_(nullptr), previous_node_
        (nullptr) {}

    ~Node() = default;

    T value_;
    Node* next_node_;
    Node* previous_node_;
  };

  Node* first_;
  Node* last_;

  void InsertBefore(Node* existing_node, Node* new_node) {
    try {
      if (existing_node == nullptr || new_node == nullptr)
        throw logic_error("nullptr");
      if (existing_node == first_) {
        first_->previous_node_ = new_node;
        new_node->next_node_ = first_;
        first_ = new_node;
      } else {
        new_node->next_node_ = existing_node;
        new_node->previous_node_ = existing_node->previous_node_;
        existing_node->previous_node_ = new_node;
        existing_node->previous_node_->next_node_ = new_node;
      }
    } catch (logic_error&) {
      cerr << "nullptr insertion" << endl;
    }
  }

  void InsertAfter(Node* existing_node, Node* new_node) {
    try {
      if (existing_node == nullptr || new_node == nullptr)
        throw logic_error("nullptr");
      if (existing_node == last_) {
        last_->next_node_ = new_node;
        new_node->previous_node_ = last_;
        last_ = new_node;
      } else {
        new_node->previous_node_ = existing_node;
        new_node->next_node_ = existing_node->next_node_;
        existing_node->next_node_ = new_node;
        existing_node->next_node_->previous_node_ = new_node;
      }
    } catch (logic_error&) {
      cerr << "nullptr insertion" << endl;
    }
  }

  void Erase(Node* node) {
    if (node == nullptr) {
      return;
    } else if (node == first_ && node == last_) {
      first_ = nullptr;
      last_ = nullptr;
    } else if (node == first_) {
      first_ = first_->next_node_;
      if (first_ != nullptr) {
        first_->previous_node_ = nullptr;
      }
    } else if (node == last_) {
      last_ = last_->previous_node_;
      if (last_ != nullptr) {
        last_->next_node_ = nullptr;
      }
    } else {
      if (node->previous_node_ != nullptr) {
        node->previous_node_->next_node_ = node->next_node_;
      }
      if (node->next_node_ != nullptr) {
        node->next_node_->previous_node_ = node->previous_node_;
      }
    }
  }
};

int main() {

  {
    // push test
    BiDirectionalList<int> list;
    vector<int> ans;

    assert(list.AsArray() == ans);
    assert(list.IsEmpty());

    for (int i = 1; i <= 10; i++) {
      list.PushBack(i);
      ans.push_back(i);
      assert(list.AsArray() == ans);
    }

    assert(!list.IsEmpty());

    for (int i = 1; i <= 10; i++) {
      list.PushFront(i);
      ans.insert(ans.begin(), i);
      assert(list.AsArray() == ans);
    }

    list.Clear();
    assert(list.IsEmpty());
  }

  {
    // iterator test
    BiDirectionalList<int> list;
    vector<int> ans;

    for (int i = 1; i <= 10; i++) {
      list.PushBack(i);
      ans.push_back(i);
      assert(*list.begin() == 1);
      assert(*(list.end() - 1) == i);
    }

    for (int i = 1; i <= 10; i++) {
      list.PushFront(i);
      ans.insert(ans.begin(), i);
      assert(*list.begin() == i);
      assert(*(list.end() - 1) == 10);
    }
  }

  {
    // pop test
    BiDirectionalList<int> list;
    vector<int> ans;

    for (int i = 1; i <= 10; i++) {
      list.PushBack(i);
      ans.push_back(i);
    }

    list.PopBack();
    ans.pop_back();
    assert(list.AsArray() == ans);

    list.PopFront();
    ans.erase(ans.begin());
    assert(list.AsArray() == ans);
  }

  {
    // insert test
    BiDirectionalList<int> list;
    vector<int> ans;

    for (int i = 1; i <= 3; i++) {
      list.PushBack(i);
      ans.push_back(i);
    }

    list.InsertAfter(list.begin(), -1);
    ans.insert(ans.begin() + 1, -1);
    assert(list.AsArray() == ans);

    list.InsertAfter(list.end() - 1, -1);
    ans.push_back(-1);
    assert(list.AsArray() == ans);

    list.InsertAfter(next(list.begin()), -2);
    ans.insert(ans.begin() + 2, -2);
    assert(list.AsArray() == ans);

    list.Erase(list.begin());
    ans.erase(ans.begin());
    assert(list.AsArray() == ans);

    list.Erase(list.end() - 1);
    ans.erase(ans.end() - 1);
    assert(list.AsArray() == ans);
  }

  {
    // for-each test
    BiDirectionalList<int> list;
    vector<int> ans;

    for (int i = 0; i < 10; i++) {
      list.PushBack(i);
      ans.push_back(i);
    }

    vector<int> class_ans;
    for (const auto& item : list) {
      // cout << item << " ";
      class_ans.push_back(item);
    }
    // cout << endl;
    assert(class_ans == ans);

  }

  {
    // some other tests

    BiDirectionalList<int> list;

    // It will cause "increment nullptr"
    list.begin()++;

    // It will cause "nullptr insertion"
    list.InsertBefore(list.begin(), 42);

    // '&nullptr"
    *list.begin();
  }

  {
    // const testings
    BiDirectionalList<int> list;
    vector<int> ans;

    for (int i = 1; i <= 10; i++) {
      const int const_i = i;
      list.PushBack(const_i);
      ans.push_back(const_i);
      assert(list.AsArray() == ans);
    }

    const int elem = -1;
    list.InsertAfter(list.begin(), elem);
    ans.insert(ans.begin() + 1, elem);
    assert(list.AsArray() == ans);

    list.InsertBefore(list.begin(), elem);
    ans.insert(ans.begin(), elem);
    assert(list.AsArray() == ans);

    list.InsertBefore(list.begin(), elem - 5);
    ans.insert(ans.begin(), elem - 5);
    assert(list.AsArray() == ans);

    list.PushBack(elem + 1);
    ans.push_back(elem + 1);
    assert(list.AsArray() == ans);

    list.PushFront(elem + 1);
    ans.insert(ans.begin(), elem + 1);
    assert(list.AsArray() == ans);

    const BiDirectionalList<int> const_list;
    assert(const_list.begin() == const_list.end());
  }

  {
    // find test
    BiDirectionalList<int> list;
    const BiDirectionalList<int> const_list;

    for (int i = 1; i <= 10; i++) {
      list.PushBack(i);
    }

    assert(*(list.Find(5)) == 5);
    assert(list.Find(1) == list.begin());
    assert(list.Find(10) == list.end() - 1);

    const int smth = 2;
    assert(list.Find(smth) == list.begin() + 1);

    assert(const_list.Find(1) == const_list.end());

    assert(list.Find([](int value) { return value == 3; }) == list.begin() + 2);
    assert(const_list.Find([](int value) { return value == 3; })
               == const_list.begin());
  }
}