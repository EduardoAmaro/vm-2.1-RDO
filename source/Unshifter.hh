/*
 * Unshifter.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>

#include "thirdpartlibs/json.hh"

using namespace std;
using json = nlohmann::json;

#ifndef UNSHIFTER_HH_
#define UNSHIFTER_HH_

class Unshifter {
 public:
  Unshifter(string from_sparse, string to_dense, string view_shift_file);
  void run();
  virtual ~Unshifter();

 private:
  string origin;
  string destinity;
  json conversion_rules;
  json load_conversion_rules(string path);
  void unshift_dataset();
  void unshift_level(json level);
  void unshift_view(int level, json pair);
};

#endif /* UNSHIFTER_HH_ */
