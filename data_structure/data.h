//
// Created by 김태현 on 2022/05/21.
//

#ifndef BP_TREE_DATA_STRUCTURE_DATA_H_
#define BP_TREE_DATA_STRUCTURE_DATA_H_

#include <memory>

template <typename Key, typename Value>
class _Data;

namespace Data {
  template<typename Key, typename Value>
  std::unique_ptr<_Data<Key, Value>> create(Key key, Value value) {
    std::unique_ptr<::_Data<Key, Value>> new_data(new _Data<Key, Value>(key, value));
    return new_data;
  }
}

template <typename Key, typename Value>
class _Data{
  friend std::unique_ptr<_Data<Key, Value>> Data::create(Key key, Value value);
 public:
  Key get_key(){return key;}
  Value get_value(){return value;}
 private:
  _Data(Key key, Value value): key(key), value(value){}
  Key key;
  Value value;
};

template <typename Key, typename Value>
using DataUnique = std::unique_ptr<_Data<Key, Value>>;

#endif //BP_TREE_DATA_STRUCTURE_DATA_H_
