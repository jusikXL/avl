#include "sequence.hpp"
#include <string>

#include <cassert>
#include <iostream>

using namespace std;

void push_front_1()
{
  // should push front on empty sequence

  Sequence<unsigned int, string> sequence;

  // Test pushing to an empty sequence.
  sequence.push_front(1, "first");
  assert(!sequence.is_empty());
  assert(sequence.length() == 1);

  unsigned int key;
  string info;
  assert(sequence.front(info, key)); // Should be able to get front element.
  assert(key == 1);                  // The key should match what was pushed.
  assert(info == "first");           // The info should match what was pushed.
}

void push_front_2()
{
  // should push front multiple elements

  Sequence<unsigned int, string> sequence;
  sequence.push_front(1, "first");
  assert(!sequence.is_empty());
  assert(sequence.length() == 1);

  // Test pushing in front of a non-empty sequence.
  sequence.push_front(2, "second");
  assert(!sequence.is_empty());   // Sequence should still not be empty.
  assert(sequence.length() == 2); // Sequence length should be 2 now.

  unsigned int key;
  string info;
  assert(sequence.front(info, key)); // Should be able to get the new front element.
  assert(key == 2);                  // The key should be the one from the new push.
  assert(info == "second");          // The info should be from the new push.

  // Verify that the order is correct (LIFO - Last In, First Out).
  Sequence<unsigned int, string>::Iterator it = sequence.begin();
  assert(it.key() == 2 && it.info() == "second"); // First element should be the last pushed.
  ++it;
  assert(it.key() == 1 && it.info() == "first"); // Second element should be the first pushed.
}

void push_front()
{
  push_front_1();
  push_front_2();
}

void push_back_1()
{
  // should push back on empty sequence
  Sequence<unsigned int, string> sequence;
  sequence.push_back(1, "last");
  assert(!sequence.is_empty());
  assert(sequence.length() == 1);

  unsigned int key;
  string info;
  assert(sequence.front(info, key));
  assert(key == 1);
  assert(info == "last");
}

void push_back_2()
{
  // should push back multiple elements
  Sequence<unsigned int, string> sequence;
  sequence.push_back(1, "first");
  sequence.push_back(2, "last");
  assert(!sequence.is_empty());
  assert(sequence.length() == 2);

  Sequence<unsigned int, string>::Iterator it = sequence.begin();
  assert(it.key() == 1 && it.info() == "first"); // Verify first element
  ++it;
  assert(it.key() == 2 && it.info() == "last"); // Verify last element
}

void push_back()
{
  push_back_1();
  push_back_2();
}

void insert_after_1()
{
  // should insert after the first occurrence of a key
  Sequence<unsigned int, string> sequence;
  sequence.push_back(1, "first");
  sequence.push_back(2, "second");
  sequence.insert_after(3, "third", 1, 1);
  assert(sequence.length() == 3);

  Sequence<unsigned int, string>::Iterator it = sequence.begin();
  ++it;                                          // Move to the second node
  assert(it.key() == 3 && it.info() == "third"); // Verify inserted element
}

void insert_after_2()
{
  // should insert after the second occurrence of a key
  Sequence<unsigned int, string> sequence;
  sequence.push_back(1, "first");
  sequence.push_back(2, "second");
  sequence.push_back(1, "another first");
  sequence.insert_after(3, "third", 1, 2);
  assert(sequence.length() == 4);

  Sequence<unsigned int, string>::Iterator it = sequence.begin();
  ++it; // Skip first node
  ++it; // Move to the second node with key == 1
  ++it; // Should now be at the inserted node
  assert(it.key() == 3 && it.info() == "third");
}

void insert_after_3()
{
  // should not insert if the key is not found
  Sequence<unsigned int, string> sequence;
  sequence.push_back(1, "first");
  bool result = sequence.insert_after(3, "third", 2, 1); // key 2 doesn't exist
  assert(!result);
  assert(sequence.length() == 1); // Length should be unchanged
}

void insert_after()
{
  insert_after_1();
  insert_after_2();
  insert_after_3();
}

int main()
{
  Sequence<unsigned int, string> sequence = Sequence<unsigned int, string>();

  assert(sequence.is_empty());
  assert(sequence.length() == 0);

  push_front();
  push_back();
  insert_after();

  return 0;
}