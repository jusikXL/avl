#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
using namespace std;

/////////////////////////////// HELPERS //////////////////////////////
enum Comparison {
  Less = -1,
  Equal = 0,
  Greater = 1,
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
    Node(const Key& k, const Info& i, Node* l = nullptr, Node* r = nullptr)
      : data(k, i), _left(l), _right(r) { }
    ~Node() {
      delete _left;
      delete _right;
    }

  private:
    Node* _left;
    Node* _right;
    unsigned int _height = 1;
    int _balance = 0;

    void _update_height() {
      _height = 1 + max(_left ? _left->_height : 0, _right ? _right->_height : 0);
      _balance = (_left ? _left->_height : 0) - (_right ? _right->_height : 0);
    }

    friend class AVLTree<Key, Info>;
  };
  /////////////////////////////// NODE //////////////////////////////

  Node* _root = nullptr;
  size_t _size = 0;

public:
  AVLTree() { }
  AVLTree(const AVLTree& src) { *this = src; }
  AVLTree& operator=(const AVLTree& src) {
    if (this != &src) {
      clear();
      _copy(_root, src._root);
    }

    return *this;
  }
  ~AVLTree() { clear(); }

  size_t size() const { return _size; }

  void clear() {
    delete _root;

    _root = nullptr;
    _size = 0;
  }

  void print(ostream& out = cout) const { _print(out, _root, 0); }

  void insert(const Key& key, const Info& info,
    function<Info(const Info& oldInfo, const Info& newInfo)> on_same_key =
    [ ](const Info& oldInfo, const Info& newInfo) { return newInfo; }) {
    _insert(_root, key, info, on_same_key);
  }

  bool remove(const Key& key) {
    if (_remove(_root, key)) {
      _size--;
      return true;
    }

    return false;
  }

  Info& operator[](const Key& key) { return _search(_root, key)->data.second; }
  const Info& operator[](const Key& key) const { return _search(_root, key)->data.second; };
  bool find(const Key& key) const { return _safe_search(_root, key) != nullptr; }

  bool balanced() {
    return _balanced(_root);
  }

  template <typename Fn> void for_each(Fn fn) { _for_each(_root, fn); }
  vector<pair<Key, Info>> get_largest(int n) {
    vector<pair<Key, Info>> result;
    _get_largest(_root, n, result);
    return result;
  }
  vector<pair<Key, Info>> get_smallest(int n) {
    vector<pair<Key, Info>> result;
    _get_smallest(_root, n, result);
    return result;
  }
private:
  void _get_largest(Node* node, int& n, vector<pair<Key, Info>>& result) {
    if (node == nullptr || n == 0) {
      return;
    }

    _get_largest(node->_right, n, result);
    if (n > 0) {
      result.push_back(pair<Key, Info>(node->data.first, node->data.second));
      n--;
    }
    if (n > 0) {
      _get_largest(node->_left, n, result);
    }
  }

  void _get_smallest(Node* node, int& n, vector<pair<Key, Info>>& result) {
    if (node == nullptr || n == 0) {
      return;
    }

    _get_smallest(node->_left, n, result);
    if (n > 0) {
      result.push_back(pair<Key, Info>(node->data.first, node->data.second));
      n--;
    }
    if (n > 0) {
      _get_smallest(node->_right, n, result);
    }
  }

  template <typename Fn> void _for_each(Node* node, Fn fn) {
    if (node == nullptr) return;
    _for_each(node->_left, fn);
    fn(node->data.first, node->data.second);
    _for_each(node->_right, fn);
  }

  void _copy(Node*& my_node, const Node* src_node) {
    if (!src_node)
      return;

    my_node = new Node(src_node->data.first, src_node->data.second);
    _size++;

    _copy(my_node->_left, src_node->_left);
    _copy(my_node->_right, src_node->_right);
  }

  bool _balanced(Node* node) {
    if (!node) {
      return true;
    }

    int balance = node->_balance;

    if (balance < -1 || balance > 1) {
      return false;
    }

    return _balanced(node->_left) && _balanced(node->_right);
  }

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

  void _insert(Node*& node, const Key& key, const Info& info,
    function<Info(const Info& oldInfo, const Info& newInfo)> on_same_key) {
    if (!node) {
      _size++;
      node = new Node(key, info);
    } else {
      switch (compare(key, node->data.first)) {
      case Less:
        _insert(node->_left, key, info, on_same_key);
        break;
      case Greater:
        _insert(node->_right, key, info, on_same_key);
        break;
      case Equal:
        node->data.second = on_same_key(node->data.second, info);
        break;
      }
    }

    node->_update_height();
    _balance(node);
  }

  Node* _safe_search(Node* node, const Key& search_key) const {
    if (!node) {
      return nullptr;
    }

    switch (compare(search_key, node->data.first)) {
    case Less:
      return _safe_search(node->_left, search_key);
    case Greater:
      return _safe_search(node->_right, search_key);
    default:
      return node;
    }
  }

  Node* _search(Node* node, const Key& search_key) const {
    node = _safe_search(node, search_key);

    if (!node) {
      ostringstream fmt;
      fmt << "Key not found: " << search_key;
      throw out_of_range(fmt.str());
    }

    return node;
  }

  Node* _find_min(Node* node) const { return node->_left ? _find_min(node->_left) : node; }

  bool _remove(Node*& node, const Key& key) {
    if (!node) {
      return false;
    }
    bool deleted = false;

    switch (compare(key, node->data.first)) {
    case Less:
      deleted = _remove(node->_left, key);
      break;
    case Greater:
      deleted = _remove(node->_right, key);
      break;
    case Equal:
      deleted = true;

      if (node->_left && node->_right) {
        Node* successor = _find_min(node->_right);
        node->data = successor->data;
        _remove(node->_right, successor->data.first);
      } else {
        Node* temp = node->_left ? node->_left : node->_right;
        if (!temp) {
          temp = node;
          node = nullptr;
        } else {
          *node = *temp;
        }
        delete temp;
      }

      break;
    }

    if (node) {
      node->_update_height();
      _balance(node);
    }

    return deleted;
  }
};


//////////////////////////// EXTERNAL //////////////////////////////////
template <typename Key, typename Info>
vector<pair<Key, Info>> maxinfo_selector(const AVLTree<Key, Info>& tree, unsigned cnt) {
  AVLTree<Info, Key> inverted;
  AVLTree<Key, Info> copy = tree;

  copy.for_each([&inverted](const int& key, const string& info) {
    inverted.insert(info, key, [&](const Key& oldKey, const Key& newKey) { return oldKey + newKey; });
    });

  vector<pair<Info, Key>> largest_inverted = inverted.get_largest(cnt);

  vector<pair<Key, Info>> largest;
  for (auto& pair : largest_inverted) {
    largest.push_back(make_pair(pair.second, pair.first));
  }

  return largest;
}


#endif // AVL_TREE_HPP