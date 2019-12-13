/*
 * DecoderSetup.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>

#include "CodecType.hh"
#include "DecoderSetup.hh"
#include "thirdpartlibs/CLI11.hpp"
#include "thirdpartlibs/split.h"
#include "exceptions.hh"

using namespace std;

DecoderSetup::DecoderSetup(int argc, char** argv) {
  run(argc, argv);
}

int DecoderSetup::run(int argc, char** argv) {
  CLI::App app { "JPL-VM-20 Decoder" };

  app.add_option("-i,--input", input_coded_bitstream,
                 "Input coded light-field bitstream (output.LF)")->required();

  app.add_option(
      "-o,--output", output_directory,
      "Output directory the decoded light-field in format xxx_yyy.ppm")
      ->required();

  app.add_set("-t,--type", type, { CodecType::MuLE, CodecType::WaSP },
              "Codec type")->type_name("enum/CodecType in {MuLE=0, WaSP=1}")
      ->required();

  /* TODO: TO BE REMOVED IN THE FUTURE */
  app.add_option(
      "-k,--kakadu", wasp_kakadu_directory,
      "Directory containing the Kakadu codec (kdu_compress and kdu_expand)");

  app.add_option("-m,--mule", mule_directory,
                 "Directory containing the MuLE codec");

  app.add_option("-c,--config", mule_config_file,
                 "Path to config file (.conf for WaSP or .cfg for MuLE)");

  //app.add_option("-s,--shift_config", view_shift_file, "Path to shift configuration file (.json)");

  CLI11_PARSE(app, argc, argv);
  return 0;
}

DecoderSetup::~DecoderSetup() {
  // TODO Auto-generated destructor stub
}

