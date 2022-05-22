//
// Created by 김태현 on 2022/05/21.
//

#include <gtest/gtest.h>
#include <bp_tree.h>
#include <random>
#include <algorithm>

TEST(BPTreeTest, InsertTest){
  BPTree<int, int> bp_tree(20, 20);
  std::vector<DataUnique<int, int>> data;
  for (int i = 0; i < 100000; i++){
    data.push_back(Data::create(i, i));
  }
  auto rng = std::default_random_engine {};
  std::shuffle(data.begin(), data.end(), rng);
  for (auto& d: data){
    bp_tree.insert(std::move(d));
  }
  bp_tree.print();
}