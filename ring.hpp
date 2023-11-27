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

  bool _remove_last(Node *ntr, const Key &key, unsigned int &n)
  {
    if (ntr == _start)
    {
      return false;
    }

    if (_remove_last(ntr->_next, key, n) && n == 0) // after deletion n is updated to n - 1
    {
      return true;
    }

    if (ntr->key == key)
    {
      n--;
      return _remove_node(ntr); // true
    }

    return false;
  }

  unsigned int _remove_exactly_last(Node *ntr, const Key &key, unsigned int n)
  {
    if (ntr == _start)
    {
      return 0;
    }

    static unsigned int counter = 0;

    if (ntr->key == key)
    {
      counter++;
    }

    unsigned int removed = _remove_exactly_last(ntr->_next, key, n);

    if (counter < n)
    {
      return 0; // not found enough key occurences, return 0 up to the first call
    }

    if (removed == n)
    {
      return removed; // removed all the needed key occurences, return removed up to the first call
    }

    if (ntr->key == key)
    {
      _remove_node(ntr);

      return ++removed;
    }

    return removed;
  }

  bool _remove_all(Node *ntr, const Key &key)
  {
    if (ntr == _start)
    {
      return false;
    }

    bool success = _remove_all(ntr->_next, key);

    if (ntr->key == key)
    {
      return _remove_node(ntr); // true
    }

    return success;
  }

  bool _push_with_priority(Node *ntr, const Key &key, const Info &info)
  {
    if (ntr == _start)
    {
      return false;
    }

    if (_push_with_priority(ntr->_next, key, info))
    {
      return true;
    }

    if (ntr->key > key)
    {
      Node *new_node = new Node(key, info, nullptr, ntr->_next);
      ntr->_next = new_node;

      return true;
    }

    return false;
  }

  bool _insert_pair_at(Node *ntr, const Key &position, const Key &key_before, const Info &info_before, const Key &key_after, const Info &info_after)
  {
    if (ntr == _start)
    {
      return false;
    }

    if (_insert_pair_at(ntr->_next, position, key_before, info_before, key_after, info_after))
    {
      return true;
    }

    if (ntr->key == position)
    {
      _insert_node(ntr->_past, ntr, key_before, info_before);
      _insert_node(ntr, ntr->_next, key_after, info_after);
      return true;
    }

    return false;
  }

  bool _insert_before_each(Node *&ntr, const Key &position, const Key &key, const Info &info)
  {
    if (ntr == _start)
    {
      return false;
    }

    bool success = _insert_before_each(ntr->_next, position, key, info);

    if (ntr->key == position)
    {
      Node *new_node = new Node(key, info, nullptr, ntr);
      ntr = new_node;

      return true;
    }

    return success;
  }

  bool _remove_interval(Node *ntr, const Key &from, const Key &to, bool &success = false)
  {
    if (ntr == _start)
    {
      return false;
    }

    bool continue_removal = _remove_interval(ntr->_next, from, to, success);

    if (ntr->key == to || continue_removal)
    {
      // handle remove
      if (ntr->key == from && ntr->_past->key != from)
      {
        // stop when the very first occurence of from is reached
        return !_remove_node(ntr); // false
      }

      success = true;
      return _remove_node(ntr); // true
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

    return _remove_last(begin().get_node(), key, n);
  }

  bool remove_all(const Key &key)
  {
    return _remove_all(begin().get_node(), key);
  }

  bool remove_interval(const Key &from, const Key &to)
  {
    bool success = false;
    _remove_interval(begin().get_node(), from, to, success);

    return success;
  }

  bool remove_exactly_last(const Key &key, unsigned int n = 1)
  {
    return (_remove_exactly_last(_start->_next, key, n) == n);
  }

  void push_with_priority(const Key &key, const Info &info)
  {
    // singly-linked list
    if (!_push_with_priority(begin().get_node(), key, info))
    {
      // push front
      Node *new_node = new Node(key, info, nullptr, _start->_next);
      _start->_next = new_node;
    }
  }

  bool insert_pair_at(const Key &position, const Key &key_before, const Info &info_before, const Key &key_after, const Info &info_after)
  {
    return _insert_pair_at(begin().get_node(), position, key_before, info_before, key_after, info_after);
  }

  bool insert_before_each(const Key &position, const Key &key, const Info &info)
  {
    // singly-linked list
    return _insert_before_each(_start->_next, position, key, info);
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