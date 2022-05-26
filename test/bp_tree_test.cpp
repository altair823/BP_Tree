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
  int min_key = 0;
  int max_key = 100000;
  BPTree<int, std::string> bp_tree(50, 50);
//  auto data = make_test_population_string();
  auto data = make_population_string(min_key, max_key);
  for (auto & i : data){
    bp_tree.insert(std::move(i));
  }
  std::vector<int> d = make_keys(min_key, max_key);
  for (auto& i: d){
    if (!bp_tree.remove(i)){
      std::cout<<i<<std::endl;
    }
  }
  bp_tree.print();
  ASSERT_TRUE(bp_tree.is_empty());
}
