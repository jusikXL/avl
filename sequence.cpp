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
Sequence<Key, Info>::~Sequence()
{
  clear();
};

template <typename Key, typename Info>
Sequence<Key, Info>::Sequence(const Sequence &src)
{
  *this = src;
};

template <typename Key, typename Info>
Sequence<Key, Info> &Sequence<Key, Info>::operator=(const Sequence<Key, Info> &src)
{
  if (this != &src)
  {
    clear();

    for (Node *curr = src._head; curr != nullptr; curr = curr->_next)
      push_back(curr->key, curr->info);
  }
  return *this;
}

template <typename Key, typename Info>
unsigned int Sequence<Key, Info>::length() const
{
  return _size;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::is_empty() const
{
  return _size == 0;
}

// insertion methods
template <typename Key, typename Info>
void Sequence<Key, Info>::push_front(const Key &key, const Info &info)
{
  _head = new Node(key, info, _head);

  if (is_empty())
    _tail = _head;

  _size++;
}

template <typename Key, typename Info>
void Sequence<Key, Info>::push_back(const Key &key, const Info &info)
{
  Node *new_node = new Node(key, info);

  if (is_empty())
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
  Node *targetNode = _get_node(target_key, occurrence);
  if (targetNode == nullptr)
    return false;

  Node *newNode = new Node(key, info, targetNode->_next);

  targetNode->_next = newNode;
  if (targetNode == _tail)
    _tail = newNode;

  _size++;

  return true;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Node *Sequence<Key, Info>::_get_node(const Key &key, unsigned int occurrence) const
{
  // can't use iterator here since it is safe
  Node *current = _head;
  while (current != nullptr && occurrence > 0)
  {
    if (current->key == key && --occurrence == 0)
      return current;

    current = current->_next;
  }

  return nullptr;
}

template <typename Key, typename Info>
typename Sequence<Key, Info>::Node *Sequence<Key, Info>::_get_node_before(const Key &key, unsigned int occurrence) const
{
  Node *target = _get_node(key, occurrence);
  if (!target || target == _head)
    return nullptr;

  // we are interested in the node before the target
  Node *current = _head;
  while (current->_next != target)
  {
    current = current->_next;
  }

  return current;
}

// retrieval methods

template <typename Key, typename Info>
bool Sequence<Key, Info>::search(const Key &key, unsigned int occurrence) const
{
  if (occurrence == 0)
    return false;

  for (Iterator it = begin(); it != end(); ++it)
  {
    if (it.key() == key && --occurrence == 0)
      return true;
  }

  return false;
}

template <typename Key, typename Info>
unsigned int Sequence<Key, Info>::search(const Key &key) const
{
  unsigned int count = 0;
  for (Iterator it = begin(); it != end(); ++it)
  {
    if (it.key() == key)
      ++count;
  }
  return count;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::front(Info &info, Key &key) const
{
  if (is_empty())
    return false;

  key = _head->key;
  info = _head->info;

  return true;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::back(Info &info, Key &key) const
{
  if (is_empty())
    return false;

  key = _tail->key;
  info = _tail->info;

  return true;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::get_info(Info &info, const Key &key, unsigned int occurrence) const
{
  if (occurrence == 0)
    return false;

  for (Iterator it = begin(); it != end(); ++it)
  {
    if (it.key() == key && --occurrence == 0)
    {
      info = it.info();
      return true;
    }
  }

  return false;
}

// removal methods

template <typename Key, typename Info>
bool Sequence<Key, Info>::pop_front()
{
  if (is_empty())
    return false;

  Node *temp = _head;
  _head = _head->_next;
  delete temp;

  if (_head == nullptr)
    _tail = nullptr;

  _size--;

  return true;
}

template <typename Key, typename Info>
void Sequence<Key, Info>::clear()
{
  while (pop_front())
  {
  }
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::pop_back()
{
  if (is_empty())
    return false;

  if (length() == 1)
  {
    delete _tail;
    _head = _tail = nullptr;
    _size = 0;
  }

  else
  {
    Node *before_tail = _get_node_before(_tail->key, search(_tail->key));

    delete _tail;
    _tail = before_tail;
    _tail->_next = nullptr;
    _size--;
  }

  return true;
}

template <typename Key, typename Info>
bool Sequence<Key, Info>::remove(const Key &key, unsigned int occurrence)
{
  if (is_empty())
    return false;

  if (_head->key == key && occurrence == 1)
    return pop_front();

  if (_tail->key == key && occurrence == search(key))
    return pop_back();

  // Find the node right before the one to be removed
  Node *
      before_target = _get_node_before(key, occurrence);
  if (before_target == nullptr)
    return false;

  // Remove the target node which is next to the before_target node
  Node *target = before_target->_next;
  before_target->_next = target->_next;
  delete target;
  _size--;

  return true;
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
typename Sequence<Key, Info>::Iterator Sequence<Key, Info>::end() const
{
  return Iterator(nullptr);
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
