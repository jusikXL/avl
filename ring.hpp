#ifndef RING_HPP
#define RING_HPP

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

  Node *_sentinel;
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
    if (ntr == _sentinel)
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

public:
  Ring() : _size(0)
  {
    _sentinel = new Node(Key{}, Info{});
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
        throw invalid_argument("Iterator cannot be initialized pointing to nullptr");
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
    return Iterator(_sentinel->_next);
  }

  Iterator end() const
  {
    return Iterator(_sentinel);
  }

  Iterator push(const Key &key, const Info &info)
  {
    return Iterator(_insert_node(_sentinel, _sentinel->_next, key, info));
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