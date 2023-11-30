#include "ring.hpp"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int main()
{

  Ring<int, string> originalRing;

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
    originalRing.push_front(pair.first, pair.second);
  }

  cout << "Original Ring:" << endl;
  cout << originalRing << endl;

  Ring<int, string> copiedRing(originalRing);
  cout << "Copied Ring using Copy Constructor:" << endl;
  cout << copiedRing << endl;

  Ring<int, string> assignedRing;
  assignedRing.push_front(123, "123");
  assignedRing = originalRing;
  cout << "Assigned Ring using Assignment Operator:" << endl;
  cout << assignedRing << endl;

  return 0;
}