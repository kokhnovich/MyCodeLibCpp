#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <cassert>

using namespace std;

class BinarySearchTree {
 public:
  //
  // Конструктор.
  // Инициализирует значения полей 'root_' и 'size_'.
  BinarySearchTree();

  // Деструктор.
  // Удаляет все элменты дерева путём удаления корневой вершины.
  ~BinarySearchTree();

  // Метод возвращает "true" если элемент со значением 'value' имеется в дереве
  // поиска и "false" в противном случае.
  bool Contains(int value) const;

  // Помещает значение 'value' в поисковое дерево.
  void Add(int value);

  // Удаляет элмент со значением 'value' из поискового дерева.
  // Если элементов с таким значением несколько, удаляется лишь одно из них.
  void Erase(int value);

  // Возвращает количество элементов в дереве.
  int Size() const;

  // Возвращает массив из элементов, содержащихся в дереве поиска,
  // расположенными в порядке неубывания.
  std::vector<int> ToSortedArray() const;

  // WARNING: DO NOT CHANGE VISIBILITY!
 protected:
  //
  // Вершина в дереве поиска.
  struct Node {
    int value_;
    Node* left_;
    Node* right_;
    Node* parent_;

    Node(int value, Node* parent) : parent_(parent), value_(value),
                                    left_(nullptr), right_(nullptr) {}

    ~Node() {
      delete left_;
      delete right_;
    }
  };

  int size_;
  Node* root_;

  // Вспомогательный метод, возвращающий указатель на одну из вершин со
  // значением 'value' (любую из них),
  // либо nullptr при отсутствии таких вершин.
  Node* Find(int value) const;

  // Вспомогательный метод, позволяющий "отвязать" вершину от дерева поиска.
  // После выполнения данной операции, вершина 'target_node' перестаёт входить
  // в поддерево, образуемое вершиной 'root_', но удаления данной вершины не
  // происходит.
  void Detach(Node* target_node);

  // Вспомогательные методы, которые позволяют получать указатели на самую
  // левую и самую правую вершины в поддереве, образуемом вершиной node,
  // соответствтенно.
  Node* GetleftmostNode(Node* node) const;

  Node* GetrightmostNode(Node* node) const;

  // Вспомогательные метод, дописывающий все элементы в поддереве, образуемом
  // вершиной node, в конец вектора *res в порядке неубывания хранимых значений.
  void AppendToSortedArray(Node* node, std::vector<int>* res) const;

  // Удаляет указатель на вершину у родителя.
  void DetachForParent(Node* node);
};


BinarySearchTree::BinarySearchTree() : size_(0), root_(nullptr) {}

BinarySearchTree::~BinarySearchTree() {
  delete root_;
}

bool BinarySearchTree::Contains(int value) const {
  return Find(value) != nullptr;
}

void BinarySearchTree::Add(int value) {
  size_++;
  if (root_ == nullptr) {
    root_ = new Node(value, nullptr);
    return;
  }
  Node* current_node = root_;
  while (true) {
    if (value >= current_node->value_) {
      if (current_node->right_ == nullptr) {
        current_node->right_ = new Node(value, current_node);
        return;
      } else {
        current_node = current_node->right_;
      }
    } else {
      if (current_node->left_ == nullptr) {
        current_node->left_ = new Node(value, current_node);
        return;
      } else {
        current_node = current_node->left_;
      }
    }
  }
}

void BinarySearchTree::Erase(int value) {
  Node* node = Find(value);
  if (node == nullptr) {
    return;
  }
  Detach(node);
  size_--;
  if (size_ == 0) {
    root_ = nullptr;
  }
}

int BinarySearchTree::Size() const {
  return size_;
}

std::vector<int> BinarySearchTree::ToSortedArray() const {
  vector<int> res;
  AppendToSortedArray(root_, &res);
  return res;
}

BinarySearchTree::Node* BinarySearchTree::Find(int value) const {
  if (root_ == nullptr) {
    return nullptr;
  }
  Node* current_node = root_;
  while (current_node != nullptr && current_node->value_ != value) {
    if (value > current_node->value_) {
      current_node = current_node->right_;
    } else {
      current_node = current_node->left_;
    }
  }
  return current_node;
}

void BinarySearchTree::Detach(BinarySearchTree::Node* node) {
  if (node->right_ != nullptr && node->left_ != nullptr) {
    Node* left_mostNode = GetleftmostNode(node->right_);
    node->value_ = left_mostNode->value_;
    Detach(left_mostNode);
  } else if (node->right_ != nullptr || node->left_ != nullptr) {
    Node* child = (node->left_ != nullptr) ? node->left_ : node->right_;
    DetachForParent(child);
    if (node == root_) {
      root_ = child;
    } else {
      if (node->parent_->left_ == node) {
        DetachForParent(node);
        node->parent_->left_ = child;
      } else {
        DetachForParent(node);
        node->parent_->right_ = child;
      }
      child->parent_ = node->parent_;
    }
    delete node;
  } else {
    DetachForParent(node);
    delete node;
  }
}

BinarySearchTree::Node*
BinarySearchTree::GetleftmostNode(BinarySearchTree::Node* node) const {
  while (node->left_ != nullptr) {
    node = node->left_;
  }
  return node;
}

BinarySearchTree::Node*
BinarySearchTree::GetrightmostNode(BinarySearchTree::Node* node) const {
  while (node->right_ != nullptr) {
    node = node->right_;
  }
  return node;
}

void BinarySearchTree::AppendToSortedArray(BinarySearchTree::Node* node,
                                           std::vector<int>* res) const {
  if (node == nullptr) {
    return;
  }
  AppendToSortedArray(node->left_, res);
  res->push_back(node->value_);
  AppendToSortedArray(node->right_, res);
}

void BinarySearchTree::DetachForParent(BinarySearchTree::Node* target_node) {
  if (target_node == root_) {
    return;
  }
  if (target_node->parent_ != nullptr) {
    if (target_node->parent_->left_ == target_node) {
      target_node->parent_->left_ = nullptr;
    } else {
      target_node->parent_->right_ = nullptr;
    }
  }
}
// --------------------------------------------------------------------------

#ifndef IGNORE_MAIN
int main() {
  const int kQueriesCount = 50'000;
  const std::vector<int>
      kElementsMaxvalues({1, 2, 5, 13, 42, 1024, 1'000'000'000});

  mt19937_64 random_generator(2018);
  for (const int max_element_value : kElementsMaxvalues) {
    vector<int> elements;
    BinarySearchTree tree;

    for (int i = 0; i < kQueriesCount; ++i) {
      int query_type = random_generator() % 3;

      switch (query_type) {
        case 0: {
          int value;
          if (elements.empty() || random_generator() % 100 < 75) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          elements.push_back(value);
          sort(elements.begin(), elements.end());  // Never do like this :)
          tree.Add(value);
          break;
        }
        case 1: {
          int value;
          if (elements.empty() || random_generator() % 100 < 40) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          bool result = tree.Contains(value);
          bool expected_result =
              std::find(elements.begin(), elements.end(), value)
                  != elements.end();
          assert(result == expected_result);
          break;
        }
        case 2: {
          int value;
          if (elements.empty() || random_generator() % 100 < 25) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          auto element_it =
              std::find(elements.begin(), elements.end(), value);
          if (element_it != elements.end()) {
            elements.erase(element_it);
          }
          tree.Erase(value);
          break;
        }
        default: {
          break;  // IMPOSSIBLE
        }
      }

      assert(elements.size() == tree.Size());
      assert(elements == tree.ToSortedArray());
    }

    cout << "Passed: max_element_value = " << max_element_value << endl;
  }

  return 0;
}
#endif
