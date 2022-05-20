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

namespace DataNode {
  template<typename Key, typename Value>
  DataNodeShared<Key, Value> create();
}

enum class Direction{
  Left,
  Right,
};

template <typename Key, typename Value>
class _DataNode{
  friend DataNodeShared<Key, Value> DataNode::create();
 public:
  DataUnique<Key, Value> get_data(int index);
  Key get_data_key(int index) const {return data[index]->get_key();}
  Value get_data_value(int index) const {return data[index]->get_value();}
  void insert(int index, DataUnique<Key, Value> data);
  void set_siblings(Direction direction, DataNodeShared<Key, Value> data_node_shared);

 private:
  _DataNode(): left(nullptr), right(nullptr) {};
  std::vector<DataUnique<Key, Value>> data;
  DataNodeShared<Key, Value> left, right;
};

template <typename Key, typename Value>
DataNodeShared<Key, Value> DataNode::create() {
  std::shared_ptr<_DataNode<Key, Value>> new_data_node(new _DataNode<Key, Value>());
  return new_data_node;
}
template<typename Key, typename Value>
void _DataNode<Key, Value>::insert(int index, DataUnique<Key, Value> new_data) {
  data.insert(data.begin() + index, std::move(new_data));
}
template<typename Key, typename Value>
void _DataNode<Key, Value>::set_siblings(Direction direction, DataNodeShared<Key, Value> data_node_shared) {
  switch (direction) {
    case Direction::Left:
      left = data_node_shared;
      break;
    case Direction::Right:
      right = data_node_shared;
      break;
  }
}
template<typename Key, typename Value>
DataUnique<Key, Value> _DataNode<Key, Value>::get_data(int index) {
  auto d = std::move(data[index]);
  data.erase(data.begin() + index);
  return d;
}

#endif //BP_TREE_DATA_STRUCTURE_DATA_NODE_H_
