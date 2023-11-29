#include "ring.hpp"
#include "tree.hpp"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int main()
{

  BST<int, string> tree;

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
    tree.insert(pair.first, pair.second);
  }

  cout << "Tree height: " << tree.find_height() << endl;
  cout << "Number of leaves: " << tree.find_number_of_leaves() << endl;
  cout << "Odd nodes: " << tree.count_odd() << endl;
  cout << "Even nodes: " << tree.count_even() << endl;

  return 0;
}