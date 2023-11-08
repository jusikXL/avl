#ifndef SPLIT_HPP
#define SPLIT_HPP

#include "sequence.hpp"

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

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


#endif // SPLIT_HPP