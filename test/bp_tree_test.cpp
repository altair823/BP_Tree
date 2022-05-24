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
  BPTree<int, std::string> bp_tree(3, 3);
  auto data = make_population_string(0, 30, false);
  for (auto & i : data){
    bp_tree.insert(std::move(i));
  }
  bp_tree.remove(4);
  bp_tree.print();
//  ASSERT_TRUE(bp_tree.remove(3));
}