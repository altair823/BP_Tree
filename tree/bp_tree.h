//
// Created by 김태현 on 2022/05/21.
//

#ifndef BP_TREE_TREE_BP_TREE_H_
#define BP_TREE_TREE_BP_TREE_H_

#include <index_node.h>
#include <data_node.h>
#include <data.h>
#include <cmath>

template <typename Key, typename Value>
class BPTree{
 public:
  explicit BPTree(int degree): head(nullptr), max_key_count(degree - 1), min_key_count((int)ceil(((double)degree - 1) / 2) - 1){};
  void insert(DataUnique<Key, Value> data);

 private:
  IndexNodeShared<Key, Value> head;
  int max_key_count, min_key_count;

};
template<typename Key, typename Value>
void BPTree<Key, Value>::insert(DataUnique<Key, Value> data) {
  if (head == nullptr){
    head = IndexNode::create<Key, Value>();
    head->insert(0, data->get_key());
    auto data_node = DataNode::create<Key, Value>();
    data_node->insert(0, std::move(data));
    head->set_data_node()
  }
}

#endif //BP_TREE_TREE_BP_TREE_H_
