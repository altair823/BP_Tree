//
// Created by 김태현 on 2022/05/21.
//

#include <gtest/gtest.h>
#include "test_util.h"
#include <bp_tree.h>
#include <algorithm>

TEST(BPTreeTest, InsertTest){
  BPTree<int, int> bp_tree(30, 50);

  auto data = make_population_int(0, 100);

  for (auto& d: data){
    bp_tree.insert(std::move(d));
  }
  bp_tree.print();
}

TEST(BPTreeTest, SearchTest){
  BPTree<int, std::string> bp_tree(3, 3);
  auto data = make_population_string(0, 30);
  for (auto & i : data){
    bp_tree.insert(std::move(i));
  }
  auto value = bp_tree.search(2).expect("Not Found");
  ASSERT_EQ(value, "2");
  ASSERT_TRUE(bp_tree.search(31).isErr());
}

TEST(BPTreeTest, RemoveTest){
  BPTree<int, std::string> bp_tree(5, 5);
  auto data = make_population_string(0, 30, true);
  for (auto & i : data){
    bp_tree.insert(std::move(i));
  }
  std::vector<int> d;
  for (int i = 0; i < 31; i++){
    d.push_back(i);
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(d.begin(), d.end(), g);
  for (auto& i: d){
    bp_tree.remove(i);
  }
  bp_tree.print();
//  ASSERT_TRUE(bp_tree.remove(3));
}