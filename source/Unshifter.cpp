/*
 * Unshifter.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

#include "Unshifter.hh"
#include "IOManager.hh"

using namespace std;
namespace fs = std::experimental::filesystem;
using json = nlohmann::json;

Unshifter::Unshifter(string from, string to, string view_shift_file) {
  origin = from;
  destinity = to;
  conversion_rules = load_conversion_rules(view_shift_file);
}

void Unshifter::run() {
  unshift_dataset();
}

// TODO: this function is duplicated at SparseToDenseLF. Please refactor it in order to keep only one implementation.
json Unshifter::load_conversion_rules(string path) {
  ifstream ifs(path);
  return json::parse(ifs);
}

Unshifter::~Unshifter() {
  // TODO Auto-generated destructor stub
}

void Unshifter::unshift_dataset() {
  vector < json::object_t > levels = conversion_rules["levels"]
      .get<vector<json::object_t>>();  // @suppress("Ambiguous problem")
  for (auto level : levels)
    unshift_level(level);
}

void Unshifter::unshift_level(json single_level_json) {
  int level = single_level_json["level"].get<int>();  // @suppress("Ambiguous problem")
  vector < json::object_t > pairs = single_level_json["pairs"]
      .get<vector<json::object_t>>();  // @suppress("Ambiguous problem")
  for (auto pair : pairs)
    unshift_view(level, pair);
}

void Unshifter::unshift_view(int level, json pair) {
  string original = pair["original"].get<string>();  // @suppress("Ambiguous problem")
  string shifted = pair["shifted"].get<string>();  // @suppress("Ambiguous problem")
  string from = origin + "/" + to_string(level) + "/" + shifted + ".pgm";
  string to = destinity + "/" + to_string(level) + "/" + original + ".pgm";
  if (IOManager::exists(from))
    IOManager::copy(from, to);
}
