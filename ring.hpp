#ifndef RING_HPP
#define RING_HPP

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

template <typename Key, typename Info>
class Ring;

template <typename Key, typename Info>
ostream &operator<<(ostream &os, const Ring<Key, Info> &ring);

template <typename Key, typename Info>
class Ring
{
private:
  /////////////////////////////// NODE CLASS //////////////////////////////
  class Node
  {
  public:
    Key key;
    Info info;
    Node(const Key &k, const Info &i, Node *p = nullptr, Node *n = nullptr)
        : key(k), info(i), _past(p ? p : this), _next(n ? n : this) {}

  private:
    Node *_past;
    Node *_next;
    friend class Ring<Key, Info>;
  };
  /////////////////////////////// NODE CLASS //////////////////////////////

  Node *_start;
  unsigned int _size;

  Node *_insert_node(Node *before, Node *after, const Key &key, const Info &info)
  {
    Node *new_node = new Node(key, info, before, after);

    before->_next = new_node;
    after->_past = new_node;

    _size++;
    return new_node;
  }

public:
  Ring() : _size(0)
  {
    _start = new Node(Key{}, Info{});
  }

  /////////////////////////////// ITERATOR CLASS //////////////////////////////
  class iterator
  {
  private:
    Node *_curr;
    friend class Ring<Key, Info>;

  public:
    iterator(Node *node) : _curr(node)
    {
      if (node == nullptr)
      {
        throw std::invalid_argument("Iterator cannot be initialized pointing to nullptr");
      }
    }

    // prefix increment operator
    iterator &operator++()
    {
      _curr = _curr->_next;
      return *this;
    }

    bool operator!=(const iterator &src) const
    {
      return _curr != src._curr;
    }

    // Dereference operator
    // std::pair<Key, Info> &operator*()
    // {
    //   return {_curr->key, _curr->info};
    // }

    // Equality and Inequality operators
    // ...

    Node *get_node() const
    {
      return _curr;
    }
  };
  /////////////////////////////// ITERATOR CLASS //////////////////////////////

  iterator begin() const
  {
    return iterator(_start->_next); // First real node what if sentinel???
  }

  iterator end() const
  {
    return iterator(_start); // Sentinel node
  }

  iterator push(const Key &key, const Info &info)
  {
    Node *new_node = _insert_node(_start, _start->_next, key, info);
    return iterator(new_node);
  }
};

template <typename Key, typename Info>
ostream &operator<<(ostream &os, const Ring<Key, Info> &ring)
{
  for (typename Ring<Key, Info>::iterator it = ring.begin(); it != ring.end(); ++it)
  {
    os << "Key: " << it.get_node()->key << ", Info: " << it.get_node()->info << endl;
  }
  return os;
}

#endif // RING_HPP