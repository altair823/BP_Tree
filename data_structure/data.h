//
// Created by 김태현 on 2022/05/21.
//

#ifndef BP_TREE_DATA_STRUCTURE_DATA_H_
#define BP_TREE_DATA_STRUCTURE_DATA_H_

#include <memory>

template <typename Key, typename Value>
class RawData;

template<typename Key, typename Value>
class Data {
 public:
  static std::unique_ptr<RawData<Key, Value>> create(Key key, Value value) {
    std::unique_ptr<::RawData<Key, Value>> new_data(new RawData<Key, Value>(key, value));
    return new_data;
  }
};

template <typename Key, typename Value>
class RawData{
  friend class Data<Key, Value>;
 public:
  Key get_key(){return key;}
  Value get_value(){return value;}
 private:
  RawData(Key key, Value value): key(key), value(value){}
  Key key;
  Value value;
};

template <typename Key, typename Value>
using DataUnique = std::unique_ptr<RawData<Key, Value>>;

#endif //BP_TREE_DATA_STRUCTURE_DATA_H_
