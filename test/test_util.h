//
// Created by 김태현 on 2022/05/23.
//

#include <vector>
#include <data.h>
#include <random>
#include <chrono>
using namespace std::chrono;
using namespace std;

#include "spdlog/spdlog.h"

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

enum class TimeType {
  Milliseconds,
  Microseconds,
  Nanoseconds,
};

template <typename TimeType>
struct ExecutionTime {
  ExecutionTime(TimeType time, std::string type_name): time(time), type_name(std::move(type_name)){}
  TimeType time;
  std::string type_name;
};

template <typename Functor>
ExecutionTime<milliseconds> execute_and_measure_milliseconds(Functor functor){

  auto start = high_resolution_clock::now();
  functor();
  auto end = high_resolution_clock::now();
  return ExecutionTime{duration_cast<milliseconds>(end - start), "milliseconds"};
}

template <typename Functor>
ExecutionTime<microseconds> execute_and_measure_microseconds(Functor functor){

  auto start = high_resolution_clock::now();
  functor();
  auto end = high_resolution_clock::now();
  return ExecutionTime{duration_cast<microseconds>(end - start), "microseconds"};
}

template <typename Functor>
ExecutionTime<nanoseconds> execute_and_measure_nanoseconds(Functor functor){

  auto start = high_resolution_clock::now();
  functor();
  auto end = high_resolution_clock::now();
  return ExecutionTime{duration_cast<nanoseconds>(end - start), "nanoseconds"};
}

template <typename Functor>
void execute_and_measure(const std::string& measuring_name, TimeType time_type, Functor functor){
  std::string log_str;
  if (time_type == TimeType::Milliseconds) {
    auto t = execute_and_measure_milliseconds(functor);
    log_str = measuring_name + " time: " + to_string(t.time.count()) + " " + t.type_name;
  } else if (time_type == TimeType::Microseconds) {
    auto t = execute_and_measure_microseconds(functor);
    log_str = measuring_name + " time: " + to_string(t.time.count()) + " " + t.type_name;
  } else if (time_type == TimeType::Nanoseconds) {
    auto t = execute_and_measure_nanoseconds(functor);
    log_str = measuring_name + " time: " + to_string(t.time.count()) + " " + t.type_name;
  }
  spdlog::info(log_str);
}

int make_random_int(int begin, int end){
  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_int_distribution<int> dis(begin, end - 1);
  return dis(g);
}