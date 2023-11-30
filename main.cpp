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
      {5, "5 node"},
      {4, "4 node"},
      {3, "3 node"},
      {2, "2 node"},
      {7, "7 node"},
      {6, "6 node"},
      {8, "8 node"},
      {10, "10 node"},
      {9, "9 node"},
  };

  for (const auto &pair : initialValues)
  {
    ring.push(pair.first, pair.second);
  }

  cout << ring << endl;

  return 0;
}