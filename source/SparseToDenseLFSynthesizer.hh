/*
 * SparseToDenseLFSynthesizer.h
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>

#include "thirdpartlibs/json.hh"
#include "Matrix2D.hh"

using namespace std;
using pair16 = pair<uint16_t, uint16_t>;
using json = nlohmann::json;

#ifndef SPARSETODENSELFSYNTHESIZER_HH_
#define SPARSETODENSELFSYNTHESIZER_HH_

class SparseToDenseLFSynthesizer {
 private:
  string origin;
  string destinity;
  json conversion_rules;

  json load_conversion_rules(string path);

  // TODO: specialize this in a coherent entity "Shifter"
  void shift_dataset();
  void shift_level(json level);
  void shift_view(int level, json pair);

  // TODO: // TODO: specialize this in a coherent entity "Synthesizer"
  vector<string> get_neighbors(string view_fullpath);
  void fill_holes();
  void fill_holes_in_level(json level);
  void fill_single_hole_if_necessary(int level, json pair);
  void synthesize(string view_fullpath);
  void make_average(vector<string> neighbor_paths, string output_path);

 public:
  SparseToDenseLFSynthesizer(string from_sparse, string to_dense,
                             string view_shift_file);
  void run();
  virtual ~SparseToDenseLFSynthesizer();
};

#endif /* SPARSETODENSELFSYNTHESIZER_HH_ */
