//
// Created by 김태현 on 2022/05/21.
//

#ifndef BP_TREE_TREE_BP_TREE_H_
#define BP_TREE_TREE_BP_TREE_H_

#include <index_node.h>
#include <data_node.h>
#include <data.h>
#include <cmath>
#include <stack>

template <typename Key, typename Value>
class BPTree{
 public:
  explicit BPTree(int index_degree, int data_degree)
  : head(nullptr), max_key_count(index_degree - 1), min_key_count((int)ceil(((double)index_degree - 1) / 2) - 1),
  max_data_count(data_degree), min_data_count(data_degree / 2){};
  void insert(DataUnique<Key, Value> data);
  void split_data(IndexNodeShared<Key, Value> parent, int data_node_index);
  void split_head(IndexNodeShared<Key, Value> current);
  void split_keys(IndexNodeShared<Key, Value> parent, IndexNodeShared<Key, Value> current);

  void print() const;

 private:
  IndexNodeShared<Key, Value> head;
  DataNodeShared<Key, Value> data_head;
  int max_key_count, min_key_count;
  int max_data_count, min_data_count;

  void print_node(IndexNodeShared<Key, Value> node, int depth) const;

};
template<typename Key, typename Value>
void BPTree<Key, Value>::insert(DataUnique<Key, Value> data) {
  if (head == nullptr){
    head = IndexNode::create<Key, Value>();
    head->insert(0, data->get_key());
    auto data_node = DataNode::create<Key, Value>();
    data_node->insert(0, std::move(data));
    head->set_data_node(0, DataNode::create<Key, Value>());
    head->set_data_node(1, data_node);
    data_head = head->get_data_node(0);
    head->get_data_node(0)->set_siblings(Direction::Left, nullptr);
    head->get_data_node(0)->set_siblings(Direction::Right, head->get_data_node(1));
    head->get_data_node(1)->set_siblings(Direction::Left, head->get_data_node(0));
    head->get_data_node(1)->set_siblings(Direction::Right, nullptr);
  } else {
    std::stack<IndexNodeShared<Key, Value>> stack;
    Key key = data->get_key();
    IndexNodeShared<Key, Value> current_node = head;
    int index = 0;
    do {
      index = current_node->search_key(key);
      stack.push(current_node);
      current_node = current_node->get_pointer(index);
    } while (current_node != nullptr);

    current_node = stack.top();
    stack.pop();
    auto current_data_node = current_node->get_data_node(index);
    int data_index = current_data_node->search(key);
    current_data_node->insert(data_index, std::move(data));

    if (current_data_node->get_data_count() > max_data_count){
      split_data(current_node, index);
      while (current_node->get_keys_count() > max_key_count){
        if (current_node == head){
          split_head(current_node);
          return;
        } else {
          split_keys(stack.top(), current_node);
        }
        current_node = stack.top();
        stack.pop();
      }
    }
  }

}
template<typename Key, typename Value>
void BPTree<Key, Value>::split_data(IndexNodeShared<Key, Value> parent, int data_node_index) {
  auto data_node = parent->get_data_node(data_node_index);
  auto data_node_left = DataNode::create<Key, Value>();
  auto data_node_right = DataNode::create<Key, Value>();

  auto data_count = data_node->get_data_count();
  for (int i = 0; i < data_count / 2; i++){
    data_node_left->insert(data_node_left->get_data_count(), std::move(data_node->get_data(0)));
  }
  if (data_node->get_siblings(Direction::Left) != nullptr) {
    data_node->get_siblings(Direction::Left)->set_siblings(Direction::Right, data_node_left);
  }
  data_node_left->set_siblings(Direction::Left, data_node->get_siblings(Direction::Left));

  data_count = data_node->get_data_count();
  for (int i = 0; i < data_count; i++){
    data_node_right->insert(data_node_right->get_data_count(), std::move(data_node->get_data(0)));
  }
  if (data_node->get_siblings(Direction::Right) != nullptr){
    data_node->get_siblings(Direction::Right)->set_siblings(Direction::Left, data_node_right);
  }
  data_node_right->set_siblings(Direction::Right, data_node->get_siblings(Direction::Right));

  data_node_left->set_siblings(Direction::Right, data_node_right);
  data_node_right->set_siblings(Direction::Left, data_node_left);

  parent->insert(data_node_index , data_node_right->get_data_key(0));
  parent->set_data_node(data_node_index, data_node_left);
  parent->set_data_node(data_node_index + 1, data_node_right);
  data_node.reset();
}
template<typename Key, typename Value>
void BPTree<Key, Value>::split_keys(IndexNodeShared<Key, Value> parent, IndexNodeShared<Key, Value> current) {
  Key new_key = current->get_key(current->get_keys_count() / 2);
  auto left_node = IndexNode::create<Key, Value>();
  auto right_node = IndexNode::create<Key, Value>();
  for (int i = 0; i < current->get_keys_count() / 2; ++i) {
    left_node->insert(left_node->get_keys_count(), current->get_key(i));
  }
  for (int i = 0; i < current->get_pointers_count() / 2; i++) {
    left_node->set_pointer(i, current->get_pointer(i));
  }
  for (int i = current->get_keys_count() / 2 + 1; i < current->get_keys_count(); i++) {
    right_node->insert(right_node->get_keys_count(), current->get_key(i));
  }
  for (int i = current->get_pointers_count() / 2; i < current->get_pointers_count(); ++i) {
    right_node->set_pointer(i - current->get_pointers_count() / 2, current->get_pointer(i));
  }
  if (current->is_leaf()) {
    for (int i = 0; i < current->get_data_node_count() / 2; i++) {
      left_node->set_data_node(i, current->get_data_node(i));
    }
    for (int i = current->get_data_node_count() / 2; i < current->get_data_node_count(); ++i) {
      right_node->set_data_node(i - current->get_data_node_count() / 2, current->get_data_node(i));
    }
  }
  int index = parent->search_key(new_key);
  parent->insert(index, new_key);
  parent->set_pointer(index, left_node);
  parent->set_pointer(index + 1, right_node);
}
template<typename Key, typename Value>
void BPTree<Key, Value>::split_head(IndexNodeShared<Key, Value> current_head) {
  if (current_head != head){
    return;
  } else {
    auto new_head = IndexNode::create<Key, Value>();
    auto left_node = IndexNode::create<Key, Value>();
    auto right_node = IndexNode::create<Key, Value>();
    for (int i = 0; i < current_head->get_keys_count() / 2; ++i) {
      left_node->insert(left_node->get_keys_count(), current_head->get_key(i));
    }
    for (int i = 0; i < current_head->get_pointers_count() / 2; i++) {
      left_node->set_pointer(i, current_head->get_pointer(i));
    }
    for (int i = 0; i < current_head->get_data_node_count() / 2; i++) {
      left_node->set_data_node(i, current_head->get_data_node(i));
    }
    for (int i = current_head->get_keys_count() / 2 + 1; i < current_head->get_keys_count(); i++) {
      right_node->insert(right_node->get_keys_count(), current_head->get_key(i));
    }
    for (int i = current_head->get_pointers_count() / 2; i < current_head->get_pointers_count(); ++i) {
      right_node->set_pointer(i - current_head->get_pointers_count() / 2, current_head->get_pointer(i));
    }
    for (int i = current_head->get_data_node_count() / 2; i < current_head->get_data_node_count(); ++i) {
      right_node->set_data_node(i - current_head->get_data_node_count() / 2, current_head->get_data_node(i));
    }
    new_head->insert(0, current_head->get_key(current_head->get_keys_count() / 2));
    new_head->set_pointer(0, left_node);
    new_head->set_pointer(1, right_node);
    if (current_head->is_leaf()){
      left_node->set_leaf(LEAF);
      right_node->set_leaf(LEAF);
    } else {
      left_node->set_leaf(NOT_LEAF);
      right_node->set_leaf(NOT_LEAF);
    }
    new_head->set_leaf(NOT_LEAF);
    head = new_head;
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::print() const {
  this->print_node(head, 1);
}
template<typename Key, typename Value>
void BPTree<Key, Value>::print_node(IndexNodeShared<Key, Value> node, int depth) const {
  if (node == nullptr){
    return;
  }
  std::cout << "Level: " << depth << std::endl;
  std::cout << "key: ";
  for (int i = 0; i < node->get_keys_count(); i++){
     std::cout << node->get_key(i) << "  ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
  depth++;
  for (int i = 0; i < node->get_pointers_count() && node->is_leaf() == NOT_LEAF; i++){
    print_node(node->get_pointer(i), depth);
  }
}

#endif //BP_TREE_TREE_BP_TREE_H_
