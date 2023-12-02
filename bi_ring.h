#ifndef RING_HPP
#define RING_HPP

#include <iostream>
using namespace std;

template <typename Key, typename Info>
class Ring {
private:
  /////////////////////////////// NODE //////////////////////////////
  class Node {

  public:
    Key key;
    Info info;
    Node(const Key& k, const Info& i, Node* p = nullptr, Node* n = nullptr)
      : key(k), info(i), _past(p ? p : this), _next(n ? n : this) { }

  private:
    Node* _past;
    Node* _next;
    friend class Ring<Key, Info>;
  };
  /////////////////////////////// NODE //////////////////////////////

  Node* _sentinel;
  unsigned int _size;

  bool _is_sentinel(Node* node) const { return node == _sentinel; }

  Node* _push(Node* before, Node* after, const Key& key, const Info& info) {
    Node* new_node = new Node(key, info, before, after);

    before->_next = new_node;
    after->_past = new_node;

    _size++;

    return new_node;
  }

  Node* _pop(Node* node) {
    if (_is_sentinel(node))
      return _sentinel;

    Node* to_delete = node;
    Node* next_node = node->_next;

    node->_next->_past = node->_past;
    node->_past->_next = node->_next;

    delete to_delete;
    _size--;

    return next_node; // returns _sentinel if the last node is deleted
  }

  /////////////////////////////// ITERATORS //////////////////////////////
  template <typename KeyType, typename InfoType>
  class IteratorBase {
  private:
    const Ring<Key, Info>* _owner;
    Node* _curr;
    friend class Ring<Key, Info>;
    IteratorBase(Node* node, const Ring* owner) : _owner(owner), _curr(node) { }

  public:
    // jump over sentinel
    IteratorBase& operator++() {
      _curr = _owner->_is_sentinel(_curr->_next) ?
        _curr->_next->_next : _curr->_next;

      return static_cast<IteratorBase&>(*this);
    }

    IteratorBase operator++(int) {
      IteratorBase temp = static_cast<IteratorBase&>(*this);
      ++(*this);
      return temp;
    }

    IteratorBase& operator--() {
      _curr = _owner->_is_sentinel(_curr->_past) ?
        _curr->_past->_past : _curr->_past;

      return static_cast<IteratorBase&>(*this);
    }

    IteratorBase operator--(int) {
      IteratorBase temp = static_cast<IteratorBase&>(*this);
      --(*this);
      return temp;
    }

    bool operator==(const IteratorBase& other) const {
      return _curr == other._curr;
    }

    bool operator!=(const IteratorBase& other) const {
      return _curr != other._curr;
    }

    KeyType& key() { return this->_curr->key; }
    InfoType& info() { return this->_curr->info; }
  };

public:
  typedef IteratorBase<Key, Info> Iterator;
  typedef IteratorBase<const Key, const Info> ConstIterator;

  Iterator begin() { return Iterator(_sentinel->_next, this); }
  ConstIterator cbegin() const { return ConstIterator(_sentinel->_next, this); }

  Iterator end() { return Iterator(_sentinel, this); }
  ConstIterator cend() const { return ConstIterator(_sentinel, this); }
  /////////////////////////////// ITERATORS //////////////////////////////

private:
  Iterator _find(const Key& search_key, Iterator from, Iterator to) {
    for (Iterator it = from; it != to; it++) {
      if (it.key() == search_key) {
        return it;
      }
    }

    return to;
  }

public:
  Ring() : _sentinel(new Node(Key{}, Info{})), _size(0) { }

  ~Ring() {
    clear();
    delete _sentinel;
  }

  Ring& operator=(const Ring& src) {
    if (this != &src) {
      clear();

      ConstIterator it_last = --(src.cend());
      ConstIterator it_sentinel = --(src.cbegin());

      for (ConstIterator it = it_last; it != it_sentinel; it--) {
        push_front(it.key(), it.info());
      }
    }

    return *this;
  }

  Ring(const Ring& src) : Ring() { *this = src; }

  unsigned int size() {
    return _size;
  }

  Iterator push_front(const Key& key, const Info& info) {
    return Iterator(_push(_sentinel, _sentinel->_next, key, info), this);
  }

  Iterator pop_front() {
    return Iterator(_pop(_sentinel->_next), this);
  }

  Iterator erase(Iterator position) {
    return Iterator(_pop(position._curr), this);
  }

  Iterator insert(Iterator position, const Key& key, const Info& info) {
    return Iterator(_push(position._curr->_past, position._curr, key, info), this);
  }

  void clear() {
    while (_size) {
      pop_front();
    }
  }

  Iterator find(const Key& search_key) {
    return _find(search_key, begin(), end());
  }

  Iterator find(const Key& search_key, Iterator from, Iterator to) {
    return _find(search_key, from, to);
  }
};

template <typename Key, typename Info>
ostream& operator<<(ostream& os, const Ring<Key, Info>& ring) {
  for (typename Ring<Key, Info>::ConstIterator it = ring.cbegin(); it != ring.cend(); ++it) {
    os << it.key() << " : " << it.info() << endl;
  }
  return os;
}

//////////////////////////// EXTERNAL //////////////////////////////////

template <typename Key, typename Info>
Ring<Key, Info> filter(const Ring<Key, Info>& src, bool (*pred)(const Key&)) {
  Ring<Key, Info> result;

  for (typename Ring<Key, Info>::ConstIterator it = --(src.cend()); it != --(src.cbegin()); it--) {
    if (pred(it.key())) {
      result.push_front(it.key(), it.info());
    }
  }

  return result;
}

template <typename Key, typename Info>
Ring<Key, Info> unique(const Ring<Key, Info>& src, Info(aggregate)(const Key&, const Info&, const Info&)) {
  Ring<Key, Info> result;

  for (typename Ring<Key, Info>::ConstIterator it = --(src.cend()); it != --(src.cbegin()); it--) {
    typename Ring<Key, Info>::Iterator it_res = result.find(it.key());

    if (it_res != result.end()) {
      // if key's been added to result previously
      it_res.info() = aggregate(it.key(), it_res.info(), it.info());
    } else {
      result.push_front(it.key(), it.info());
    }
  }

  return result;
}

template <typename Key, typename Info>
Info _concatenate_info(const Key&, const Info& i1, const Info& i2) {
  return i1 + i2;
}

template <typename Key, typename Info>
Ring<Key, Info> join(const Ring<Key, Info>& first, const Ring<Key, Info>& second) {
  Ring<Key, Info> result;

  for (typename Ring<Key, Info>::ConstIterator it = --(first.cend()); it != --(first.cbegin()); it--) {
    result.push_front(it.key(), it.info());
  }

  for (typename Ring<Key, Info>::ConstIterator it = --(second.cend()); it != --(second.cbegin()); it--) {
    result.push_front(it.key(), it.info());
  }

  return unique(result, _concatenate_info<Key, Info>);
}

template <typename Key, typename Info>
Ring<Key, Info> _reverse(const Ring<Key, Info>& ring) {
  Ring<Key, Info> reversed;

  for (typename Ring<Key, Info>::ConstIterator it = ring.cbegin(); it != ring.cend(); it++) {
    reversed.push_front(it.key(), it.info());
  }

  return reversed;
}

template <typename Key, typename Info>
Ring<Key, Info> shuffle(
  const Ring<Key, Info>& first, unsigned int fcnt,
  const Ring<Key, Info>& second, unsigned int scnt,
  unsigned int reps) {
  Ring<Key, Info> result;

  typename Ring<Key, Info>::ConstIterator it_first = first.cbegin();
  typename Ring<Key, Info>::ConstIterator it_second = second.cbegin();

  for (unsigned int rep = 0; rep < reps; rep++) {
    for (unsigned int i = 0; i < fcnt; i++) {
      result.push_front(it_first.key(), it_first.info());

      it_first++;
      if (it_first == first.cend()) {
        it_first = first.cbegin(); // if _sentinel is reached reset the iterator
      }
    }

    for (unsigned int i = 0; i < scnt; i++) {
      result.push_front(it_second.key(), it_second.info());

      it_second++;
      if (it_second == second.cend()) {
        it_second = second.cbegin();
      }
    }
  }

  return _reverse(result);
}

#endif // RING_HPP