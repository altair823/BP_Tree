//
// Created by 김태현 on 2022/05/21.
//

#include <gtest/gtest.h>
#include <data_node.h>

TEST(DataNodeTest, CreateTest){
  auto data_node = DataNode<int, int>::create();
  data_node->insert(0, std::move(Data<int, int>::create(3, 4)));
  ASSERT_EQ(data_node->get_data_key(0), 3);
  ASSERT_EQ(data_node->get_data_value(0), 4);
  auto data = std::move(data_node->get_data(0));
}