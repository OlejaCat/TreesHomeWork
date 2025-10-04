#ifndef TREES_HPP_ 
#define TREES_HPP_

#include <sys/types.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <functional>

namespace trees {

template <typename KeyType, typename Comparator = std::less<KeyType>>
class AvlTree {
 private:
  struct Node {
    KeyType data;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;

    int height = 1;

    Node(KeyType key);
    Node(KeyType key, Node* parent_node);
  };

 public:
  using Iterator = Node*;

  AvlTree() = default;
  AvlTree(const Comparator& comparator);

  AvlTree(const AvlTree&) = delete;
  AvlTree operator=(const AvlTree&) = delete;

  ~AvlTree();

  void insert(const KeyType& key);

  std::size_t size();

  KeyType get_key(Iterator node) const;

  Iterator begin();
  Iterator end();
  Iterator next(Iterator node) const;
  Iterator prev(Iterator node) const;

  Iterator lower_bound(const KeyType& key);
  Iterator upper_bound(const KeyType& key);
  int distance(Iterator begin, Iterator end) const;

  void print_tree();

 private:
  void delete_nodes(Node* node);

  Node* insert(Node* node, Node* parent, KeyType key);
  Node* balance(Node* node);

  Node* left_rotate(Node* node);
  Node* right_rotate(Node* node);
  Node* big_left_rotate(Node* node);
  Node* big_right_rotate(Node* node);

  int get_height(Node* node);
  int get_balance(Node* node);
  void update_height(Node* node);

  Iterator find_lower_bound(Iterator node, const KeyType& key);
  Iterator find_upper_bound(Iterator node, const KeyType& key);

  Iterator find_bigger(Iterator node) const;
  Iterator find_smaller(Iterator node) const;

  Iterator find_min(Iterator node) const;
  Iterator find_max(Iterator node) const;

  void print_tree(const std::string& prefix, const Node* node, bool is_left);

  Comparator comparator_;
  Node* root_ = nullptr;

  std::size_t size_ = 0;
};

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::AvlTree(const Comparator& comparator)
    : comparator_(comparator) {}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Node::Node(KeyType key) : data(key) {}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Node::Node(KeyType key, Node* parent_node)
    : data(key), parent(parent_node) {}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::~AvlTree() {
  delete_nodes(root_);
}

template <typename KeyType, typename Comparator>
void AvlTree<KeyType, Comparator>::insert(const KeyType& key) {
  root_ = insert(root_, nullptr, key);
}

template <typename KeyType, typename Comparator>
std::size_t AvlTree<KeyType, Comparator>::size() {
  return size_;
}

template <typename KeyType, typename Comparator>
KeyType AvlTree<KeyType, Comparator>::get_key(Iterator node) const {
  return node->data;
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator AvlTree<KeyType, Comparator>::begin() {
  return find_min(root_);
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator AvlTree<KeyType, Comparator>::end() {
  return nullptr;
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator AvlTree<KeyType, Comparator>::next(
    Iterator node) const {
  return find_bigger(node);
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator AvlTree<KeyType, Comparator>::prev(
    Iterator node) const {
  return find_smaller(node);
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator
AvlTree<KeyType, Comparator>::lower_bound(const KeyType& key) {
  return find_lower_bound(root_, key);
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator
AvlTree<KeyType, Comparator>::upper_bound(const KeyType& key) {
  return find_upper_bound(root_, key);
}

template <typename KeyType, typename Comparator>
int AvlTree<KeyType, Comparator>::distance(Iterator begin, Iterator end) const {
  if (begin == end) {
    return 0;
  }

  if (begin == nullptr) {
    return -1;
  }

  int count = 0;
  Iterator current = begin;
  while (current != nullptr && current != end) {
    ++count;
    current = find_bigger(current);
  }

  if (current == end) {
    return count;
  }

  return -1;
}

template <typename KeyType, typename Comparator>
void AvlTree<KeyType, Comparator>::print_tree() {
  print_tree("", root_, false);
}

template <typename KeyType, typename Comparator>
void AvlTree<KeyType, Comparator>::delete_nodes(Node* node) {
  if (node != nullptr) {
    delete_nodes(node->left);
    delete_nodes(node->right);

    delete node;
  }
}

template <typename KeyType, typename Comparator>
typename AvlTree<KeyType, Comparator>::Node*
AvlTree<KeyType, Comparator>::insert(Node* node, Node* parent, KeyType key) {
  if (node == nullptr) {
    ++size_;
    return new Node(key, parent);
  }

  if (comparator_(key, node->data)) {
    node->left = insert(node->left, node, key);
  } else if (comparator_(node->data, key)) {
    node->right = insert(node->right, node, key);
  } else {
    return node;
  }

  return balance(node);
}

template <typename KeyType, typename Comparator>
typename AvlTree<KeyType, Comparator>::Node*
AvlTree<KeyType, Comparator>::balance(Node* node) {
  if (node == nullptr) {
    return nullptr;
  }

  update_height(node);
  int balance_factor = get_balance(node);

  if (balance_factor > 1) {
    int left_balance_factor = get_balance(node->left);
    if (left_balance_factor >= 0) {
      return right_rotate(node);
    }
    return big_right_rotate(node);
  }

  if (balance_factor < -1) {
    int right_balance_factor = get_balance(node->right);
    if (right_balance_factor <= 0) {
      return left_rotate(node);
    }
    return big_left_rotate(node);
  }

  return node;
}

template <typename KeyType, typename Comparator>
typename AvlTree<KeyType, Comparator>::Node*
AvlTree<KeyType, Comparator>::left_rotate(Node* node) {
  Node* right_child = node->right;
  node->right = right_child->left;
  right_child->left = node;

  update_height(node);
  update_height(right_child);

  return right_child;
}

template <typename KeyType, typename Comparator>
typename AvlTree<KeyType, Comparator>::Node*
AvlTree<KeyType, Comparator>::right_rotate(Node* node) {
  Node* left_child = node->left;
  node->left = left_child->right;
  left_child->right = node;

  update_height(node);
  update_height(left_child);

  return left_child;
}

template <typename KeyType, typename Comparator>
typename AvlTree<KeyType, Comparator>::Node*
AvlTree<KeyType, Comparator>::big_left_rotate(Node* node) {
  node->right = right_rotate(node->right);
  return left_rotate(node);
}

template <typename KeyType, typename Comparator>
typename AvlTree<KeyType, Comparator>::Node*
AvlTree<KeyType, Comparator>::big_right_rotate(Node* node) {
  node->left = left_rotate(node->left);
  return right_rotate(node);
}

template <typename KeyType, typename Comparator>
int AvlTree<KeyType, Comparator>::get_height(Node* node) {
  return node != nullptr ? node->height : 0;
}

template <typename KeyType, typename Comparator>
void AvlTree<KeyType, Comparator>::update_height(Node* node) {
  if (node != nullptr) {
    node->height =
        1 + std::max(get_height(node->left), get_height(node->right));
  }
}

template <typename KeyType, typename Comparator>
int AvlTree<KeyType, Comparator>::get_balance(Node* node) {
  return get_height(node->left) - get_height(node->right);
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator
AvlTree<KeyType, Comparator>::find_lower_bound(Iterator node,
                                               const KeyType& key) {
  Node* result = nullptr;
  Node* current = node;

  while (current != nullptr) {
    if (comparator_(current->data, key)) {
      current = current->right;
    } else {
      result = current;
      current = current->left;
    }
  }

  return result;
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator
AvlTree<KeyType, Comparator>::find_upper_bound(Iterator node,
                                               const KeyType& key) {
  Node* result = nullptr;
  Node* current = node;

  while (current != nullptr) {
    if (comparator_(key, current->data)) {
      result = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }

  return result;
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator
AvlTree<KeyType, Comparator>::find_bigger(Iterator node) const {
  if (node == nullptr) {
    return nullptr;
  }

  if (node->right != nullptr) {
    return find_min(node->right);
  }

  Node* parent = node->parent;
  while (parent != nullptr && node == parent->right) {
    node = parent;
    parent = parent->parent;
  }

  return parent;
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator
AvlTree<KeyType, Comparator>::find_smaller(Iterator node) const {
  if (node == nullptr) {
    return nullptr;
  }

  if (node->left != nullptr) {
    return find_max(node->left);
  }

  Node* parent = node->parent;
  while (parent != nullptr && node == parent->left) {
    node = parent;
    parent = parent->parent;
  }

  return parent;
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator AvlTree<KeyType, Comparator>::find_min(
    Iterator node) const {
  if (node == nullptr) {
    return nullptr;
  }

  while (node->left != nullptr) {
    node = node->left;
  }

  return node;
}

template <typename KeyType, typename Comparator>
AvlTree<KeyType, Comparator>::Iterator AvlTree<KeyType, Comparator>::find_max(
    Iterator node) const {
  if (node == nullptr) {
    return nullptr;
  }

  while (node->right != nullptr) {
    node = node->right;
  }

  return node;
}

template <typename KeyType, typename Comparator>
void AvlTree<KeyType, Comparator>::print_tree(const std::string& prefix,
                                              const Node* node, bool is_left) {
  if (node != nullptr) {
    std::cout << prefix;
    std::cout << (is_left ? "├──" : "└──");
    std::cout << node->data;
    std::cout << "value: " << node->data << " parent ref: " << node->parent << std::endl;

    print_tree(prefix + (is_left ? "│   " : "    "), node->left, true);
    print_tree(prefix + (is_left ? "│   " : "    "), node->right, false);
  }
}

} // namespace trees

#endif // TREES_HPP_
