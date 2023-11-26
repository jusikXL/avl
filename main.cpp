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
      {"B", "B node"},
      {"A", "A node"},
      {"C", "C node"}};

  for (const auto &pair : initialValues)
  {
    ring.push(pair.first, pair.second);
  }

  cout << ring << endl;

  return 0;
}