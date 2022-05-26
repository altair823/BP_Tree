//
// Created by 김태현 on 2022/05/23.
//

#include <vector>
#include <data.h>
#include <random>


std::vector<DataUnique<int, int>> make_population_int(int min, int max, bool is_random = true){
  std::vector<DataUnique<int, int>> data;
  for (int i = min; i < max; i++){
    data.push_back(Data<int, int>::create(i, i));
  }
  if (is_random) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
  }
  return data;
}

std::vector<DataUnique<int, std::string>> make_population_string(int min, int max, bool is_random = true){
  std::vector<DataUnique<int, std::string>> data;
  for (int i = min; i < max; i++){
    data.push_back(Data<int, std::string>::create(i, std::to_string(i)));
  }
  if (is_random) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
  }
  return data;
}

template<typename Key>
std::vector<Key> make_keys(Key min, Key max, bool is_random = true){
  std::vector<Key> keys;
  for (int i = min; i < max; i++){
    keys.push_back(i);
  }
  if (is_random) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(keys.begin(), keys.end(), g);
  }
  return keys;
}

std::vector<DataUnique<int, std::string>> make_test_population_string(){
  std::vector<int> d {8, 17, 10, 26, 9, 21, 29, 7, 11, 12, 4, 23, 20, 2, 16, 0, 13, 24, 15, 18, 5, 27, 22, 3, 1, 25, 28, 6, 19, 14};
  std::vector<DataUnique<int, std::string>> data;
  for (auto i: d){
    data.push_back(Data<int, std::string>::create(i, std::to_string(i)));
  }
  return data;
}
