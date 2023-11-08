#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

template <typename Key, typename Info>
class Sequence;

template <typename Key, typename Info>
ostream& operator<<(ostream& os, const Sequence<Key, Info>& sequence);

template <typename Key, typename Info>
void split_pos(const Sequence<Key, Info>& seq, int start_pos, int len1, int len2, int count, Sequence<Key, Info>& seq1, Sequence<Key, Info>& seq2);

template <typename Key, typename Info>
void split_key(Sequence<Key, Info>& seq, const Key& start_key, int start_occ, int len1, int len2, int count, Sequence<Key, Info>& seq1, Sequence<Key, Info>& seq2);

template <typename Key, typename Info>
class Sequence {
private:
  class Node {
    friend class Sequence;

  public:
    Key key;
    Info info;
    Node(const Key& k, const Info& i, Node* n = nullptr)
      : key(k), info(i), _next(n) { };

  private:
    Node* _next;
  };

  Node* _head;
  Node* _tail;
  unsigned int _size;

  /**
   * Private method to get the node of a given key and occurence
   * cannot be replaced with iterators because they are safe (have no access to next)
   */
  Node* _get_node(const Key& target_key, unsigned int target_occurrence) const {
    Node* current = _head;
    while (current != nullptr && target_occurrence > 0) {
      if (current->key == target_key && --target_occurrence == 0)
        return current;

      current = current->_next;
    }

    return nullptr;
  };
  Node* _get_node_before(const Key& target_key, unsigned int target_occurrence) const {
    Node* target_node = _get_node(target_key, target_occurrence);
    if (!target_node || target_node == _head)
      return nullptr;

    // transverse to the node before the target
    Node* current = _head;
    while (current->_next != target_node) {
      current = current->_next;
    }

    return current;
  };

public:
  Sequence() : _head(nullptr), _tail(nullptr), _size(0) { };
  ~Sequence() {
    clear();
  };
  Sequence(const Sequence& src) {
    *this = src;
  };
  Sequence& operator=(const Sequence<Key, Info>& src) {
    if (this != &src) {
      clear();

      for (Node* curr = src._head; curr != nullptr; curr = curr->_next)
        push_back(curr->key, curr->info);
    }
    return *this;
  };

  unsigned int length() const {
    return _size;
  };
  bool is_empty() const {
    return _size == 0;
  };
  friend ostream& operator<< <>(ostream& os, const Sequence<Key, Info>& sequence);

  friend void split_pos <>(const Sequence<Key, Info>& seq, int start_pos, int len1, int len2, int count, Sequence<Key, Info>& seq1, Sequence<Key, Info>& seq2);
  friend void split_key <>(Sequence<Key, Info>& seq, const Key& start_key, int start_occ, int len1, int len2, int count, Sequence<Key, Info>& seq1, Sequence<Key, Info>& seq2);

  /**
   * Inserts a new element with the provided key and info after the specified target element
   * of a given key and occurence.
   *
   * @param key The key of the new element to insert.
   * @param info The info of the new element to insert.
   * @param target_key The key after which the new element should be inserted.
   * @param target_occurrence Specifies af which occurrence of `target_key` to insert.
   * @return true if the element was successfully inserted, false otherwise.
   */
  bool insert_after(const Key& key, const Info& info, const Key& target_key, unsigned int target_occurrence = 1) {
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
  };

  void push_front(const Key& key, const Info& info) {
    _head = new Node(key, info, _head);

    if (is_empty())
      _tail = _head;

    _size++;
  };
  void push_back(const Key& key, const Info& info) {
    Node* new_node = new Node(key, info);

    if (is_empty()) {
      _head = _tail = new_node;
    } else {
      _tail->_next = new_node;
      _tail = new_node;
    }

    _size++;
  };

  /**
   * Removes the specified element of a given key and occurence.
   *
   * @param target_key The key of the element to remove.
   * @param target_occurrence Specifies which occurrence of the key to consider. Defaults to 1.
   * @return true if an element was successfully removed, false otherwise.
   */
  bool remove(const Key& target_key, unsigned int target_occurrence = 1) {
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
  };
  bool pop_front() {
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
  };
  bool pop_back() {
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
  };
  void clear() {
    while (pop_front()) { }
  };

  /**
   *
   * Retrieves the info associated to a specified element of a given key and occurence.
   *
   * @param[out] info Reference to where the info will be stored.
   * @param target_key The key for which to retrieve the associated info.
   * @param target_occurrence Specifies which occurrence of the key to consider. Defaults to 1.
   * @return true if the element was found and the associated info was retrieved, false otherwise.
   */
  bool get_info(Info& info, const Key& target_key, unsigned int target_occurrence) const {
    if (target_occurrence == 0)
      return false;

    for (Iterator it = begin(); it != end(); ++it) {
      if (it.key() == target_key && --target_occurrence == 0) {
        info = it.info();
        return true;
      }
    }

    return false;
  };
  bool front(Info& info, Key& key) const {
    if (is_empty())
      return false;

    key = _head->key;
    info = _head->info;

    return true;
  };
  bool back(Info& info, Key& key) const {
    if (is_empty())
      return false;

    key = _tail->key;
    info = _tail->info;

    return true;
  };

  /**
   * Searches for the specified element of a given key and occurence.
   *
   * @param target_key The key to search for.
   * @param target_occurrence Specifies which occurrence of the key to consider. Defaults to 1.
   * @return true if the specified element is found, false otherwise.
   */
  bool search(const Key& target_key, unsigned int target_occurrence) const {
    if (target_occurrence == 0)
      return false;

    for (Iterator it = begin(); it != end(); ++it) {
      if (it.key() == target_key && --target_occurrence == 0)
        return true;
    }

    return false;
  };

  /**
   * Counts the number of occurences of the given key.
   */
  unsigned int search(const Key& target_key) const {
    unsigned int occurences = 0;
    for (Iterator it = begin(); it != end(); ++it) {
      if (it.key() == target_key)
        ++occurences;
    }
    return occurences;
  };

  class Iterator {
  private:
    Node* _current;

  public:
    Iterator(Node* ptr = nullptr) : _current(ptr) { };
    ~Iterator() { };
    Iterator(const Iterator& src) {
      *this = src;
    };
    Iterator& operator=(const Iterator& src) {
      if (this != &src) {
        _current = src._current;
      }
      return *this;
    }
    ;

    bool operator==(const Iterator& src) const {
      return _current == src._current;
    }
    ;
    bool operator!=(const Iterator& src) const {
      return _current != src._current;
    };

    Iterator& operator++() {
      assert(_current != nullptr && "Incrementing a null iterator.");
      _current = _current->_next;
      return *this;
    };
    Iterator operator++(int) {
      Iterator temp = *this;
      ++(*this);
      return temp;
    };
    Iterator operator+(unsigned int interval) {
      Iterator temp = *this;
      while (interval-- > 0 && temp._current != nullptr) {
        ++temp;
      }
      return temp;
    };

    Key& key() const {
      assert(_current != nullptr && "Attempting to access key of a null iterator.");
      return _current->key;
    };
    Info& info() const {
      assert(_current != nullptr && "Attempting to access info of a null iterator.");
      return _current->info;
    };
  };

  Iterator begin() const {
    return Iterator(_head);
  };
  Iterator end() const {
    return Iterator(nullptr);
  };
};


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

#endif // SEQUENCE_HPP