//
// Created by 김태현 on 2022/05/20.
//

#include "gtest/gtest.h"
#include <index_node.h>

TEST(IndexNodeTest, KeySearchTest){
  auto node = IndexNode::create<int, int>();
  std::vector<int> keys;
  for (int i = 10; i >= 0; i = i - 2){
    keys.push_back(i);
  }
  for (auto& k: keys){
    node->insert_key(0, k);
  }
  auto index = node->search_key(3);
  ASSERT_EQ(index, 2);
}

TEST(IndexNodeTest, PointerSearchTest){
  auto node = IndexNode::create<int, int>();
  std::vector<int> keys;
  for (int i = 5; i >= 0; i--){
    keys.push_back(i);
  }
  for (auto& k: keys){
    node->insert_key(0, k);
  }
  auto target_node = IndexNode::create<int, int>();
  node->set_pointer(3, target_node);
  auto index = node->search_pointer(target_node);
  ASSERT_EQ(index, 3);
}

TEST(IndexNodeTest, InsertTest){

}