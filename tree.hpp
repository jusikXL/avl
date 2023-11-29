#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
using namespace std;

template <typename Key, typename Info>
class BST
{
private:
  class Node
  {
  public:
    Key key;
    Info info;
    Node(const Key &k, const Info &i, Node *l = nullptr, Node *r = nullptr)
        : key(k), info(i), _left(l), _right(r) {}

  private:
    Node *_left;
    Node *_right;

    friend class BST<Key, Info>;
  };

  Node *_root;

  void _destroy_tree(Node *node)
  {
    if (node != nullptr)
    {
      _destroy_tree(node->_left);
      _destroy_tree(node->_right);
      delete node;
    }
  }

  void _insert(const Key &key, const Info &info, Node *&node)
  {
    if (node == nullptr)
    {
      node = new Node(key, info);
    }
    else if (key < node->key)
    {
      _insert(key, info, node->_left);
    }
    else if (key > node->key)
    {
      _insert(key, info, node->_right);
    }
  }

  Node *_min_value_node(Node *node)
  {
    Node *current = node;
    while (current && current->_left != nullptr)
      current = current->_left;

    return current;
  }

  unsigned int _find_number_of_leaves(Node *node)
  {
    if (node == nullptr)
    {
      return 0;
    }
    if (node->_left == nullptr && node->_right == nullptr)
    {
      return 1;
    }
    return _find_number_of_leaves(node->_left) + _find_number_of_leaves(node->_right);
  }

  unsigned int _find_height(Node *node)
  {
    if (node == nullptr)
    {
      return 0;
    }
    return 1 + max(_find_height(node->_left), _find_height(node->_right));
  }

  unsigned int _count_odd(Node *node)
  {
    if (node == nullptr)
    {
      return 0;
    }

    return (node->key % 2 != 0) + _count_odd(node->_left) + _count_odd(node->_right);
  }

  unsigned int _count_even(Node *node)
  {
    if (node == nullptr)
    {
      return 0;
    }

    return (node->key % 2 == 0) + _count_even(node->_left) + _count_even(node->_right);
  }

  Node *_remove(const Key &key, Node *&node)
  {
    if (node == nullptr)
    {
      return nullptr; // Key not found
    }
    if (key < node->key)
    {
      node->_left = _remove(key, node->_left);
    }
    else if (key > node->key)
    {
      node->_right = _remove(key, node->_right);
    }
    else
    {
      // Node with only one child or no child
      if (node->_left == nullptr)
      {
        Node *temp = node->_right;
        delete node;
        return temp;
      }
      else if (node->_right == nullptr)
      {
        Node *temp = node->_left;
        delete node;
        return temp;
      }

      // Node with two children: Get the inorder successor
      Node *temp = _min_value_node(node->_right);

      // Copy the inorder successor's content to this node
      node->key = temp->key;
      node->info = temp->info;

      // Delete the inorder successor
      node->_right = _remove(temp->key, node->_right);
    }

    return node;
  }

public:
  BST() : _root(nullptr) {}

  ~BST()
  {
    _destroy_tree(_root);
  }

  void insert(const Key &key, const Info &info)
  {
    _insert(key, info, _root);
  }

  bool remove(const Key &key)
  {
    return _remove(key, _root) != nullptr;
  }

  unsigned int find_number_of_leaves()
  {
    return _find_number_of_leaves(_root);
  }

  unsigned int find_height()
  {
    return _find_height(_root);
  }

  unsigned int count_odd()
  {
    return _count_odd(_root);
  }

  unsigned int count_even()
  {
    return _count_even(_root);
  }

  static void printInOrder(ostream &os, const Node *node)
  {
    if (node == nullptr)
    {
      return;
    }
    printInOrder(os, node->_left);
    os << "Key: " << node->key << ", Info: " << node->info << endl;
    printInOrder(os, node->_right);
  }

  friend ostream &operator<<(ostream &os, const BST<Key, Info> &tree)
  {
    if (tree._root != nullptr)
    {
      printInOrder(os, tree._root);
    }
    return os;
  }
};

#endif // TREE_HPP