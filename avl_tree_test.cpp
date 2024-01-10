#include "avl_tree.h"
#include "avl_tree_test.h"
#include <cassert>
#include <vector>
#include <utility>

using namespace std;

template <typename Key, typename Info>
void _insert_fixture(AVLTree<Key, Info>& tree, const vector<pair<Key, Info>>& nodes) {
  for (const auto& pair : nodes) {
    tree.insert(pair.first, pair.second);
  }
  for (const auto& pair : nodes) {
    assert(tree[pair.first] == pair.second);
  }
  assert(tree.size() == nodes.size());
  assert(tree.balanced());
}

void insert() {
  AVLTree<int, string> tree1;
  vector<pair<int, string>> nodes1 = {
    // LL case
    {4, "D node"},
    {3, "C node"},
    {2, "B node"},
  };
  _insert_fixture(tree1, nodes1);

  AVLTree<int, string> tree2;
  vector<pair<int, string>> nodes2 = {
    // RR case
    {2, "B node"},
    {3, "C node"},
    {4, "D node"},
  };
  _insert_fixture(tree2, nodes2);

  AVLTree<int, string> tree3;
  vector<pair<int, string>> nodes3 = {
    // LR case
    {4, "D node"},
    {2, "B node"},
    {3, "C node"},
  };
  _insert_fixture(tree3, nodes3);

  AVLTree<int, string> tree4;
  vector<pair<int, string>> nodes4 = {
    // RL case
    {2, "B node"},
    {4, "D node"},
    {3, "C node"},
  };
  _insert_fixture(tree4, nodes4);
}

void constructors() {
  // constructor
  AVLTree<int, string> tree1;
  assert(tree1.size() == 0);
  assert(tree1.balanced());

  // copy constructor
  AVLTree<int, string> tree2;
  vector<pair<int, string>> nodes2 = {
    {5, "E node"},
    {3, "C node"},
    {7, "G node"},
  };
  _insert_fixture(tree2, nodes2);

  AVLTree<int, string> tree3(tree2);
  assert(tree3.size() == tree2.size());
  assert(tree3.balanced());
  for (const auto& pair : nodes2) {
    assert(tree3[pair.first] == pair.second);
  }

  // assignment operator test
  AVLTree<int, string> tree4;
  vector<pair<int, string>> nodes4 = {
    {8, "H node"},
    {4, "D node"},
  };
  _insert_fixture(tree4, nodes4);

  AVLTree<int, string> tree5;
  tree5 = tree4;
  assert(tree5.size() == tree4.size());
  assert(tree5.balanced());
  for (const auto& pair : nodes4) {
    assert(tree5[pair.first] == pair.second);
  }
}

void clear() {
  AVLTree<int, string> tree;
  vector<pair<int, string>> nodes = {
    {5, "E node"},
    {3, "C node"},
    {7, "G node"},
  };
  _insert_fixture(tree, nodes);

  tree.clear();
  assert(tree.size() == 0);
  assert(tree.balanced());
  for (const auto& pair : nodes) {
    assert(tree.find(pair.first) == false);
  }
}

void remove() {
  AVLTree<int, string> tree;
  vector<pair<int, string>> nodes = {
    {10, "A"},
    {5, "B"},
    {15, "C"},
    {2, "D"},
    {8, "E"},
    {12, "F"},
    {18, "G"},
  };
  _insert_fixture(tree, nodes);

  // leaf
  assert(tree.remove(2));
  assert(!tree.find(2));
  assert(tree.balanced());
  assert(tree.size() == 6);

  // one child
  assert(tree.remove(5));
  assert(!tree.find(5));
  assert(tree.balanced());
  assert(tree.size() == 5);

  // two children
  assert(tree.remove(15));
  assert(!tree.find(15));
  assert(tree.balanced());
  assert(tree.size() == 4);

  // non - existing node
  assert(!tree.remove(100));
  assert(tree.balanced());
  assert(tree.size() == 4);

  // root node
  assert(tree.remove(10));
  assert(!tree.find(10));
  assert(tree.balanced());
  assert(tree.size() == 3);
}


void find_and_at() {
  AVLTree<int, string> tree;
  vector<pair<int, string>> nodes = {
    {10, "A"},
    {5, "B"},
    {15, "C"},
  };
  _insert_fixture(tree, nodes);

  for (const auto& pair : nodes) {
    assert(tree.find(pair.first));
    assert(tree[pair.first] == pair.second);
  }
  assert(tree.find(11) == false);
}

void for_each() {
  AVLTree<int, int> tree;
  vector<pair<int, int>> nodes = {
    {5, 10},
    {3, 20},
    {7, 30},
    {2, 40},
    {4, 50},
    {6, 60},
    {8, 70},
  };
  _insert_fixture(tree, nodes);

  tree.for_each([ ](const int& key, int& info) { info++; });

  assert(tree[5] == 11);
  assert(tree[3] == 21);
  assert(tree[7] == 31);
  assert(tree[2] == 41);
  assert(tree[4] == 51);
  assert(tree[6] == 61);
  assert(tree[8] == 71);
}

void get_largest() {
  AVLTree<int, string> tree;
  vector<pair<int, string>> nodes = {
    {10, "A"},
    {5, "B"},
    {15, "C"},
    {2, "D"},
    {8, "E"},
    {12, "F"},
    {18, "G"},
  };
  _insert_fixture(tree, nodes);

  int n = 3;
  vector<pair<int, string>> largestElements = tree.get_largest(n);
  vector<pair<int, string>> expectedElements = {
    {18, "G"},
    {15, "C"},
    {12, "F"}
  };
  assert(largestElements == expectedElements);

  largestElements = tree.get_largest(0);
  expectedElements = {};
  assert(largestElements == expectedElements);

  largestElements = tree.get_largest(10); // more than in array
  expectedElements = {
    {18, "G"},
    {15, "C"},
    {12, "F"},
    {10, "A"},
    {8, "E"},
    {5, "B"},
    {2, "D"}
  };
  assert(largestElements == expectedElements);
}

void get_smallest() {
  AVLTree<int, string> tree;
  vector<pair<int, string>> nodes = {
    {10, "A"},
    {5, "B"},
    {15, "C"},
    {2, "D"},
    {8, "E"},
    {12, "F"},
    {18, "G"},
  };
  _insert_fixture(tree, nodes);

  int n = 3;
  vector<pair<int, string>> smallestElements = tree.get_smallest(n);
  vector<pair<int, string>> expectedElements = {
      {2, "D"},
      {5, "B"},
      {8, "E"}
  };

  assert(smallestElements == expectedElements);

  smallestElements = tree.get_smallest(0);
  expectedElements = {};
  assert(smallestElements == expectedElements);

  smallestElements = tree.get_smallest(10); // more than in the array
  expectedElements = {
      {2, "D"},
      {5, "B"},
      {8, "E"},
      {10, "A"},
      {12, "F"},
      {15, "C"},
      {18, "G"}
  };

  assert(smallestElements == expectedElements);
}

void max_info_selector() {
  AVLTree<int, string> tree;
  vector<pair<int, string>> nodes = {
    {10, "A"},
    {5, "B"},
    {15, "C"},
    {2, "D"},
    {8, "E"},
    {12, "F"},
    {18, "G"},
  };
  _insert_fixture(tree, nodes);

  int n = 3;
  vector<pair<int, string>> selectedElements = maxinfo_selector(tree, n);

  vector<pair<int, string>> expectedElements = {
      {18, "G"},
      {12, "F"},
      {8, "E"}
  };
  assert(selectedElements == expectedElements);

  // Test when n is greater than the number of elements in the tree
  selectedElements = maxinfo_selector(tree, 10);
  expectedElements = {
      {18, "G"},
      {12, "F"},
      {8, "E"},
      {2, "D"},
      {15, "C"},
      {5, "B"},
      {10, "A"}};
  assert(selectedElements == expectedElements);

  // Test when n is 0
  selectedElements = maxinfo_selector(tree, 0);
  expectedElements = {};
  assert(selectedElements == expectedElements);

  // Test when info can be the same, keys should sum
  tree[2] = "F";
  selectedElements = maxinfo_selector(tree, n);
  expectedElements = {
      {18, "G"},
      {14, "F"},
      {8, "E"}};
}


int main() {
  constructors();

  insert();

  clear();

  remove();

  find_and_at();

  for_each();

  get_largest();
  get_smallest();

  max_info_selector();

  cout << "All tests passed successfully." << endl;
  return 0;
}
