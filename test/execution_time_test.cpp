//
// Created by 김태현 on 2022/05/27.
//

#include <filesystem>
#include <fstream>
#include <bp_tree.h>
#include "test_util.h"

#include <chrono>
using namespace std::chrono;

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace std;

int main() {

  try {
    auto logger = spdlog::basic_logger_mt("time_logger", "logs/time_log.log");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);
  }
  catch (const spdlog::spdlog_ex &ex) {
    std::cout << "Log init failed: " << ex.what() << std::endl;
  }

  spdlog::info("----------------------------------------------");
  spdlog::info("Initiate execution time test.");

  BPTree<int, std::string> bp_tree(30, 30);

  int min_key = 0;
  int max_key = 100000;

  spdlog::info("Data Count: " + to_string(max_key - min_key));

  auto data = make_population_string(min_key, max_key);

  execute_and_measure("Insertion", TimeType::Milliseconds, [&]() {
    for (auto &d : data) {
      bp_tree.insert(std::move(d));
    }
  });


  int search_target = make_random_int(min_key, max_key);
  spdlog::info("Search target: " + to_string(search_target));
  bool is_found = false;
  execute_and_measure("Search", TimeType::Microseconds, [&]() {
    is_found = bp_tree.search(search_target).isOk();
  });
  auto s = is_found ? "true" : "false";
  spdlog::info("Target is found: " + std::string(s));

  int search_begin = make_random_int(min_key, max_key - 3000);
  int search_end = search_begin + 3000;
  bool are_all_found = false;
  execute_and_measure("Range search", TimeType::Microseconds, [&](){
    auto result = bp_tree.search(search_begin, search_end).unwrapOr(std::vector<std::string>());
    if (result.size() == search_begin - search_end){
      are_all_found = true;
    } else {
      are_all_found = false;
    }
  });
  auto t = is_found ? "true" : "false";
  spdlog::info("Targets all found: " + std::string(t));

  auto keys = make_keys(min_key, max_key);
  execute_and_measure("Deletion", TimeType::Milliseconds, [&](){
    for (auto &k: keys){
      bp_tree.remove(k);
    }
  });
}