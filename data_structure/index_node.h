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
class _IndexNode;

template <typename Key, typename Value>
using IndexNodeShared = std::shared_ptr<_IndexNode<Key, Value>>;

namespace IndexNode{
template <typename Key, typename Value>
  IndexNodeShared<Key, Value> create();
}

template <typename Key, typename Value>
class _IndexNode{
 public:
  Key get_key(int index) const;
  IndexNodeShared<Key, Value> get_pointer(int index) const;
  bool is_leaf(){return this->is_node_leaf();}
  [[nodiscard]] int get_keys_count() const {return keys.size();}
  [[nodiscard]] int get_pointers_count() const {return pointers.size();}
  void set_key(int index, Key &key);
  void set_pointer(int index, const IndexNodeShared<Key, Value> &pointer);
  void set_leaf(bool is_leaf){this->is_node_leaf = is_leaf;}
  void insert(int index, Key &key);
  void erase(int key_index, int pointer_index);
  int search_key(const Key &key);
  int search_pointer(const IndexNodeShared<Key, Value> &pointer);

  friend IndexNodeShared<Key, Value> IndexNode::create();

 private:
  _IndexNode();
  std::vector<Key> keys;
  std::vector<IndexNodeShared<Key, Value>> pointers;
  std::vector<DataNodeShared<Key, Value>> data_nodes;
  bool is_node_leaf;
};

template<typename Key, typename Value>
IndexNodeShared<Key, Value> IndexNode::create() {
  std::shared_ptr<_IndexNode<Key, Value>> new_node(new _IndexNode<Key, Value>());
  return new_node;
}
template<typename Key, typename Value>
_IndexNode<Key, Value>::_IndexNode(): is_node_leaf(LEAF) {
  pointers.push_back(nullptr);
}
template<typename Key, typename Value>
Key _IndexNode<Key, Value>::get_key(int index) const {
  return keys[index];
}
template<typename Key, typename Value>
IndexNodeShared<Key, Value> _IndexNode<Key, Value>::get_pointer(int index) const {
  return NodeShared<Key, Value>();
}
template<typename Key, typename Value>
void _IndexNode<Key, Value>::insert(int index, Key &key) {
  keys.insert(keys.begin() + index, key);
  pointers.insert(pointers.begin() + index, nullptr);
}
template<typename Key, typename Value>
void _IndexNode<Key, Value>::erase(int key_index, int pointer_index) {
  if (key_index >= keys.size() || pointer_index >= pointers.size() || key_index < 0 || pointer_index < 0){
    std::cout<<"Wrong index!"<<std::endl;
  }
  keys.erase(keys.begin() + key_index);
  pointers.erase(pointers.begin() + pointer_index);
}
template<typename Key, typename Value>
void _IndexNode<Key, Value>::set_key(int index, Key &key) {
  keys[index] = key;
}
template<typename Key, typename Value>
void _IndexNode<Key, Value>::set_pointer(int index, const IndexNodeShared<Key, Value> &pointer) {
  pointers[index] = pointer;
}
template<typename Key, typename Value>
int _IndexNode<Key, Value>::search_key(const Key &key) {
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
int _IndexNode<Key, Value>::search_pointer(const IndexNodeShared<Key, Value> &pointer) {
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

#endif //BP_TREE_DATA_STRUCTURE_INDEX_NODE_H_
