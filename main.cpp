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
  assert(!sequence.isEmpty());
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
  assert(!sequence.isEmpty());
  assert(sequence.length() == 1);

  // Test pushing in front of a non-empty sequence.
  sequence.push_front(2, "second");
  assert(!sequence.isEmpty());    // Sequence should still not be empty.
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

int main()
{
  Sequence<unsigned int, string> sequence = Sequence<unsigned int, string>();

  assert(sequence.isEmpty());
  assert(sequence.length() == 0);

  push_front();

  return 0;
}