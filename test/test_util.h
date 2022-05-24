//
// Created by 김태현 on 2022/05/23.
//

#include <vector>
#include <data.h>
#include <random>

std::vector<DataUnique<int, int>> make_population_int(int min, int max){
  std::vector<DataUnique<int, int>> data;
  for (int i = min; i <= max; i++){
    data.push_back(Data<int, int>::create(i, i));
  }
  auto rng = std::default_random_engine {};
  std::shuffle(data.begin(), data.end(), rng);
  return data;
}

std::vector<DataUnique<int, std::string>> make_population_string(int min, int max){
  std::vector<DataUnique<int, std::string>> data;
  for (int i = min; i <= max; i++){
    data.push_back(Data<int, std::string>::create(i, std::to_string(i)));
  }
  auto rng = std::default_random_engine {};
  std::shuffle(data.begin(), data.end(), rng);
  return data;
}