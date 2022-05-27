//
// Created by 김태현 on 2022/05/21.
//

#ifndef BP_TREE_DATA_STRUCTURE_DATA_NODE_H_
#define BP_TREE_DATA_STRUCTURE_DATA_NODE_H_

#include <memory>
#include "data.h"

template <typename Key, typename Value>
class RawDataNode;

template <typename Key, typename Value>
using DataNodeShared = std::shared_ptr<RawDataNode<Key, Value>>;

template<typename Key, typename Value>
class DataNode {
 public:
  static DataNodeShared<Key, Value> create(DataNodeShared<Key, Value> left = nullptr, DataNodeShared<Key, Value> right = nullptr);
};

template <typename Key, typename Value>
DataNodeShared<Key, Value> DataNode<Key, Value>::create(DataNodeShared<Key, Value> left, DataNodeShared<Key, Value> right) {
  std::shared_ptr<RawDataNode<Key, Value>> new_data_node(new RawDataNode<Key, Value>(left, right));
  return new_data_node;
}

enum class Direction{
  Left,
  Right,
};

template <typename Key, typename Value>
class RawDataNode{
  friend class DataNode<Key, Value>;
 public:
  DataUnique<Key, Value> get_data(int index);
  [[nodiscard]] int get_data_count() const {return data.size();}
  Key get_data_key(int index) const {return data[index]->get_key();}
  Value get_data_value(int index) const {return data[index]->get_value();}
  void set_siblings(Direction direction, DataNodeShared<Key, Value> data_node_shared);
  DataNodeShared<Key, Value> get_siblings(Direction direction);
  int search(Key key) const;
  void insert(int index, DataUnique<Key, Value> data);
  void erase(int index);

 private:
  RawDataNode(DataNodeShared<Key, Value> left, DataNodeShared<Key, Value> right): left(left), right(right) {}
  std::vector<DataUnique<Key, Value>> data;
  DataNodeShared<Key, Value> left, right;
};

template<typename Key, typename Value>
void RawDataNode<Key, Value>::insert(int index, DataUnique<Key, Value> new_data) {
  data.insert(data.begin() + index, std::move(new_data));
}
template<typename Key, typename Value>
void RawDataNode<Key, Value>::set_siblings(Direction direction, DataNodeShared<Key, Value> data_node_shared) {
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
DataNodeShared<Key, Value> RawDataNode<Key, Value>::get_siblings(Direction direction) {
  switch (direction) {
    case Direction::Left:
      return left;
    case Direction::Right:
      return right;
  }
  return nullptr;
}
template<typename Key, typename Value>
DataUnique<Key, Value> RawDataNode<Key, Value>::get_data(int index) {
  DataUnique<Key, Value> d = std::move(data[index]);
  data.erase(data.begin() + index);
  return std::move(d);
}
template<typename Key, typename Value>
int RawDataNode<Key, Value>::search(Key key) const {
  int index = 0;
  for (auto& d: data){
    if (d->get_key() >= key){
      break;
    }
    index++;
  }
  return index;
}
template<typename Key, typename Value>
void RawDataNode<Key, Value>::erase(int index) {
  if (index < 0 || index >= data.size()) {
    std::cout<<"Wrong index!"<<std::endl;
    exit(1);
  } else {
    data.erase(data.begin() + index);
  }
}

#endif //BP_TREE_DATA_STRUCTURE_DATA_NODE_H_
