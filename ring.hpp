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

  bool is_sentinel(Node *node) const { return node == _sentinel; }

  Node *_push(Node *before, Node *after, const Key &key, const Info &info)
  {
    Node *new_node = new Node(key, info, before, after);

    before->_next = new_node;
    after->_past = new_node;

    size++;
    return new_node;
  }

  Node *_pop(Node *node)
  {
    if (is_sentinel(node))
      return nullptr;

    Node *to_delete = node;

    node->_next->_past = node->_past;
    node->_past->_next = node->_next;

    delete to_delete;
    size--;

    return node->_next;
  }

public:
  Ring() : size(0)
  {
    _sentinel = new Node(Key{}, Info{});
  }

  unsigned int size;

  /////////////////////////////// ITERATORS //////////////////////////////
  class Iterator
  {
  private:
    Node *_curr;
    friend class Ring<Key, Info>;

  public:
    Iterator(Node *node) : _curr(node) {}

    Key &key()
    {
      return _curr->key;
    }

    Info &info()
    {
      return _curr->info;
    }

    Iterator &operator++()
    {
      _curr = _curr->_next;
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    Iterator &operator--()
    {
      _curr = _curr->_past;
      return *this;
    }

    Iterator operator--(int)
    {
      Iterator temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const Iterator &other) const
    {
      return _curr == other._curr;
    }

    bool operator!=(const Iterator &other) const
    {
      return _curr != other._curr;
    }
  };

  class ConstIterator : public Iterator
  {
  public:
    using Iterator::Iterator;

    const Key &key() const
    {
      return Iterator::_curr->key;
    }

    const Info &info() const
    {
      return Iterator::_curr->info;
    }
  };

  /////////////////////////////// ITERATORS //////////////////////////////

  Iterator begin() const { return Iterator(_sentinel->_next); }
  ConstIterator cbegin() const { return ConstIterator(_sentinel->_next); }

  Iterator end() const { return Iterator(_sentinel); }
  ConstIterator cend() const { return ConstIterator(_sentinel); }

  Iterator push_front(const Key &key, const Info &info)
  {
    return Iterator(_push(_sentinel, _sentinel->_next, key, info));
  }

  Iterator pop_front()
  {
    return Iterator(_pop(_sentinel->_next));
  }

  void clear()
  {
    while (size)
    {
      _pop(_sentinel->_next);
    }
  }
};

template <typename Key, typename Info>
ostream &operator<<(ostream &os, const Ring<Key, Info> &ring)
{
  for (typename Ring<Key, Info>::ConstIterator it = ring.cbegin(); it != ring.cend(); ++it)
  {
    os << "Key: " << it.key() << ", Info: " << it.info() << endl;
  }
  return os;
}

#endif // RING_HPP