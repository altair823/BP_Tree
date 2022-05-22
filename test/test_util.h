//
// Created by 김태현 on 2022/05/23.
//

#include <vector>
#include <data.h>
#include <random>

std::vector<DataUnique<int, int>> make_population(int min, int max){
  std::vector<DataUnique<int, int>> data;
  for (int i = min; i < max; i++){
    data.push_back(Data::create(i, i));
  }
  auto rng = std::default_random_engine {};
  std::shuffle(data.begin(), data.end(), rng);
  return data;
}