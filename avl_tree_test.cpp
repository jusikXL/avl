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


int main() {
  constructors();

  insert();

  clear();

  remove();

  find_and_at();

  std::cout << "All tests passed successfully." << endl;
  return 0;
}
