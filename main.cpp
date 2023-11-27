#include "ring.hpp"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int main()
{

  Ring<int, string> ring;

  vector<pair<int, string>> initialValues = {
      {3, "3 node"}, // delete
      {7, "7 node"},
      {6, "6 node"},
      {3, "3 node"}, // delete
      {5, "5 node"},
      {3, "3 node"}, // delete
      {4, "4 node"},
      {2, "2 node"},
      {3, "3 node"}, // delete
      {1, "1 node"},
      {3, "3 node"}, // delete
  };

  for (const auto &pair : initialValues)
  {
    ring.push(pair.first, pair.second);
  }

  cout << ring << endl;

  cout << ring.remove_exactly_last(3, 3) << endl;

  cout << ring << endl;

  return 0;
}