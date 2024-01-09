#include "avl_tree.h"
#include <cassert>
#include <vector>
#include <utility>

using namespace std;

void _insert_fixture(AVLTree<int, string>& tree, const vector<pair<int, string>>& nodes) {
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
   {5, "E node"},
   {3, "C node"},
   {7, "G node"},
   {2, "B node"},
   {4, "D node"},
   {6, "F node"},
   {8, "H node"},
  };
  _insert_fixture(tree, nodes);

  // remove a leaf node
  string removed_info = tree.remove(8);
  assert(removed_info == "H node");
  assert(tree.size() == 7);
  assert(tree.balanced());

  // Remove a node with one child
  removed_info = tree.remove(2);
  assert(removed_info == "B node");
  assert(tree.size() == 6);
  assert(tree.balanced());

  // Remove a node with two children
  removed_info = tree.remove(5);
  assert(removed_info == "E node");
  assert(tree.size() == 5);
  assert(tree.balanced());

  // Remove non-existing node
  removed_info = tree.remove(10);
  assert(removed_info == ""); // The key doesn't exist
  assert(tree.size() == 5);
  assert(tree.balanced());

  // Test removing all nodes
  tree.remove(3);
  tree.remove(4);
  tree.remove(6);
  tree.remove(7);

  assert(tree.size() == 1);
  assert(tree.balanced());

  // Remove the last node
  removed_info = tree.remove(8);
  assert(removed_info == ""); // No such key
  assert(tree.size() == 0);
  assert(tree.balanced());

  // Additional assertions if needed
}



int main() {
  constructors();

  insert();

  clear();

  cout << "All tests passed successfully." << endl;
  return 0;
}
