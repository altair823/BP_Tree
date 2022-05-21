//
// Created by 김태현 on 2022/05/21.
//

#include <gtest/gtest.h>
#include <bp_tree.h>

TEST(BPTreeTest, InsertTest){
  BPTree<int, int> bp_tree(4, 2);
  std::vector<DataUnique<int, int>> data;
  for (int i = 0; i < 25; i++){
    data.push_back(Data::create(i, i));
  }
  for (auto& d: data){
    bp_tree.insert(std::move(d));
  }
  bp_tree.print();
}