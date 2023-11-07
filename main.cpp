#include "sequence.hpp"

#include <string>
#include <cassert>
#include <iostream>
using namespace std;

// Test for print
void test_print() {
  Sequence<unsigned int, string> sequence;

  sequence.push_back(1, "One");
  sequence.push_back(2, "Two");
  sequence.push_back(3, "Three");

  cout << "The sequence contains: " << sequence << endl;
}

// Test for pop_front method
void test_pop_front() {
  Sequence<unsigned int, string> seq;

  // Test removal on empty sequence
  assert(!seq.pop_front());

  // Add elements and test removal
  seq.push_back(1, "One");
  assert(seq.pop_front());
  assert(seq.is_empty());

  cout << "Pop front method test passed." << endl;
}

// Test for pop_back method
void test_pop_back() {
  Sequence<unsigned int, string> seq;

  // Test removal on empty sequence
  assert(!seq.pop_back());

  // Add elements and test removal
  seq.push_back(1, "One");
  seq.push_back(2, "Two");

  // Variables to store the output of back()
  string info;
  unsigned int key;

  assert(seq.pop_back());

  // Using back method with out parameters
  bool hasBack = seq.back(info, key);
  assert(hasBack && info == "One" && key == 1);

  cout << "Pop back method test passed." << endl;
}

// Test for clear method
void test_clear() {
  Sequence<unsigned int, string> seq;

  // Add elements and clear
  seq.push_back(1, "One");
  seq.push_back(2, "Two");
  seq.clear();
  assert(seq.is_empty());

  cout << "Clear method test passed." << endl;
}

// Test for remove method
void test_remove() {
  Sequence<unsigned int, string> seq;

  // Add elements
  seq.push_back(1, "One");
  seq.push_back(2, "Two");
  seq.push_back(3, "Three");
  seq.push_back(2, "Another Two");

  // Remove element in the middle
  assert(seq.remove(2, 1));
  assert(seq.search(2) == 1);

  // Remove non-existent element
  assert(!seq.remove(4, 1));

  // Remove head and tail
  assert(seq.remove(1, 1));
  assert(seq.remove(3, 1));

  cout << "Remove method test passed." << endl;
}

// Test for search method that returns a bool
void test_search_bool() {
  Sequence<unsigned int, string> seq;

  // Test when the sequence is empty
  assert(!seq.search(1, 1));

  // Add elements to the sequence
  seq.push_back(1, "One");
  seq.push_back(2, "Two");
  seq.push_back(1, "Another One");

  // Test finding first occurrence
  assert(seq.search(1, 1));

  // Test finding second occurrence
  assert(seq.search(1, 2));

  // Test finding non-existent third occurrence
  assert(!seq.search(1, 3));

  // Test with a non-existent key
  assert(!seq.search(3, 1));

  cout << "Search (bool) method test passed." << endl;
}

// Test for search method that returns count of occurrences
void test_search_count() {
  Sequence<unsigned int, string> seq;

  // Test when the sequence is empty
  assert(seq.search(1) == 0);
  // Add elements to the sequence
  seq.push_back(1, "One");
  seq.push_back(2, "Two");
  seq.push_back(1, "Another One");

  // Test counting occurrences for an existing key
  assert(seq.search(1) == 2);

  // Test counting occurrences for a non-existent key
  assert(seq.search(3) == 0);

  cout << "Search (count) method test passed." << endl;
}

// Test for front method
void test_front() {
  Sequence<unsigned int, string> seq;
  string info;
  unsigned int key;

  // Test when the sequence is empty
  assert(!seq.front(info, key));

  // Test when the sequence has one element
  seq.push_front(1, "One");
  assert(seq.front(info, key) && key == 1 && info == "One");

  // Test when the sequence has multiple elements
  seq.push_back(2, "Two");
  assert(seq.front(info, key) && key == 1 && info == "One");

  cout << "Front method test passed." << endl;
}

// Test for back method
void test_back() {
  Sequence<unsigned int, string> seq;
  string info;
  unsigned int key;

  // Test when the sequence is empty
  assert(!seq.back(info, key));

  // Test when the sequence has one element
  seq.push_back(1, "One");
  assert(seq.back(info, key) && key == 1 && info == "One");

  // Test when the sequence has multiple elements
  seq.push_front(2, "Two");
  assert(seq.back(info, key) && key == 1 && info == "One");

  cout << "Back method test passed." << endl;
}

// Test for get_info method considering occurrences
void test_get_info() {
  Sequence<unsigned int, string> seq;
  string info;

  // Test when the sequence is empty
  assert(!seq.get_info(info, 1, 1));

  // Add elements to the sequence
  seq.push_back(1, "One");
  seq.push_back(2, "Two");
  seq.push_back(1, "Another One");

  // Test getting first occurrence
  assert(seq.get_info(info, 1, 1) && info == "One");

  // Test getting second occurrence
  assert(seq.get_info(info, 1, 2) && info == "Another One");

  // Test asking for a third occurrence which does not exist
  assert(!seq.get_info(info, 1, 3));

  // Test with non-existent key
  assert(!seq.get_info(info, 3, 1));

  cout << "Get info method test passed." << endl;
}

// Test for push_front
void test_push_front() {
  Sequence<unsigned int, string> seq;
  string info;
  unsigned int key;

  assert(seq.is_empty());

  // Insert at the front when the sequence is empty
  seq.push_front(2, "Two");
  assert(seq.length() == 1);
  assert(seq.front(info, key) && info == "Two" && key == 2);

  // Insert at the front when the sequence is not empty
  seq.push_front(1, "One");
  assert(seq.length() == 2);
  assert(seq.front(info, key) && info == "One" && key == 1);

  cout << "Push front test passed." << endl;
}

// Test for insert_after
void test_insert_after() {
  Sequence<unsigned int, string> seq;
  string info;
  unsigned int key;

  seq.push_back(1, "One");
  seq.push_back(3, "Three");

  // Insert after the first element
  bool inserted = seq.insert_after(2, "Two", 1);
  assert(inserted);
  assert(seq.length() == 3);

  // Insert after the last element (which should be equivalent to push_back)
  inserted = seq.insert_after(4, "Four", 3);
  assert(inserted);
  assert(seq.length() == 4);
  assert(seq.back(info, key) && info == "Four" && key == 4);

  // Attempting to insert after a non-existing key
  inserted = seq.insert_after(5, "Five", 10);
  assert(!inserted);
  assert(seq.length() == 4);

  // Insert after a middle element
  seq.push_back(5, "Five");
  inserted = seq.insert_after(4, "Four-and-a-half", 3);
  assert(inserted);
  assert(seq.length() == 6);

  cout << "Insert after test passed." << endl;
}

// Test for push_back
void test_push_back() {
  Sequence<unsigned int, string> seq;
  string info;
  unsigned int key;

  assert(seq.is_empty());

  // Insert at the back when the sequence is empty
  seq.push_back(1, "One");
  assert(seq.length() == 1);
  assert(seq.back(info, key) && info == "One" && key == 1);

  // Insert at the back when the sequence is not empty
  seq.push_back(2, "Two");
  assert(seq.length() == 2);
  assert(seq.back(info, key) && info == "Two" && key == 2);

  cout << "Push back test passed." << endl;
}

// Test for the default constructor
void test_constructor() {
  Sequence<unsigned int, string> seq;
  assert(seq.length() == 0);
  assert(seq.is_empty() == true);
  cout << "Constructor test passed." << endl;
}

// Test for the copy constructor
void test_copy_constructor() {
  Sequence<unsigned int, string> original;
  original.push_back(1, "One");
  Sequence<unsigned int, string> copy(original);
  assert(copy.length() == 1);
  assert(copy.is_empty() == false);
  string info;
  unsigned int key;
  assert(copy.front(info, key) == true);
  assert(info == "One" && key == 1);
  cout << "Copy constructor test passed." << endl;
}

// Test for the assignment operator
// Test for the assignment operator
void test_assignment_operator() {
  Sequence<unsigned int, string> seq1;
  seq1.push_back(1, "One");
  Sequence<unsigned int, string> seq2;
  seq2 = seq1;
  seq1.push_back(2, "Two");
  assert(seq2.length() == 1);
  string info;
  unsigned int key;
  assert(seq2.front(info, key));
  assert(info == "One" && key == 1);
  assert(seq1.length() == 2);
  string infoSecond;
  unsigned int keySecond;
  seq1.back(infoSecond, keySecond);
  assert(infoSecond == "Two" && keySecond == 2);
  cout << "Assignment operator test passed." << endl;
}


// Test for length and is_empty methods
void test_length_is_empty() {
  Sequence<unsigned int, string> seq;
  assert(seq.length() == 0);
  assert(seq.is_empty() == true);
  seq.push_back(1, "One");
  assert(seq.length() == 1);
  assert(seq.is_empty() == false);
  cout << "Length and Is empty test passed." << endl;
}

// Test for self-assignment
void test_self_assignment() {
  Sequence<unsigned int, string> seq;
  seq.push_back(1, "One");
  seq = seq;  // Self assignment
  assert(seq.length() == 1);
  string info;
  unsigned int key;
  assert(seq.front(info, key));
  assert(info == "One" && key == 1);
  cout << "Self assignment test passed." << endl;
}

void core_tests() {
  test_constructor();
  test_copy_constructor();
  test_assignment_operator();
  test_length_is_empty();
  test_self_assignment();
}

void insertion_tests() {
  test_push_front();
  test_push_back();
  test_insert_after();
}

void retrieval_tests() {
  test_front();
  test_back();
  test_get_info();
  test_search_bool();
  test_search_count();
}

void removal_tests() {
  test_pop_front();
  test_pop_back();
  test_remove();
  test_clear();
}

void test_iterator() {
  // Create a sequence to iterate over
  Sequence<unsigned int, string> seq;
  seq.push_back(1, "One");
  seq.push_back(2, "Two");
  seq.push_back(3, "Three");

  // Begin and end
  Sequence<unsigned int, string>::Iterator begin = seq.begin();
  Sequence<unsigned int, string>::Iterator end = seq.end();

  // Test operator!= and operator==
  assert(begin != end);
  assert(!(begin == end));

  // Test dereference and member access
  assert(begin.key() == 1);
  assert(begin.info() == "One");

  // Test increment operators
  ++begin; // now should point to "Two"
  assert(begin.key() == 2);
  assert(begin.info() == "Two");

  begin++; // now should point to "Three"
  assert(begin.key() == 3);
  assert(begin.info() == "Three");

  // Test the postfix increment operator
  Sequence<unsigned int, string>::Iterator it = seq.begin();
  Sequence<unsigned int, string>::Iterator postIncrementedIt = it++;
  assert(postIncrementedIt.key() == 1); // Should still be "One"
  assert(it.key() == 2); // Should now be "Two"

  // Test operator+
  it = seq.begin();
  Sequence<unsigned int, string>::Iterator plusOne = it + 1;
  assert(plusOne.key() == 2);
  assert(plusOne.info() == "Two");

  // Test copy constructor and assignment operator
  Sequence<unsigned int, string>::Iterator copiedIt = it;
  assert(copiedIt == it);
  Sequence<unsigned int, string>::Iterator assignedIt;
  assignedIt = it;
  assert(assignedIt == it);

  // Test end condition
  it = end; // should be equivalent to nullptr
  assert(it != seq.begin()); // End should not be equal to begin

  cout << "Iterator tests passed." << endl;
}

int main() {
  core_tests();
  insertion_tests();
  retrieval_tests();
  removal_tests();

  test_iterator();

  test_print();

  return 0;
}