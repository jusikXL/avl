#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <iomanip>
using namespace std;

/////////////////////////////// HELPERS //////////////////////////////

enum Comparison {
  Less = -1,
  Equal = 0,
  Greater = 1,
};

enum Balance {
  LeftHeavy = -1,
  Neutral = 0,
  RightHeavy = 1
};

enum Rotate {
  Left,
  Right
};

template <typename Key>
int compare(const Key& key1, const Key& key2) {
  if (key1 < key2) {
    return -1;
  } else if (key1 > key2) {
    return 1;
  } else {
    return 0;
  }
}

/////////////////////////////// HELPERS //////////////////////////////


template <typename Key, typename Info>
class AVLTree {
private:
  /////////////////////////////// NODE //////////////////////////////
  class Node {
  public:
    pair<Key, Info> data;
    Node(const Key& k, const Info& i, Node* l = nullptr, Node* r = nullptr, unsigned int h = 1, int b = 0)
      : data(k, i), _left(l), _right(r), _height(h), _balance(b) { }

  private:
    Node* _left;
    Node* _right;
    unsigned int _height;
    int _balance;

    void _update_height() {
      _height = 1 + max(_left ? _left->_height : 0, _right ? _right->_height : 0);
      _balance = (_left ? _left->_height : 0) - (_right ? _right->_height : 0);
    }

    friend class AVLTree<Key, Info>;
  };
  /////////////////////////////// NODE //////////////////////////////

  Node* _root = nullptr;
  size_t _size = 0;

  void _print(ostream& os, Node* node, int indent) const {
    if (!!node) {
      // Print right subtree
      _print(os, node->_right, indent + 6);

      // Print current node
      os << setw(indent) << ' ';
      os << node->data.first << ":" << node->data.second << "\n";

      // Print left subtree
      _print(os, node->_left, indent + 6);
    }
  }

  void _rotate(Node*& old_root, Rotate direction) {
    Node* pivot;

    switch (direction) {
    case Left:
      pivot = old_root->_right;
      old_root->_right = pivot->_left;
      pivot->_left = old_root;
      break;

    case Right:
      pivot = old_root->_left;
      old_root->_left = pivot->_right;
      pivot->_right = old_root;
      break;
    }

    old_root->_update_height();
    old_root = pivot;
    pivot->_update_height();
  }

  void _balance(Node*& node) {
    int balance = node->_balance;

    if (balance > 1) {
      // Left-Right case (LR)
      if (node->_left->_balance < 0) {
        _rotate(node->_left, Left);
      }
      // Left-Left case (LL)
      _rotate(node, Right);
    }

    if (balance < -1) {
      // Right-Left case (RL)
      if (node->_right->_balance > 0) {
        _rotate(node->_right, Right);
      }
      // Right-Right case (RR)
      _rotate(node, Left);
    }
  }

  Node* _insert_or_search(Node*& node, const Key& key, const Info& info) {
    Node* found = nullptr;

    if (!node) {
      _size++;
      node = new Node(key, info);

      found = node;
    }

    switch (compare(key, node->data.first)) {
    case Less:
      _insert_or_search(node->_left, key, info);
      break;
    case Greater:
      _insert_or_search(node->_right, key, info);
      break;
    case Equal:
      found = node;
      break;
    }

    node->_update_height();
    _balance(node);

    return found;
  }

  Node* _search(Node* node, const Key& search_key) const {
    if (!node) {
      return nullptr;
    }

    switch (compare(search_key, node->data.first)) {
    case Less:
      return _search(search_key, node->_left);
    case Greater:
      return _search(search_key, node->_right);
    case Equal:
      return node;
    }
  }

  Node* _search_with_error(Node* node, const Key& search_key) const {
    node = _search(node, search_key);

    if (!node) {
      ostringstream fmt;
      fmt << "Key not found: " << search_key;
      throw out_of_range(fmt.str());
    }

    return node;
  }


public:
  AVLTree() { }
  AVLTree(const AVLTree& src) { *this = src; }
  AVLTree& operator=(const AVLTree& src) {
    if (this != &src) {
      clear();
      copy(_root, src._root);
    }

    return *this;
  }
  ~AVLTree() { clear(); }

  size_t size() const { return _size; }

  void copy(Node*& my_node, const Node* src_node) {
    if (!src_node)
      return;

    my_node = new Node(src_node->data.first, src_node->data.second);
    _size++;

    copy(my_node->left, src_node->left);
    copy(my_node->right, src_node->right);
  }

  void clear() {
    delete _root;

    _root = nullptr;
    _size = 0;
  }

  void print(ostream& out = cout) const { _print(out, _root, 0); }

  void insert(const Key& key, const Info& info) {
    _insert_or_search(_root, key, info);
  }

  Info& operator[](const Key& key) { return _insert_or_search(_root, key, Info())->data.second; }
  const Info& operator[](const Key& key) const {
    Node* found = _search(_root, key);
    return found ? found->data.second : Info();
  };
  Info& at(const Key& key) { return _search_with_error(_root, key)->info; }
  const Info& at(const Key& key) const { return _search_with_error(_root, key)->info; }
};


//////////////////////////// EXTERNAL //////////////////////////////////




#endif // AVL_TREE_HPP