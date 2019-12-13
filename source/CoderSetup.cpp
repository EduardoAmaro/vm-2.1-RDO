/*
 * CoderSetup.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>
#include <iostream>
#include <list>

#include "CodecType.hh"
#include "CoderSetup.hh"
#include "thirdpartlibs/CLI11.hpp"
#include "thirdpartlibs/contains.hpp"
#include "thirdpartlibs/json.hh"
#include "exceptions.hh"

using namespace std;
using json = nlohmann::json;

CoderSetup::CoderSetup(int argc, char** argv) {
  int test = run(argc, argv);
  if (test == 0)
    parse();
  else
    throw MissingRequiredInputException();
}

// TODO: This function is clearly replaced by polymorphic classes on definition
// of WaSPCoderSetuop and MuLECOderSetup
void CoderSetup::parse() {
  ifstream ifs(config_file);
  json conf = json::parse(ifs);
  string input_type = conf["type"].get<string>();
  if (input_type == "WaSP" || input_type == "wasp" || input_type == "WASP")
    type = CodecType::WaSP;
  else if (input_type == "MuLE" || input_type == "mule" || input_type == "MULE")
    type = CodecType::MuLE;
}

int CoderSetup::run(int argc, char** argv) {
  CLI::App app { "JPL-VM-20 Encoder" };

  app.add_option(
      "-i,--input",
      input_directory,
      "Input directory containing a set of uncompressed light-field images (xxx_yyy.ppm).")
      ->required();

  app.add_option(
      "-o,--output",
      output_directory,
      "Output directory containing temporary light-field data and the compressed bitstream.")
      ->required();

  // TODO: To be removed. Just kept to maintain CLI compatibility with previous versions
  string t;
  app.add_option("-t,--type", t,
                 "Path to config file (.conf for WaSP or .cfg for MuLE)");

  app.add_option("-c,--config", config_file,
                 "Path to config file (.conf for WaSP or .cfg for MuLE)")
      ->required();

  app.add_option(
      "-k,--kakadu", wasp_kakadu_directory,
      "Directory containing the Kakadu codec (kdu_compress and kdu_expand).");

  app.add_option("-m,--mule", mule_directory,
                 "Directory containing the MuLE codec (root directory).");

  CLI11_PARSE(app, argc, argv);
  return 0;
}

CoderSetup::~CoderSetup() {
  // TODO Auto-generated destructor stub
}

