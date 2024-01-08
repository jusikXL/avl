#include "avl_tree.h"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

void insert() {
  AVLTree<int, string> tree;

  // Test 1: Inserting elements into the tree
  tree.insert(10, "Apple");
  tree.insert(5, "Banana");
  tree.insert(15, "Cherry");
  tree.insert(2, "Strawberry");
  tree.insert(1, "Blueberry");



  tree.print();
}

int main() {

  insert();

  return 0;
}