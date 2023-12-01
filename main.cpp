#include "ring.hpp"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int concatenate(const string &key, const int &i1, const int &i2)
{
  return i1 + i2;
};

int main()
{
  Ring<string, int> ring_first;
  Ring<string, int> ring_second;

  vector<pair<string, int>> initialValues = {
      {"a", 1},
      {"b", 2},
      {"c", 3},
      {"d", 4},
      {"e", 5},
      {"f", 6},
      {"g", 7},
      {"h", 8},
      {"i", 9},
      {"j", 10},
  };

  for (const auto &pair : initialValues)
  {
    ring_first.push_front(pair.first, pair.second);
    ring_second.push_front(pair.first, pair.second);
  }

  Ring<string, int> ring_merged = join(ring_first, ring_second);
  cout << ring_merged << endl;

  return 0;
}