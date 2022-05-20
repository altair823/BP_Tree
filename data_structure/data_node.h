//
// Created by 김태현 on 2022/05/21.
//

#ifndef BP_TREE_DATA_STRUCTURE_DATA_NODE_H_
#define BP_TREE_DATA_STRUCTURE_DATA_NODE_H_

#include <memory>
#include "data.h"

template <typename Key, typename Value>
class _DataNode;

template <typename Key, typename Value>
using DataNodeShared = std::shared_ptr<_DataNode<Key, Value>>;

template <typename Key, typename Value>
class _DataNode{
 public:
  void insert(int index, DataUnique<Key, Value> data);

 private:
  std::vector<DataUnique<Key, Value>> data;
};

#endif //BP_TREE_DATA_STRUCTURE_DATA_NODE_H_
