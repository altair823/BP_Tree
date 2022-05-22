//
// Created by 김태현 on 2022/05/20.
//

#ifndef BP_TREE_DATA_STRUCTURE_INDEX_NODE_H_
#define BP_TREE_DATA_STRUCTURE_INDEX_NODE_H_

#include <vector>
#include <iostream>
#include "data_node.h"

#define LEAF true
#define NOT_LEAF false

template <typename Key, typename Value>
class RawIndexNode;

template <typename Key, typename Value>
using IndexNodeShared = std::shared_ptr<RawIndexNode<Key, Value>>;

namespace IndexNode{
template <typename Key, typename Value>
  IndexNodeShared<Key, Value> create();
}

template <typename Key, typename Value>
class RawIndexNode{
 public:
  Key get_key(int index) const {return keys[index];};
  IndexNodeShared<Key, Value> get_pointer(int index) const {return pointers[index];};
  DataNodeShared<Key, Value> get_data_node(int index) const {return data_nodes[index];};
  bool is_leaf(){return this->is_node_leaf;}
  [[nodiscard]] int get_keys_count() const {return keys.size();}
  [[nodiscard]] int get_pointers_count() const {return pointers.size();}
  [[nodiscard]] int get_data_node_count() const {return data_nodes.size();}
  void set_key(int index, Key &key) {keys[index] = key;};
  void set_pointer(int index, const IndexNodeShared<Key, Value> &pointer) {pointers[index] = pointer;};
  void set_data_node(int index, DataNodeShared<Key, Value> data_node);
  void set_leaf(bool is_leaf);
  void insert_key(int index, Key key);
  void erase(int key_index, int pointer_index);
  void erase_data_node(int index);
  int search_key(const Key &key);
  int search_pointer(const IndexNodeShared<Key, Value> &pointer);

  friend IndexNodeShared<Key, Value> IndexNode::create();

 private:
  RawIndexNode();
  std::vector<Key> keys;
  std::vector<IndexNodeShared<Key, Value>> pointers;
  std::vector<DataNodeShared<Key, Value>> data_nodes;
  bool is_node_leaf;
};

template<typename Key, typename Value>
IndexNodeShared<Key, Value> IndexNode::create() {
  std::shared_ptr<RawIndexNode<Key, Value>> new_node(new RawIndexNode<Key, Value>());
  return new_node;
}
template<typename Key, typename Value>
RawIndexNode<Key, Value>::RawIndexNode(): is_node_leaf(LEAF) {
  pointers.push_back(nullptr);
}
template<typename Key, typename Value>
void RawIndexNode<Key, Value>::insert_key(int index, Key key) {
  keys.insert(keys.begin() + index, key);
  pointers.insert(pointers.begin() + index, nullptr);
  if (is_node_leaf){
    data_nodes.insert(data_nodes.begin() + index, nullptr);
  }
}
template<typename Key, typename Value>
void RawIndexNode<Key, Value>::erase(int key_index, int pointer_index) {
  if (key_index >= keys.size() || pointer_index >= pointers.size() || key_index < 0 || pointer_index < 0){
    std::cout<<"Wrong index!"<<std::endl;
  }
  keys.erase(keys.begin() + key_index);
  pointers.erase(pointers.begin() + pointer_index);
}
template<typename Key, typename Value>
int RawIndexNode<Key, Value>::search_key(const Key &key) {
  int index = 0;
  for (auto& k: keys){
    if (k >= key){
      break;
    }
    index++;
  }
  return index;
}
template<typename Key, typename Value>
int RawIndexNode<Key, Value>::search_pointer(const IndexNodeShared<Key, Value> &pointer) {
  auto raw_pointer = pointer.get();
  int index = 0;
  for (auto& p: pointers){
    if (p.get() == raw_pointer){
      break;
    }
    index++;
  }
  return index;
}
template<typename Key, typename Value>
void RawIndexNode<Key, Value>::set_data_node(int index, DataNodeShared<Key, Value> data_node) {
  if (is_node_leaf){
    if (index >= data_nodes.size()) {
      data_nodes.resize(index + 1);
      data_nodes[index] = data_node;
    } else {
      data_nodes[index] = data_node;
    }
  } else {
    std::cout<<"Node is not leaf!";
    exit(1);
  }
}
template<typename Key, typename Value>
void RawIndexNode<Key, Value>::erase_data_node(int index) {
  if (index < 0 || index >= data_nodes.size()){
    std::cout<<"Wrong index!"<<std::endl;
  }
  data_nodes.erase(data_nodes.begin() + index);
}
template<typename Key, typename Value>
void RawIndexNode<Key, Value>::set_leaf(bool is_leaf) {
  this->is_node_leaf = is_leaf;
  if (is_leaf == NOT_LEAF){
    data_nodes.clear();
  }
}

#endif //BP_TREE_DATA_STRUCTURE_INDEX_NODE_H_
