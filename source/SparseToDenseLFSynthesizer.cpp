/*
 * SparseToDenseLFSynthesizer.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <algorithm>
#include <functional>
#include <cinttypes>
#include <regex>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <experimental/filesystem>

#include "SparseToDenseLFSynthesizer.hh"
#include "IOManager.hh"
#include "ppm.hh"
#include "exceptions.hh"
#include "thirdpartlibs/json.hh"

using namespace std;
namespace fs = std::experimental::filesystem;
using json = nlohmann::json;

SparseToDenseLFSynthesizer::SparseToDenseLFSynthesizer(string from_sparse,
                                                       string to_dense,
                                                       string view_shift_file) {
  origin = from_sparse;
  destinity = to_dense;
  conversion_rules = load_conversion_rules(view_shift_file);
}

void SparseToDenseLFSynthesizer::run() {
  shift_dataset();
  fill_holes();
}

json SparseToDenseLFSynthesizer::load_conversion_rules(string path) {
  ifstream ifs(path);
  return json::parse(ifs);
}

void SparseToDenseLFSynthesizer::shift_dataset() {
  vector < json::object_t > levels = conversion_rules["levels"]
      .get<vector<json::object_t>>();
  for (auto level : levels)
    shift_level(level);
}

void SparseToDenseLFSynthesizer::shift_level(json single_level_json) {
  int level = single_level_json["level"].get<int>();
  vector < json::object_t > pairs = single_level_json["pairs"]
      .get<vector<json::object_t>>();
  for (auto pair : pairs)
    shift_view(level, pair);
}

void SparseToDenseLFSynthesizer::shift_view(int level, json pair) {
  string original = pair["original"].get<string>();
  string shifted = pair["shifted"].get<string>();
  string from = origin + "/" + to_string(level) + "/" + original + ".pgm";
  string to = destinity + "/" + to_string(level) + "/" + shifted + ".pgm";
  if (IOManager::exists(from))
    IOManager::copy(from, to);
}

void SparseToDenseLFSynthesizer::fill_holes() {
  vector < json::object_t > levels = conversion_rules["levels"]
      .get<vector<json::object_t>>();
  for (auto level : levels)
    fill_holes_in_level(level);
}

void SparseToDenseLFSynthesizer::fill_holes_in_level(json single_level_json) {
  int level = single_level_json["level"].get<int>();
  vector < json::object_t > pairs = single_level_json["pairs"]
      .get<vector<json::object_t>>();
  for (auto pair : pairs)
    fill_single_hole_if_necessary(level, pair);
}

void SparseToDenseLFSynthesizer::fill_single_hole_if_necessary(int level,
                                                               json pair) {
  string shifted = pair["shifted"].get<string>();
  string to = destinity + "/" + to_string(level) + "/" + shifted + ".pgm";
  if (!IOManager::exists(to))
    synthesize(to);
}

void SparseToDenseLFSynthesizer::synthesize(string view_fullpath) {
  vector < string > neighbors = get_neighbors(view_fullpath);
  make_average(neighbors, view_fullpath);
}

// TODO: CLean this shitty function
vector<string> SparseToDenseLFSynthesizer::get_neighbors(string view_fullpath) {
  fs::path full_path(view_fullpath);
  string stem = full_path.stem().string();
  string dir = full_path.parent_path().string();
  regex re("(\\d+)_(\\d+)");
  smatch sm;
  regex_match(stem, sm, re);
  int x = stoi(sm[1]);
  int y = stoi(sm[2]);
  vector<pair<int, int>> neigbors_search_indexes = { { x - 1, y }, { x + 1, y },
      { x, y - 1 }, { x, y + 1 }, { x - 1, y - 1 }, { x - 1, y + 1 }, { x + 1, y
          + 1 }, { x + 1, y - 1 } };
  vector < string > search_neighbors_filenames;
  transform(
      neigbors_search_indexes.begin(), neigbors_search_indexes.end(),
      back_inserter(search_neighbors_filenames), [dir](pair<int, int> p) {
        char buffer[1024];
        sprintf(buffer, "%s/%03d_%03d.pgm", dir.c_str(), p.first, p.second);
        string buffered(buffer);
        return buffered;
      });
  vector < string > valid_neighbors;
  copy_if(search_neighbors_filenames.begin(), search_neighbors_filenames.end(),
          back_inserter(valid_neighbors), [](string neighbor_path) {
            return IOManager::exists(neighbor_path);
          });
  return valid_neighbors;
}

// TODO: Can it be improved?
void SparseToDenseLFSynthesizer::make_average(vector<string> neighbor_paths,
                                              string output_path) {
  int ppm_columns, ppm_rows, number_of_channels;
  vector<unsigned short*> pgm_pointers;
  transform(
      neighbor_paths.begin(),
      neighbor_paths.end(),
      back_inserter(pgm_pointers),
      [&ppm_columns, &ppm_rows, &number_of_channels](string neighbor_path) {
        unsigned short* ptr;
        aux_read16PGMPPM(neighbor_path.c_str(), ppm_columns, ppm_rows, number_of_channels, ptr);
        return ptr;
      });
  unsigned short *synthesized_image = new unsigned short[ppm_columns * ppm_rows
      * number_of_channels]();
  for (unsigned int i = 0; i < ppm_columns * ppm_rows; ++i) {
    unsigned short value = 0;
    for (unsigned short* ptr : pgm_pointers)
      value = value + ptr[i];
    synthesized_image[i] = value / pgm_pointers.size();
  }
  aux_write16PGMPPM(output_path.c_str(), ppm_columns, ppm_rows,
                    number_of_channels, synthesized_image);
  delete[] (synthesized_image);
}

SparseToDenseLFSynthesizer::~SparseToDenseLFSynthesizer() {

}

