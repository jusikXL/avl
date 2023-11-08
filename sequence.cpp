#include "sequence.hpp"

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

/// SEQUENCE ///

template class Sequence<unsigned int, string>; // should be fixed
template ostream& operator<<(ostream& os, const Sequence<unsigned int, string>& sequence);
template
void split_pos(const Sequence<unsigned int, string>& seq, int start_pos, int len1, int len2, int count, Sequence<unsigned int, string>& seq1, Sequence<unsigned int, string>& seq2);
template
void split_key(Sequence<unsigned int, string>& seq, const unsigned int& start_key, int start_occ, int len1, int len2, int count, Sequence<unsigned int, string>& seq1, Sequence<unsigned int, string>& seq2);

// core methods
template <typename Key, typename Info>
Sequence<Key, Info>::Sequence() : _head(nullptr), _tail(nullptr), _size(0) { };

template <typename Key, typename Info>
Sequence<Key, Info>::~Sequence() {
  clear();
};

template <typename Key, typename Info>
Sequence<Key, Info>::Sequence(const Sequence& src) {
  *this = src;
};

template <typename Key, typename Info>
Sequence<Key, Info>& Sequence<Key, Info>::operator=(const Sequence<Key, Info>& src) {
  if (this != &src) {
    clear();

    for (Node* curr = src._head; curr != nullptr; curr = curr->_next)
      push_back(curr->key, curr->info);
  }
  return *this;
}

template <typename Key, typename Info>
unsigned int Sequence<Key, Info>::length() const {
  return _size;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::is_empty() const {
  return _size == 0;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Node* Sequence<Key, Info>::_get_node(const Key& target_key, unsigned int target_occurrence) const {
  Node* current = _head;
  while (current != nullptr && target_occurrence > 0) {
    if (current->key == target_key && --target_occurrence == 0)
      return current;

    current = current->_next;
  }

  return nullptr;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Node* Sequence<Key, Info>::_get_node_before(const Key& target_key, unsigned int target_occurrence) const {
  Node* target_node = _get_node(target_key, target_occurrence);
  if (!target_node || target_node == _head)
    return nullptr;

  // transverse to the node before the target
  Node* current = _head;
  while (current->_next != target_node) {
    current = current->_next;
  }

  return current;
}

// insertion methods
template <typename Key, typename Info>
void Sequence<Key, Info>::push_front(const Key& key, const Info& info) {
  _head = new Node(key, info, _head);

  if (is_empty())
    _tail = _head;

  _size++;
}

template <typename Key, typename Info>
void Sequence<Key, Info>::push_back(const Key& key, const Info& info) {
  Node* new_node = new Node(key, info);

  if (is_empty()) {
    _head = _tail = new_node;
  } else {
    _tail->_next = new_node;
    _tail = new_node;
  }

  _size++;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::insert_after(const Key& key, const Info& info, const Key& target_key, unsigned int target_occurrence) {
  Node* target_node = _get_node(target_key, target_occurrence);

  if (target_node == nullptr)
    return false; // the target node was not found

  if (target_node == _tail) {
    push_back(key, info); // if the target node is the tail, it is push back
    return true;
  }

  Node* new_node = new Node(key, info, target_node->_next); // create a new node pointing to the next of the target node
  target_node->_next = new_node; // now target node points to the new node
  _size++;

  return true;
}

// retrieval methods

template <typename Key, typename Info>
bool Sequence<Key, Info>::search(const Key& target_key, unsigned int target_occurrence) const {
  if (target_occurrence == 0)
    return false;

  for (Iterator it = begin(); it != end(); ++it) {
    if (it.key() == target_key && --target_occurrence == 0)
      return true;
  }

  return false;
}

template <typename Key, typename Info>
unsigned int Sequence<Key, Info>::search(const Key& target_key) const {
  unsigned int occurences = 0;
  for (Iterator it = begin(); it != end(); ++it) {
    if (it.key() == target_key)
      ++occurences;
  }
  return occurences;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::front(Info& info, Key& key) const {
  if (is_empty())
    return false;

  key = _head->key;
  info = _head->info;

  return true;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::back(Info& info, Key& key) const {
  if (is_empty())
    return false;

  key = _tail->key;
  info = _tail->info;

  return true;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::get_info(Info& info, const Key& target_key, unsigned int target_occurrence) const {
  if (target_occurrence == 0)
    return false;

  for (Iterator it = begin(); it != end(); ++it) {
    if (it.key() == target_key && --target_occurrence == 0) {
      info = it.info();
      return true;
    }
  }

  return false;
}

// removal methods
template <typename Key, typename Info>
bool Sequence<Key, Info>::pop_front() {
  if (is_empty())
    return false;

  Node* temp = _head;
  _head = _head->_next;
  delete temp;

  if (_head == nullptr)
    // if head is nullptr it means the next node of the head is nullptr so head is also tail
    _tail = nullptr;

  _size--;

  return true;
}

template <typename Key, typename Info>
void Sequence<Key, Info>::clear() {
  while (pop_front()) { }
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::pop_back() {
  if (is_empty())
    return false;

  if (length() == 1) {
    delete _tail;
    _head = _tail = nullptr;
    _size = 0;
  } else {
    Node* before_tail = _get_node_before(_tail->key, search(_tail->key));

    delete _tail;
    _tail = before_tail;
    _tail->_next = nullptr;

    _size--;
  }

  return true;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::remove(const Key& target_key, unsigned int target_occurrence) {
  if (is_empty())
    return false;

  if (_head->key == target_key && target_occurrence == 1)
    return pop_front();

  if (_tail->key == target_key && target_occurrence == search(target_key))
    return pop_back();

  Node*
    before_target_node = _get_node_before(target_key, target_occurrence);
  if (before_target_node == nullptr)
    // the node before the target was not found
    return false;

  Node* target_node = before_target_node->_next;
  before_target_node->_next = target_node->_next;
  delete target_node;
  _size--;

  return true;
}

/// NODE ///

template <typename Key, typename Info>
Sequence<Key, Info>::Node::Node(const Key& k, const Info& i, Node* n)
  : key(k), info(i), _next(n) { }

/// ITERATOR ///
template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator Sequence<Key, Info>::begin() const {
  return Iterator(_head);
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator Sequence<Key, Info>::end() const {
  return Iterator(nullptr);
}

template <typename Key, typename Info>
Sequence<Key, Info>::Iterator::Iterator(Node* ptr) : _current(ptr) { }

template <typename Key, typename Info>
Sequence<Key, Info>::Iterator::~Iterator() { }

template <typename Key, typename Info>
Sequence<Key, Info>::Iterator::Iterator(const Iterator& src) {
  *this = src;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator& Sequence<Key, Info>::Iterator::operator=(const Iterator& src) {
  if (this != &src) {
    _current = src._current;
  }
  return *this;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::Iterator::operator==(const Iterator& src) const {
  return _current == src._current;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::Iterator::operator!=(const Iterator& src) const {
  return _current != src._current;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator& Sequence<Key, Info>::Iterator::operator++() {
  assert(_current != nullptr && "Incrementing a null iterator.");
  _current = _current->_next;
  return *this;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator Sequence<Key, Info>::Iterator::operator++(int) {
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator Sequence<Key, Info>::Iterator::operator+(unsigned int interval) {
  Iterator temp = *this;
  while (interval-- > 0 && temp._current != nullptr) {
    ++temp;
  }
  return temp;
}

template <typename Key, typename Info>
Key& Sequence<Key, Info>::Iterator::key() const {
  assert(_current != nullptr && "Attempting to access key of a null iterator.");
  return _current->key;
}

template <typename Key, typename Info>
Info& Sequence<Key, Info>::Iterator::info() const {
  assert(_current != nullptr && "Attempting to access info of a null iterator.");
  return _current->info;
}

template <typename Key, typename Info>
ostream& operator<<(ostream& os, const Sequence<Key, Info>& sequence) {
  typename Sequence<Key, Info>::Iterator it = sequence.begin();

  os << "[";
  while (it != sequence.end()) {
    os << "(" << it.key() << ", " << it.info() << ")";
    ++it;
    if (it != sequence.end()) {
      os << ", ";
    }
  }
  os << "]";

  return os;
}

template <typename Key, typename Info>
void split_pos(const Sequence<Key, Info>& seq, int start_pos, int len1, int len2, int count, Sequence<Key, Info>& seq1, Sequence<Key, Info>& seq2) {
  seq1.clear();
  seq2.clear();

  Sequence<Key, Info> temp; // Temporary sequence to store the remaining elements.
  typename Sequence<Key, Info>::Iterator it = seq.begin();

  // Move the iterator to the start position.
  // Stop if iterator reached the end of the original sequence.
  for (int i = 0; i < start_pos && it != seq.end(); ++i, ++it) {
    temp.push_back(it.key(), it.info());
  }

  // Perform the splitting `count` times. Stop if iterator reached the end of the original sequence.
  while (count > 0 && it != seq.end()) {
    // Move `len1` elements to seq1. Stop if iterator reached the end of the original sequence.
    for (int i = 0; i < len1 && it != seq.end(); ++i, ++it) {
      seq1.push_back(it.key(), it.info());
    }

    // Move `len2` elements to seq2. Stop if iterator reached the end of the original sequence.
    for (int i = 0; i < len2 && it != seq.end(); ++i, ++it) {
      seq2.push_back(it.key(), it.info());
    }

    count--;
  }

  while (it != seq.end()) {
    temp.push_back(it.key(), it.info());
    ++it;
  }

  // can be done because split_pos is declared as a friend class
  Sequence<Key, Info>& modifiable_seq = const_cast<Sequence<Key, Info>&>(seq);
  modifiable_seq.clear();
  modifiable_seq = temp;
}

template <typename Key, typename Info>
void split_key(Sequence<Key, Info>& seq, const Key& start_key, int start_occ, int len1, int len2, int count, Sequence<Key, Info>& seq1, Sequence<Key, Info>& seq2) {
  seq1.clear();
  seq2.clear();

  if (!seq.search(start_key, start_occ)) return; // Start node not found, exit the function.

  Sequence<Key, Info> temp;
  typename Sequence<Key, Info>::Iterator it = seq.begin();

  // Move the iterator to the start position.
  // Stop if iterator reached the end of the original sequence.
  while (it != seq.end()) {
    if (it.key() == start_key && --start_occ == 0) {
      break;
    }
    temp.push_back(it.key(), it.info());
    ++it;
  }

  // Perform the splitting `count` times. Stop if iterator reached the end of the original sequence.
  while (count > 0 && it != seq.end()) {
    // Move `len1` elements to seq1. Stop if iterator reached the end of the original sequence.
    for (int i = 0; i < len1 && it != seq.end(); ++i, ++it) {
      seq1.push_back(it.key(), it.info());
    }

    // Move `len2` elements to seq2. Stop if iterator reached the end of the original sequence.
    for (int i = 0; i < len2 && it != seq.end(); ++i, ++it) {
      seq2.push_back(it.key(), it.info());
    }

    count--;
  }

  while (it != seq.end()) {
    temp.push_back(it.key(), it.info());
    ++it;
  }

  // can be done because split_pos is declared as a friend class
  Sequence<Key, Info>& modifiable_seq = const_cast<Sequence<Key, Info>&>(seq);
  modifiable_seq.clear();
  modifiable_seq = temp;
}
