#include "sequence.hpp"
#include <string>

#include <cassert>

using namespace std;

/// SEQUENCE ///

template class Sequence<unsigned int, string>; // should be fixed later

// core methods
template <typename Key, typename Info>
Sequence<Key, Info>::Sequence() : _head(nullptr), _tail(nullptr), _size(0){};

template <typename Key, typename Info>
Sequence<Key, Info>::~Sequence(){};

template <typename Key, typename Info>
unsigned int Sequence<Key, Info>::length() const
{
  return _size;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::isEmpty() const
{
  return _size == 0;
}

// insertion methods
template <typename Key, typename Info>
void Sequence<Key, Info>::push_front(const Key &key, const Info &info)
{
  _head = new Node(key, info, _head);

  if (isEmpty())
    _tail = _head;

  _size++;
}

template <typename Key, typename Info>
void Sequence<Key, Info>::push_back(const Key &key, const Info &info)
{
  Node *new_node = new Node(key, info);

  if (isEmpty())
  {
    _head = new_node;
    _tail = new_node;
  }
  else
  {
    _tail->_next = new_node;
    _tail = new_node;
  }

  _size++;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::insert_after(const Key &key, const Info &info, const Key &target_key, unsigned int occurrence)
{
  Node *targetNode = nullptr;
  if (!search(targetNode, target_key, occurrence))
    return false;

  Node *newNode = new Node(key, info, targetNode->_next);
  targetNode->_next = newNode;

  if (targetNode == _tail)
    _tail = newNode;

  _size++;
  return true;
}

// retrieval methods
template <typename Key, typename Info>
bool Sequence<Key, Info>::front(Info &info, Key &key) const
{
  if (isEmpty())
    return false;

  Iterator iter = begin();
  key = iter.key();
  info = iter.info();
  return true;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::search(Node *&foundNode, const Key &key, unsigned int occurrence) const
{
  foundNode = nullptr;

  if (occurrence == 0)
    return false;

  // can't use implemented iterator here because it has no access to the node

  for (Node *current = _head; current != nullptr; current = current->_next)
  {
    if (current->key == key && --occurrence == 0)
    {
      foundNode = current;
      return true;
    }
  }

  return false;
}

/// NODE ///

template <typename Key, typename Info>
Sequence<Key, Info>::Node::Node(const Key &k, const Info &i, Node *n)
    : key(k), info(i), _next(n) {}

/// ITERATOR ///
template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator Sequence<Key, Info>::begin() const
{
  return Iterator(_head);
}

template <typename Key, typename Info>
Sequence<Key, Info>::Iterator::Iterator(Node *ptr) : _current(ptr) {}

template <typename Key, typename Info>
Sequence<Key, Info>::Iterator::~Iterator() {}

template <typename Key, typename Info>
Sequence<Key, Info>::Iterator::Iterator(const Iterator &src)
{
  *this = src;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator &Sequence<Key, Info>::Iterator::operator=(const Iterator &src)
{
  if (this != &src)
  {
    _current = src._current;
  }
  return *this;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::Iterator::operator==(const Iterator &src) const
{
  return _current == src._current;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::Iterator::operator!=(const Iterator &src) const
{
  return _current != src._current;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator &Sequence<Key, Info>::Iterator::operator++()
{
  assert(_current != nullptr && "Incrementing a null iterator.");
  _current = _current->_next;
  return *this;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator Sequence<Key, Info>::Iterator::operator++(int)
{
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Iterator Sequence<Key, Info>::Iterator::operator+(unsigned int interval)
{
  Iterator temp = *this;
  while (interval-- > 0 && temp._current != nullptr)
  {
    ++temp;
  }
  return temp;
}

template <typename Key, typename Info>
Key &Sequence<Key, Info>::Iterator::key() const
{
  assert(_current != nullptr && "Attempting to access key of a null iterator.");
  return _current->key;
}

template <typename Key, typename Info>
Info &Sequence<Key, Info>::Iterator::info() const
{
  assert(_current != nullptr && "Attempting to access info of a null iterator.");
  return _current->info;
}
