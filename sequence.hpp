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
class Sequence {
private:
  class Node {
    friend class Sequence;

  public:
    Key key;
    Info info;
    Node(const Key& k, const Info& i, Node* n = nullptr);

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
  Node* _get_node(const Key& target_key, unsigned int target_occurrence = 1) const;
  Node* _get_node_before(const Key& target_key, unsigned int target_occurrence = 1) const;

public:
  Sequence();
  ~Sequence();
  Sequence(const Sequence& src);
  Sequence& operator=(const Sequence& src);

  unsigned int length() const;
  bool is_empty() const;
  friend ostream& operator<< <>(ostream& os, const Sequence<Key, Info>& sequence);

  friend void split_pos <>(const Sequence<Key, Info>& seq, int start_pos, int len1, int len2, int count, Sequence<Key, Info>& seq1, Sequence<Key, Info>& seq2);

  /**
   * Inserts a new element with the provided key and info after the specified target element
   * of a given key and occurence.
   *
   * @param key The key of the new element to insert.
   * @param info The info of the new element to insert.
   * @param target_key The key after which the new element should be inserted.
   * @param target_occurrence Specifies after which occurrence of `target_key` to insert.
   * @return true if the element was successfully inserted, false otherwise.
   */
  bool insert_after(const Key& key, const Info& info, const Key& target_key, unsigned int target_occurrence = 1);
  void push_front(const Key& key, const Info& info);
  void push_back(const Key& key, const Info& info);

  /**
   * Removes the specified element of a given key and occurence.
   *
   * @param target_key The key of the element to remove.
   * @param target_occurrence Specifies which occurrence of the key to consider. Defaults to 1.
   * @return true if an element was successfully removed, false otherwise.
   */
  bool remove(const Key& target_key, unsigned int target_occurrence = 1);
  bool pop_front();
  bool pop_back();
  void clear();

  /**
   *
   * Retrieves the info associated to a specified element of a given key and occurence.
   *
   * @param[out] info Reference to where the info will be stored.
   * @param target_key The key for which to retrieve the associated info.
   * @param target_occurrence Specifies which occurrence of the key to consider. Defaults to 1.
   * @return true if the element was found and the associated info was retrieved, false otherwise.
   */
  bool get_info(Info& info, const Key& target_key, unsigned int target_occurrence = 1) const;
  bool front(Info& info, Key& key) const;
  bool back(Info& info, Key& key) const;

  /**
   * Searches for the specified element of a given key and occurence.
   *
   * @param target_key The key to search for.
   * @param target_occurrence Specifies which occurrence of the key to consider. Defaults to 1.
   * @return true if the specified element is found, false otherwise.
   */
  bool search(const Key& target_key, unsigned int target_occurrence) const;

  /**
   * Counts the number of occurences of the given key.
   */
  unsigned int search(const Key& target_key) const;

  class Iterator {
  private:
    Node* _current;

  public:
    Iterator(Node* ptr = nullptr);
    ~Iterator();
    Iterator(const Iterator& src);
    Iterator& operator=(const Iterator& src);

    bool operator==(const Iterator& src) const;
    bool operator!=(const Iterator& src) const;

    Iterator& operator++();
    Iterator operator++(int);
    Iterator operator+(unsigned int inverval);

    Key& key() const;
    Info& info() const;
  };

  Iterator begin() const;
  Iterator end() const;
};

#endif // SEQUENCE_HPP