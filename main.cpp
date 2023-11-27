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
      {1, "C node"},
      {4, "K node"},
      {3, "C node"},
      {3, "C node"},
      {7, "Y node"},
      {2, "A node"},
      {5, "C node"},
      {3, "C node"},
      {6, "G node"},
  };

  for (const auto &pair : initialValues)
  {
    ring.push(pair.first, pair.second);
  }

  ring.insert_pair_at(3, 77, "77", 66, "66");

  cout << ring << endl;

  return 0;
}