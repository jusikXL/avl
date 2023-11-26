#ifndef RING_HPP
#define RING_HPP

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

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

  bool _remove_node(Node *ntr)
  {
    if (ntr == _start)
    {
      return false;
    }

    Node *to_delete = ntr;

    ntr->_next->_past = ntr->_past;
    ntr->_past->_next = ntr->_next;

    delete to_delete;
    _size--;

    return true;
  }

  bool _remove_last(const Key &key, Node *ntr, unsigned int &n)
  {
    if (ntr == _start)
    {
      return false;
    }

    if (_remove_last(key, ntr->_next, n) && n == 0) // after deletion n is updated to n - 1
    {
      return true;
    }

    if (ntr->key == key)
    {
      n--;
      return _remove_node(ntr);
    }

    return false;
  }

public:
  Ring() : _size(0)
  {
    _start = new Node(Key{}, Info{});
  }

  /////////////////////////////// ITERATOR CLASS //////////////////////////////
  class Iterator
  {
  private:
    Node *_curr;
    friend class Ring<Key, Info>;

  public:
    Iterator(Node *node) : _curr(node)
    {
      if (!node)
      {
        throw std::invalid_argument("Iterator cannot be initialized pointing to nullptr");
      }
    }

    // prefix increment operator
    Iterator &operator++()
    {
      _curr = _curr->_next;
      return *this;
    }

    bool operator!=(const Iterator &src) const
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

  Iterator begin() const
  {
    return Iterator(_start->_next); // First real node what if sentinel???
  }

  Iterator end() const
  {
    return Iterator(_start); // Sentinel node
  }

  Iterator push(const Key &key, const Info &info)
  {
    Node *new_node = _insert_node(_start, _start->_next, key, info);
    return Iterator(new_node);
  }

  bool remove_last(const Key &key, unsigned int n = 1)
  {
    if (n == 0)
    {
      return false;
    }

    return _remove_last(key, begin().get_node(), n);
  }
};

template <typename Key, typename Info>
ostream &operator<<(ostream &os, const Ring<Key, Info> &ring)
{
  for (typename Ring<Key, Info>::Iterator it = ring.begin(); it != ring.end(); ++it)
  {
    os << "Key: " << it.get_node()->key << ", Info: " << it.get_node()->info << endl;
  }
  return os;
}

#endif // RING_HPP