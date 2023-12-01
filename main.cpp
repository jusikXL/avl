#include "ring.hpp"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int concatenate(const string& key, const int& i1, const int& i2) {
  return i1 + i2;
};

int main() {
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

  return 0;
}