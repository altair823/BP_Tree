//
// Created by 김태현 on 2022/05/23.
//

#include <vector>
#include <data.h>
#include <random>

std::vector<DataUnique<int, int>> make_population_int(int min, int max, bool is_random = true){
  std::vector<DataUnique<int, int>> data;
  for (int i = min; i <= max; i++){
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
  for (int i = min; i <= max; i++){
    data.push_back(Data<int, std::string>::create(i, std::to_string(i)));
  }
  if (is_random) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
  }
  return data;
}