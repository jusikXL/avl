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
  Ring<string, int> ring;

  vector<pair<string, int>> initialValues = {
      {"a", 1},
      {"b", 2},
      {"c", 3},
      {"d", 4},
      {"e", 5},
      {"f", 6},
      {"q", 17},
      {"g", 7},
      {"h", 8},
      {"q", 17},
      {"i", 9},
      {"a", 1},
      {"a", 1},
      {"j", 10},
      {"k", 11},
      {"l", 12},
      {"m", 13},
      {"q", 17},
      {"n", 14},
      {"o", 15},
      {"p", 16},
      {"a", 1},
      {"q", 17},
      {"r", 18},
      {"s", 19},
      {"t", 20},
      {"a", 1},
  };

  for (const auto &pair : initialValues)
  {
    ring.push_front(pair.first, pair.second);
  }
  cout << ring << endl;

  Ring<string, int> uniqueRing = unique(ring, concatenate);
  cout << uniqueRing << endl;

  return 0;
}