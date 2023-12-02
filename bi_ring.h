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
    pair<Key, Info> data;
    Node(const Key& k, const Info& i, Node* p = nullptr, Node* n = nullptr)
      : data(k, i), _past(p ? p : this), _next(n ? n : this) { }

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
  template <typename DataType>
  class IteratorBase {
  private:
    friend class Ring<Key, Info>;

    const Ring<Key, Info>* _owner;
    Node* _curr;

    IteratorBase(Node* node, const Ring* owner) : _owner(owner), _curr(node) { }

  public:
    IteratorBase& operator++() {
      _curr = _owner->_is_sentinel(_curr->_next) ?
        _curr->_next->_next : _curr->_next;

      return *this;
    }

    IteratorBase operator++(int) {
      IteratorBase temp = *this;
      ++(*this);
      return temp;
    }

    IteratorBase& operator--() {
      _curr = _owner->_is_sentinel(_curr->_past) ?
        _curr->_past->_past : _curr->_past;

      return *this;
    }

    IteratorBase operator--(int) {
      IteratorBase temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const IteratorBase& other) const {
      return _curr == other._curr;
    }

    bool operator!=(const IteratorBase& other) const {
      return _curr != other._curr;
    }

    DataType& operator*() const {
      return _curr->data;
    }

    DataType* operator->() const {
      return &_curr->data;
    }

    IteratorBase& next() {
      _curr = _curr->_next;
      return *this;
    }

    IteratorBase& past() {
      _curr = _curr->_past;
      return *this;
    }
  };

public:
  typedef IteratorBase<pair<Key, Info>> Iterator;
  typedef IteratorBase<const pair<Key, Info>> ConstIterator;

  Iterator begin() { return Iterator(_sentinel->_next, this); }
  ConstIterator cbegin() const { return ConstIterator(_sentinel->_next, this); }

  Iterator end() { return Iterator(_sentinel, this); }
  ConstIterator cend() const { return ConstIterator(_sentinel, this); }
  /////////////////////////////// ITERATORS //////////////////////////////

private:
  Iterator _find(const Key& search_key, Iterator from, Iterator to) {
    for (Iterator it = from; it != to; it.next()) {
      if (it->first == search_key) {
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

      for (ConstIterator it = --(src.cend()); it != src.cend(); it.past()) {
        push_front(it->first, it->second);
      }
    }

    return *this;
  }

  Ring(const Ring& src) : Ring() { *this = src; }

  unsigned int size() const {
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
  for (auto it = ring.cbegin(); it != ring.cend(); it.next()) {
    os << it->first << " : " << it->second << endl;
  }

  return os;
}

//////////////////////////// EXTERNAL //////////////////////////////////

template <typename Key, typename Info>
Ring<Key, Info> filter(const Ring<Key, Info>& src, bool (*pred)(const Key&)) {
  Ring<Key, Info> result;

  for (auto it = --(src.cend()); it != src.cend(); it.past()) {
    if (pred(it->first)) {
      result.push_front(it->first, it->second);
    }
  }

  return result;
}

template <typename Key, typename Info>
Ring<Key, Info> unique(const Ring<Key, Info>& src, Info(aggregate)(const Key&, const Info&, const Info&)) {
  Ring<Key, Info> result;

  for (auto it = --(src.cend()); it != src.cend(); it.past()) {
    auto found_it = result.find(it->first);

    if (found_it != result.end()) {
      // if key's been added to result previously
      found_it->second = aggregate(it->first, found_it->second, it->second);
    } else {
      result.push_front(it->first, it->second);
    }

    return result;
  }
}

template <typename Key, typename Info>
Info _concatenate_info(const Key&, const Info& i1, const Info& i2) {
  return i1 + i2;
}

template <typename Key, typename Info>
Ring<Key, Info> join(const Ring<Key, Info>& first, const Ring<Key, Info>& second) {
  Ring<Key, Info> result;

  for (auto it = --(first.cend()); it != first.cend(); it.past()) {
    result.push_front(it->first, it->second);
  }

  for (auto it = --(second.cend()); it != second.cend(); it.past()) {
    result.push_front(it->first, it->second);
  }

  return unique(result, _concatenate_info<Key, Info>);
}

template <typename Key, typename Info>
Ring<Key, Info> _reverse(const Ring<Key, Info>& ring) {
  Ring<Key, Info> reversed;

  for (typename Ring<Key, Info>::ConstIterator it = ring.cbegin(); it != ring.cend(); it.next()) {
    reversed.push_front(it->first, it->second);
  }

  return reversed;
}

template <typename Key, typename Info>
Ring<Key, Info> shuffle(
  const Ring<Key, Info>& first, unsigned int fcnt,
  const Ring<Key, Info>& second, unsigned int scnt,
  unsigned int reps) {
  Ring<Key, Info> result;

  auto first_it = first.cbegin();
  auto second_it = second.cbegin();

  for (unsigned int rep = 0; rep < reps; rep++) {
    for (unsigned int i = 0; i < fcnt; i++) {
      result.push_front(first_it->first, first_it->second);

      first_it++;
    }

    for (unsigned int i = 0; i < scnt; i++) {
      result.push_front(second_it->first, second_it->second);

      second_it++;
    }
  }

  return _reverse(result);
}

#endif // RING_HPP