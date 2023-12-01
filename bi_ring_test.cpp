#include "bi_ring.h"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

int concatenate(const string& key, const int& i1, const int& i2) {
  return i1 + i2;
};

void shuffle() {
  Ring<string, int> ring_first;
  Ring<string, int> ring_second;

  vector<pair<string, int>> nodes_first = {
      {"quattro", 4},
      {"tre", 3},
      {"due", 2},
      {"uno", 1},
  };

  vector<pair<string, int>> nodes_second = {
      {"bes", 5},
      {"dort", 4},
      {"uc", 3},
      {"iki", 2},
      {"bir", 1},
  };

  for (const auto& pair : nodes_first) {
    ring_first.push_front(pair.first, pair.second);
  }

  for (const auto& pair : nodes_second) {
    ring_second.push_front(pair.first, pair.second);
  }

  cout << ring_first << endl;
  cout << ring_second << endl;

  Ring<string, int> ring_shuffled = shuffle(ring_first, 1, ring_second, 2, 3);
  cout << ring_shuffled << endl;
}

void push() {
  Ring<string, string> ring;

  vector<pair<string, string>> nodes = {
      {"C", "C node"},
      {"B", "B node"},
      {"A", "A node"},
  };

  for (const auto& pair : nodes) {
    ring.push_front(pair.first, pair.second);
  }

  // should change _past pointers
  // so that we can traverse the ring backwards using iterators
  auto it = --(ring.cend());
  for (const auto& pair : nodes) {
    // should sequentially add keys and infos
    assert(it.key() == pair.first && it.info() == pair.second);

    it--;
  }

  // should change _next pointers
  // so that we can traverse the ring forwards using iterators
  auto it_revered = ring.cbegin();
  for (vector<pair<string, string>>::reverse_iterator i = nodes.rbegin(); i != nodes.rend(); ++i) {
    assert(it_revered.key() == i->first && it_revered.info() == i->second);

    it_revered++;
  }

  // should increase the size
  assert(ring.size() == nodes.size());

  // should return a correct iterator to the just pushed node
  auto it_pushed = ring.push_front("D", "D node");
  assert(it_pushed == ring.begin());
}

void insert() {
  Ring<string, string> ring;

  // should insert in an empty ring if begin() is provided as a position
  ring.insert(ring.begin(), "key", "info");
  assert(ring.begin().key() == "key" && ring.begin().info() == "info");

  // should increase size
  assert(ring.size() == 1);

  // should insert before successfully updating _next and _past pointers
  ring.insert(ring.begin(), "before key", "before info");
  auto it_first = ++ring.begin();

  assert(ring.begin().key() == "before key" && ring.begin().info() == "before info");
  assert(it_first.key() == "key" && it_first.info() == "info");

  auto it_second = --it_first;
  assert(it_second.key() == "before key" && it_second.info() == "before info");
}

void pop() {
  Ring<int, string> ring;

  vector<pair<int, string>> nodes = {
      {4, "D node"},
      {3, "C node"},
      {2, "B node"},
      {1, "A node"},
  };

  for (const auto& pair : nodes) {
    ring.push_front(pair.first, pair.second);
  }

  for (unsigned int i = 0; i < nodes.size(); i++) {
    auto it_next = ring.pop_front();

    // should decrease the size
    assert(ring.size() == nodes.size() - i - 1);

    if (i != nodes.size() - 1) {
      // should return iterator to a next node after removed
      assert(it_next.key() == nodes[nodes.size() - i - 2].first && it_next.info() == nodes[nodes.size() - i - 2].second);

      // should change the _next pointer of the _sentinel to its _next
      assert(ring.begin().key() == nodes[nodes.size() - i - 2].first && ring.begin().info() == nodes[nodes.size() - i - 2].second);
    } else {
      assert(it_next.key() == int{});
      assert(ring.begin().key() == int{});
    }

    // should change the _past pointer of the next node to its _sentinel
    assert((--it_next).key() == int{});
  }
}