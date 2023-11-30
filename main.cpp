#include "ring.hpp"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

bool alwaysTrue(const string &) { return true; }
bool alwaysFalse(const string &) { return false; }
bool lengthGreaterThanThree(const string &str) { return str.size() > 3; }

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
      {"g", 7},
      {"h", 8},
      {"i", 9},
      {"j", 10},
      {"k", 11},
      {"l", 12},
      {"m", 13},
      {"n", 14},
      {"o", 15},
      {"p", 16},
      {"q", 17},
      {"r", 18},
      {"s", 19},
      {"t", 20},
  };

  for (const auto &pair : initialValues)
  {
    ring.push_front(pair.first, pair.second);
  }

  cout << ring.find("q").info() << endl;

  return 0;
}