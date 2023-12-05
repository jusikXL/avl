#include "bi_ring.h"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

pair<Ring<int, string>, vector<pair<int, string>>>
_ring_fixture() {
  // Call the fixture to initialize ring and nodes
  Ring<int, string> ring;

  vector<pair<int, string>> nodes = {
      {4, "D node"},
      {3, "C node"},
      {2, "B node"},
      {1, "A node"},
  };

  for (const auto& pair : nodes) {
    ring.push_front(pair.first, pair.second);
  }

  return make_pair(ring, nodes);
}

void constructor() {
  Ring<int, int> ring;

  assert(ring.cbegin() == ring.cend());
  assert((++ring.cbegin()) == ring.cend());
  assert((--ring.cbegin()) == ring.cend());

  assert(ring.cend()->first == int{});
  assert(ring.cend()->second == int{});

  assert(ring.size() == 0);
}

void push() {
  auto [ring, nodes] = _ring_fixture();

  auto it = --(ring.cend());
  for (const auto& pair : nodes) {
    // should sequentially add keys and infos
    assert(*it == pair);

    it--;
  }

  // should change _next pointers
  // so that we can traverse the ring forward using iterators
  it = --ring.cend();
  for (auto node : nodes) {
    assert(*it == node);

    it--;
  }

  // should increase the size
  assert(ring.size() == nodes.size());

  // should return a correct iterator to the just pushed node
  auto it_pushed = ring.push_front(5, "E node");
  assert(it_pushed == ring.begin());
}

void insert() {
  Ring<string, string> ring;

  // should insert in an empty ring if begin() is provided as a position
  ring.insert(ring.begin(), "key", "info");
  assert(ring.begin()->first == "key" && ring.begin()->second == "info");

  // should increase size
  assert(ring.size() == 1);

  // should insert before successfully updating _next and _past pointers
  ring.insert(ring.begin(), "before key", "before info");
  auto it_first = ++ring.begin();

  assert(ring.begin()->first == "before key" && ring.begin()->second == "before info");
  assert(it_first->first == "key" && it_first->second == "info");

  auto it_second = --it_first;
  assert(it_second->first == "before key" && it_second->second == "before info");
}

void pop() {
  auto [ring, nodes] = _ring_fixture();

  for (unsigned int i = 0; i < nodes.size(); i++) {
    auto it_next = ring.pop_front();

    // should decrease the size
    assert(ring.size() == nodes.size() - i - 1);

    if (i != nodes.size() - 1) {
      // should return iterator to a next node after removed
      assert(*it_next == nodes[nodes.size() - i - 2]);

      // should change the _next pointer of the _sentinel to its _next
      assert(*ring.begin() == nodes[nodes.size() - i - 2]);
    } else {
      assert(*it_next == make_pair(int{}, string{}));
      assert(*ring.begin() == make_pair(int{}, string{}));
    }

    // should change the _past pointer of the next node to _sentinel
    assert(*it_next.past() == make_pair(int{}, string{}));
  }

  // should return _sentinel while trying to pop_front on the empty ring
  Ring<int, string> ring_empty;

  assert(ring_empty.pop_front() == ring_empty.begin());
}

void erase() {
  auto [ring, nodes] = _ring_fixture();

  // should remove the first node - covered in pop()
  // should decrease the size - covered in pop()
  // should return _sentinel if trying to erease _sentinel - covered in pop()

  auto it = ring.begin();
  it++; // second node

  auto it_next = ring.erase(it);

  // should return pointer to the next node after removed
  assert(*it_next == nodes[nodes.size() - 3]);

  // should change _next pointer of the previous node to its next
  assert(++ring.begin() == it_next);

  // should change _past pointer of the next node to its past
  assert(--it_next == ring.begin());
}

void clear() {
  auto [ring, nodes] = _ring_fixture();

  ring.clear();

  assert(ring.cbegin() == ring.cend());
  assert((++ring.cbegin()) == ring.cend());
  assert((--ring.cbegin()) == ring.cend());

  assert(ring.size() == 0);
}

void find() {
  auto [ring, nodes] = _ring_fixture();

  // should return iterator to the found node
  for (unsigned int i = 0; i < nodes.size(); i++) {
    auto it = ring.find(nodes[i].first);
    assert(*it == nodes[i]);
  }

  // should return iterator to the upper bound if not found (_sentinel in our case)
  auto it_not_found = ring.find(456789);
  assert(it_not_found == ring.end());

  // should find the node even if the to comes after the from
  auto from = --ring.end();
  auto to = ring.begin();

  auto found = ring.find(ring.cend()->first, from, to);
  assert(found->first == ring.cend()->first);
}

template <typename Key, typename Info>
Info _add_info(const Key&, const Info& i1, const Info& i2) {
  return i1 + "-" + i2;
}

void unique() {
  Ring<int, string> source;
  string infos_fr[] = {"un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf", "dix"};
  string infos_en[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
  string infos_ru[] = {"один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять", "десять"};

  for (int i = 0; i < 10; i++) {
    source.push_back(i + 1, infos_fr[i]);
  }
  for (int i = 0; i < 10; i++) {
    source.push_back(i + 1, infos_en[i]);
  }
  for (int i = 0; i < 10; i++) {
    source.push_back(i + 1, infos_ru[i]);
  }

  source.push_back(777, "Слава Украине");

  auto res = unique(source, _add_info);

  string info_expected[] = {"un-one-один", "deux-two-два", "trois-three-три", "quatre-four-четыре", "cinq-five-пять", "six-six-шесть", "sept-seven-семь", "huit-eight-восемь", "neuf-nine-девять", "dix-ten-десять"};

  int i = 1;
  for (auto it = res.cbegin(); it != res.cend() - 1; it.next()) {
    assert(it->first == i);
    assert(it->second == info_expected[i - 1]);
    i++;
  }
  assert((res.cend() - 1)->first == 777);
  assert((res.cend() - 1)->second == "Слава Украине");
}

void join() {
  Ring<int, string> ring1;
  ring1.push_back(1, "One");
  ring1.push_back(2, "Two");
  ring1.push_back(3, "Three");
  ring1.push_back(4, "Four");

  Ring<int, string> ring2;
  ring1.push_back(1, "Раз");
  ring1.push_back(2, "Два");
  ring1.push_back(3, "Три");

  auto res = join(ring1, ring2);

  string info_expected[] = {"OneРаз", "TwoДва", "ThreeТри", "Four"};
  auto it = res.cbegin();
  for (int i = 0; i < 4; i++) {
    assert(it->first == i + 1);
    assert(it->second == info_expected[i]);
    it.next();
  }
}

bool lengthGreaterThanThree(const string& str) { return str.size() > 3; }

void filter() {
  Ring<string, int> source;
  string keys[] = {"un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf", "dix"};
  for (int i = 0; i < 10; i++) {
    source.push_back(keys[i], i + 1);
  }

  auto res = filter(source, lengthGreaterThanThree);

  string res_keys[] = {"deux", "trois", "quatre", "cinq", "sept", "huit", "neuf"};
  int res_infos[] = {2, 3, 4, 5, 7, 8, 9};
  int i = 0;
  for (auto it = res.cbegin(); it != res.cend(); it.next()) {
    assert(it->first == res_keys[i]);
    assert(it->second == res_infos[i]);
    i++;
  }
}

void shuffle() {
  Ring<string, int> first;
  Ring<string, int> second;
  string first_keys[] = {"uno",
                              "due",
                              "tre",
                              "quattro"};
  string second_keys[] = {"bir",
                               "iki",
                               "uc",
                               "dort",
                               "bes"};
  for (int i = 0; i < 4; i++) {
    first.push_back(first_keys[i], i + 1);
  }
  for (int i = 0; i < 5; i++) {
    second.push_back(second_keys[i], i + 1);
  }

  auto res = shuffle(first, 1, second, 2, 3);
  int exp_infos[] = {1, 1, 2, 2, 3, 4, 3, 5, 1};
  string exp_keys[] = {"uno",
                            "bir",
                            "iki",
                            "due",
                            "uc",
                            "dort",
                            "tre",
                            "bes",
                            "bir"};

  auto it = res.cbegin();
  for (int i = 0; i < 9; i++) {
    assert(it->first == exp_keys[i]);
    assert(it->second == exp_infos[i]);
    it.next();
  }
}