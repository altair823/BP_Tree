//
// Created by 김태현 on 2022/05/21.
//

#include <gtest/gtest.h>
#include "test_util.h"
#include <bp_tree.h>
#include <algorithm>

TEST(BPTreeTest, InsertTest){
  BPTree<int, int> bp_tree(30, 50);

  auto data = make_population_int(0, 100000);

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
  auto value = bp_tree.search(3).expect("Not Found");
  ASSERT_EQ(value, "3");
  ASSERT_TRUE(bp_tree.search(31).isErr());
}