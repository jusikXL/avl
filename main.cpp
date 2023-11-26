#include "ring.hpp"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int main()
{

  Ring<string, string> ring;

  vector<pair<string, string>> initialValues = {
      {"C", "C node"}, // delete
      {"A", "A node"},
      {"C", "C node"}, // delete
      {"K", "K node"},
      {"C", "C node"}, // delete
      {"G", "G node"},
      {"Y", "Y node"},
      {"C", "C node"},  // delete
      {"C", "C node"}}; // delete

  for (const auto &pair : initialValues)
  {
    ring.push(pair.first, pair.second);
  }

  cout << ring.remove_all("DS") << endl;
  cout << ring << endl;

  return 0;
}