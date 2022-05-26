//
// Created by 김태현 on 2022/05/21.
//

#ifndef BP_TREE_TREE_BP_TREE_H_
#define BP_TREE_TREE_BP_TREE_H_

#include <index_node.h>
#include <data_node.h>
#include "../result/result.h"
#include <data.h>
#include <cmath>
#include <stack>

template <typename Key, typename Value>
struct TargetLeaf {
  TargetLeaf(IndexNodeShared<Key, Value> leaf_index_node, int data_node_index, std::stack<IndexNodeShared<Key, Value>> parents)
  : leaf_index_node(leaf_index_node), data_node_index(data_node_index), parents(std::move(parents)){}
  IndexNodeShared<Key, Value> leaf_index_node;
  int data_node_index;
  std::stack<IndexNodeShared<Key, Value>> parents;
};

template <typename Key, typename Value>
class BPTree{
 public:
  explicit BPTree(int index_degree, int data_degree)
  : head(nullptr), max_key_count(index_degree - 1), min_key_count((int)ceil(((double)index_degree - 1) / 2)),
  max_data_count(data_degree), min_data_count((int)ceil((double)data_degree / 2)){};
  void insert(DataUnique<Key, Value> data);
  Result<Value, std::string> search(Key key);
  bool remove(Key key);

  bool is_empty(){return head == nullptr;}
  void print() const;

 private:
  IndexNodeShared<Key, Value> head;
  DataNodeShared<Key, Value> data_head;
  int max_key_count, min_key_count;
  int max_data_count, min_data_count;

  TargetLeaf<Key, Value> search_to_leaf(Key key);

  void split_data(IndexNodeShared<Key, Value> parent, int data_node_index);
  void split_head(IndexNodeShared<Key, Value> current);
  void split_keys(IndexNodeShared<Key, Value> parent, IndexNodeShared<Key, Value> current);

  void solve_data(IndexNodeShared<Key, Value> parent_index_node, int data_node_index);
  void solve_index(std::stack<IndexNodeShared<Key, Value>> &parents, IndexNodeShared<Key, Value> current);


  void merge_data_node_right(IndexNodeShared<Key, Value> parent, int data_node_index);
  void borrow_data_left(IndexNodeShared<Key, Value> parent, int data_node_index);
  void borrow_data_right(IndexNodeShared<Key, Value> parent, int data_node_index);

  void spin_clockwise(IndexNodeShared<Key, Value> parent, int current_node_index);
  void spin_counterclockwise(IndexNodeShared<Key, Value> parent, int current_node_index);
  void merge_index_left(IndexNodeShared<Key, Value> parent, int index);
  void merge_index_right(IndexNodeShared<Key, Value> parent, int index);

  void print_index_node(IndexNodeShared<Key, Value> node, int depth) const;
  void print_data_node(DataNodeShared<Key, Value> node, int depth) const;

};

template<typename Key, typename Value>
TargetLeaf<Key, Value> BPTree<Key, Value>::search_to_leaf(Key key) {
  std::stack<IndexNodeShared<Key, Value>> stack;
  IndexNodeShared<Key, Value> current_node = head;
  int index;
  do {
    index = current_node->search_key(key);
    stack.push(current_node);
    current_node = current_node->get_pointer(index);
  } while (current_node != nullptr);
  current_node = stack.top();
  stack.pop();
  return TargetLeaf<Key, Value>(current_node, index, stack);
}
template<typename Key, typename Value>
void BPTree<Key, Value>::insert(DataUnique<Key, Value> data) {
  if (head == nullptr){
    head = IndexNode<Key, Value>::create();
    head->insert_key(0, data->get_key());
    auto data_node = DataNode<Key, Value>::create();
    data_node->insert(0, std::move(data));
    head->set_data_node(0, DataNode<Key, Value>::create());
    head->set_data_node(1, data_node);
    data_head = head->get_data_node(0);
    head->get_data_node(0)->set_siblings(Direction::Left, nullptr);
    head->get_data_node(0)->set_siblings(Direction::Right, head->get_data_node(1));
    head->get_data_node(1)->set_siblings(Direction::Left, head->get_data_node(0));
    head->get_data_node(1)->set_siblings(Direction::Right, nullptr);
  } else {

    Key key = data->get_key();
    auto target_leaf = search_to_leaf(key);
    std::stack<IndexNodeShared<Key, Value>> parents = target_leaf.parents;
    IndexNodeShared<Key, Value> current_node = target_leaf.leaf_index_node;
    int index = target_leaf.data_node_index;

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
          split_keys(parents.top(), current_node);
        }
        current_node = parents.top();
        parents.pop();
      }
    }
  }

}
template<typename Key, typename Value>
void BPTree<Key, Value>::split_data(IndexNodeShared<Key, Value> parent, int data_node_index) {

  if (data_node_index != 0
  && parent->get_data_node(data_node_index - 1)->get_data_count() < min_data_count){
    borrow_data_right(parent, data_node_index - 1);
  }

  auto data_node = parent->get_data_node(data_node_index);
  auto data_node_left = DataNode<Key, Value>::create();
  auto data_node_right = DataNode<Key, Value>::create();

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

  parent->insert_key(data_node_index, data_node_right->get_data_key(0));
  parent->set_data_node(data_node_index, data_node_left);
  parent->set_data_node(data_node_index + 1, data_node_right);
  data_node.reset();
}
template<typename Key, typename Value>
void BPTree<Key, Value>::split_keys(IndexNodeShared<Key, Value> parent, IndexNodeShared<Key, Value> current) {
  Key new_key = current->get_key(current->get_keys_count() / 2);
  auto left_node = IndexNode<Key, Value>::create();
  auto right_node = IndexNode<Key, Value>::create();
  auto key_index = current->get_keys_count() / 2;
  for (int i = 0; i < key_index; ++i) {
    left_node->insert_key(left_node->get_keys_count(), current->get_key(i));
  }
  for (int i = 0; i <= key_index; i++) {
    left_node->set_pointer(i, current->get_pointer(i));
  }
  for (int i = key_index + 1; i < current->get_keys_count(); i++) {
    right_node->insert_key(right_node->get_keys_count(), current->get_key(i));
  }
  for (int i = key_index + 1; i < current->get_pointers_count(); ++i) {
    right_node->set_pointer(i - (key_index + 1), current->get_pointer(i));
  }
  if (current->is_leaf()) {
    for (int i = 0; i <= key_index; i++) {
      left_node->set_data_node(i, current->get_data_node(i));
    }
    for (int i = key_index + 1; i < current->get_data_node_count(); ++i) {
      right_node->set_data_node(i - (key_index + 1), current->get_data_node(i));
    }
  }

  if (current->is_leaf()){
    left_node->set_leaf(LEAF);
    right_node->set_leaf(LEAF);
  } else {
    left_node->set_leaf(NOT_LEAF);
    right_node->set_leaf(NOT_LEAF);
  }

  int index = parent->search_key(new_key);
  parent->insert_key(index, new_key);
  parent->set_pointer(index, left_node);
  parent->set_pointer(index + 1, right_node);

}
template<typename Key, typename Value>
void BPTree<Key, Value>::split_head(IndexNodeShared<Key, Value> current_head) {
  if (current_head != head){
    return;
  } else {
    auto new_head = IndexNode<Key, Value>::create();
    auto left_node = IndexNode<Key, Value>::create();
    auto right_node = IndexNode<Key, Value>::create();
    auto key_index = current_head->get_keys_count() / 2;

    for (int i = 0; i < key_index; ++i) {
      left_node->insert_key(left_node->get_keys_count(), current_head->get_key(i));
    }
    for (int i = 0; i <= key_index; i++) {
      left_node->set_pointer(i, current_head->get_pointer(i));
    }
    for (int i = key_index + 1; i < current_head->get_keys_count(); i++) {
      right_node->insert_key(right_node->get_keys_count(), current_head->get_key(i));
    }
    for (int i = key_index + 1; i < current_head->get_pointers_count(); ++i) {
      right_node->set_pointer(i - (key_index + 1), current_head->get_pointer(i));
    }
    if (current_head->is_leaf()) {
      for (int i = 0; i <= key_index; i++) {
        left_node->set_data_node(i, current_head->get_data_node(i));
      }
      for (int i = key_index + 1; i < current_head->get_data_node_count(); ++i) {
        right_node->set_data_node(i - (key_index + 1), current_head->get_data_node(i));
      }
    }
    new_head->insert_key(0, current_head->get_key(current_head->get_keys_count() / 2));
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
  this->print_index_node(head, 1);
}
template<typename Key, typename Value>
void BPTree<Key, Value>::print_index_node(IndexNodeShared<Key, Value> node, int depth) const {
  if (node == nullptr){
    return;
  }
  std::cout << "Level: " << depth << " - Index Node" << std::endl;
  std::cout << "key: ";
  for (int i = 0; i < node->get_keys_count(); i++){
     std::cout << node->get_key(i) << "  ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
  depth++;
  for (int i = 0; i < node->get_pointers_count() && node->is_leaf() == NOT_LEAF; i++){
    print_index_node(node->get_pointer(i), depth);
  }
  for (int i = 0; node->is_leaf() == LEAF && i < node->get_data_node_count(); i++){
    print_data_node(node->get_data_node(i), depth);
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::print_data_node(DataNodeShared<Key, Value> node, int depth) const {
  std::cout << "Level: " << depth << " - Data Node" << std::endl;
  std::cout << "key: ";
  for (int i = 0; i < node->get_data_count(); i++){
     std::cout << node->get_data_key(i) << "  ";
  }
  std::cout << "\nvalue: ";
  for (int i = 0; i < node->get_data_count(); i++){
     std::cout << node->get_data_value(i) << "  ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
}

template<typename Key, typename Value>
Result<Value, std::string> BPTree<Key, Value>::search(Key key) {
  if (is_empty()){
    return Err(std::string("Tree is empty!"));
  }
  auto target_leaf = search_to_leaf(key);
  IndexNodeShared<Key, Value> current_node = target_leaf.leaf_index_node;
  int index = target_leaf.data_node_index;

  auto current_data_node = current_node->get_data_node(index);
  auto data_index = current_data_node->search(key);
  if (data_index < current_data_node->get_data_count() && current_data_node->get_data_key(data_index) == key){
    return Ok(current_data_node->get_data_value(data_index));
  } else{
    return Err(std::string("Cannot found the data in key: " + std::to_string(key)));
  }
}
template<typename Key, typename Value>
bool BPTree<Key, Value>::remove(Key key) {
  if (is_empty()){
    return false;
  }
  auto target_leaf = search_to_leaf(key);
  IndexNodeShared<Key, Value> current_node = target_leaf.leaf_index_node;
  std::stack<IndexNodeShared<Key, Value>> parents = target_leaf.parents;
  int index = target_leaf.data_node_index;

  auto current_data_node = current_node->get_data_node(index);
  auto data_index = current_data_node->search(key);
  if (data_index < current_data_node->get_data_count() && current_data_node->get_data_key(data_index) == key){
    current_data_node->erase(data_index);
    solve_data(current_node, index);
    solve_index(parents, current_node);
  } else {
    return false;
  }
  if (current_node == head && current_node->get_keys_count() == 0){
    head.reset();
  }
  return true;
}
template<typename Key, typename Value>
void BPTree<Key, Value>::solve_data(IndexNodeShared<Key, Value> parent_index_node,
                                    int data_node_index) {
  auto current_data_node = parent_index_node->get_data_node(data_node_index);
  if (current_data_node->get_data_count() >= min_data_count){
    return;
  } else {
    if (parent_index_node->get_data_node_count() - 1 > data_node_index
        && parent_index_node->get_data_node(data_node_index + 1)->get_data_count() == min_data_count) {
      merge_data_node_right(parent_index_node, data_node_index);
    } else if (parent_index_node->get_data_node_count() > 1
        && parent_index_node->get_data_node_count() - 1 == data_node_index
        && parent_index_node->get_data_node(data_node_index - 1)->get_data_count() == min_data_count) {
      merge_data_node_right(parent_index_node, data_node_index - 1);
    } else if (parent_index_node->get_data_node_count() - 1 > data_node_index
        && parent_index_node->get_data_node(data_node_index + 1)->get_data_count() > min_data_count){
      borrow_data_right(parent_index_node, data_node_index);
    } else if ( parent_index_node->get_data_node_count() > 1
        && parent_index_node->get_data_node_count() - 1 == data_node_index
        && parent_index_node->get_data_node(data_node_index - 1)->get_data_count() > min_data_count){
      borrow_data_left(parent_index_node, data_node_index);
    }
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::solve_index(std::stack<IndexNodeShared<Key, Value>> &parents_stack, IndexNodeShared<Key, Value> current) {
  if (current == head || current->get_keys_count() >= min_key_count) {
    return;
  }
  auto parent = parents_stack.top();
  auto index = parent->search_pointer(current);
  auto current_node = current;
  if (index != parent->get_pointers_count() - 1
      && parent->get_pointer(index + 1)->get_keys_count() >= min_key_count + 1) {
    spin_counterclockwise(parent, index);
  } else if (index != 0 && parent->get_pointer(index - 1)->get_keys_count() > min_key_count) {
    spin_clockwise(parent, index);
  } else if (index != 0 && parent->get_pointer(index - 1)->get_keys_count() <= min_key_count) {
    merge_index_left(parent, index);
  } else if (index != parent->get_pointers_count() - 1
      && parent->get_pointer(index + 1)->get_keys_count() <= min_key_count) {
    merge_index_right(parent, index);
  }
  current_node = parent;
  parents_stack.pop();
  if (current_node->get_keys_count() < min_key_count && current_node != head) {
    solve_index(parents_stack, current_node);
  } else if (current_node == head && current_node->get_keys_count() == 0) {
    head = current_node->get_pointer(0);
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::merge_data_node_right(IndexNodeShared<Key, Value> parent, int data_node_index){
  if (data_node_index == parent->get_data_node_count() - 1 && !parent->is_leaf()){
    return;
  } else {
    auto current_data_node = parent->get_data_node(data_node_index);
    auto right_data_node = parent->get_data_node(data_node_index + 1);
    for (int i = current_data_node->get_data_count() - 1; i >= 0; i--) {
      right_data_node->insert(0, current_data_node->get_data(i));
    }
    right_data_node->set_siblings(Direction::Left, current_data_node->get_siblings(Direction::Left));
    parent->erase_data_node(data_node_index);
    parent->erase(data_node_index, data_node_index);
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::borrow_data_left(IndexNodeShared<Key, Value> parent, int index) {
  if (!parent->is_leaf()){
    return;
  } else {
    auto current_data_node = parent->get_data_node(index);
    auto left_data_node = parent->get_data_node(index - 1);
    auto l_data = left_data_node->get_data(left_data_node->get_data_count() - 1);
    current_data_node->insert(0, std::move(l_data));
    parent->set_key(index - 1, current_data_node->get_data_key(0));
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::borrow_data_right(IndexNodeShared<Key, Value> parent, int index) {
  if (!parent->is_leaf()){
    return;
  } else {
    auto current_data_node = parent->get_data_node(index);
    auto right_data_node = parent->get_data_node(index + 1);
    auto r_data = right_data_node->get_data(0);
    current_data_node->insert(current_data_node->get_data_count(), std::move(r_data));
    parent->set_key(index, right_data_node->get_data_key(0));
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::spin_clockwise(IndexNodeShared<Key, Value> parent, int current_node_index) {
  auto current_node = parent->get_pointer(current_node_index);
  auto left_node = parent->get_pointer(current_node_index - 1);
  if (left_node->get_keys_count() > min_key_count){
    auto p_key = parent->get_key(current_node_index - 1);
    auto l_key = left_node->get_key(left_node->get_keys_count() - 1);
    current_node->insert_key(0, p_key);
    parent->set_key(current_node_index - 1, l_key);
    if (current_node->is_leaf()){
      current_node->set_data_node(0, left_node->get_data_node(left_node->get_data_node_count() - 1));
      left_node->erase(left_node->get_keys_count() - 1, left_node->get_pointers_count() - 1);
      left_node->erase_data_node(left_node->get_data_node_count() - 1);
    } else {
      current_node->set_pointer(0, left_node->get_pointer(left_node->get_pointers_count() - 1));
      left_node->erase(left_node->get_keys_count() - 1, left_node->get_pointers_count() - 1);
    }
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::spin_counterclockwise(IndexNodeShared<Key, Value> parent, int current_node_index) {
  auto current_node = parent->get_pointer(current_node_index);
  auto right_node = parent->get_pointer(current_node_index + 1);
  if (right_node->get_keys_count() > min_key_count){
    auto p_key = parent->get_key(current_node_index);
    auto r_key = right_node->get_key(0);
    current_node->insert_key(current_node->get_keys_count(), p_key);
    parent->set_key(current_node_index, r_key);
    if (current_node->is_leaf()){
      current_node->set_data_node(current_node->get_data_node_count() - 1, right_node->get_data_node(0));
      right_node->erase(0, 0);
      right_node->erase_data_node(0);
    } else {
      current_node->set_pointer(current_node->get_pointers_count() - 1, right_node->get_pointer(0));
      right_node->erase(0, 0);
    }
  }
}
template<typename Key, typename Value>
void BPTree<Key, Value>::merge_index_left(IndexNodeShared<Key, Value> parent, int index) {
  auto current = parent->get_pointer(index);
  auto left = parent->get_pointer(index - 1);
  auto p_key = parent->get_key(index - 1);
  left->insert_key(left->get_keys_count(), p_key, true);
  for (int i = 0; i < current->get_keys_count(); i++){
    left->insert_key(current->get_keys_count(), current->get_key(i), true);
  }
  for (int i = 0; i < current->get_pointers_count() && !current->is_leaf(); i++){
    left->set_pointer(left->get_pointers_count() - 1, current->get_pointer(i));
  }
  for (int i = 0; i < current->get_data_node_count() && current->is_leaf(); i++){
    left->set_data_node(left->get_data_node_count() - 1, current->get_data_node(i));
  }
  parent->erase(index - 1, index);
}
template<typename Key, typename Value>
void BPTree<Key, Value>::merge_index_right(IndexNodeShared<Key, Value> parent, int index) {
  auto current = parent->get_pointer(index);
  auto right = parent->get_pointer(index + 1);
  auto p_key = parent->get_key(index);
  right->insert_key(0, p_key);
  for (int i = 0; i < current->get_keys_count(); i++){
    right->insert_key(0, current->get_key(current->get_keys_count() - i - 1));
  }
  for (int i = 0; i < current->get_pointers_count() && !current->is_leaf(); i++){
    right->set_pointer(i, current->get_pointer(current->get_pointers_count() - i - 1));
  }
  for (int i = 0; i < current->get_data_node_count() && current->is_leaf(); i++){
    right->set_data_node(i, current->get_data_node(current->get_data_node_count() - i - 1));
  }
  parent->erase(index, index);
}

#endif //BP_TREE_TREE_BP_TREE_H_
