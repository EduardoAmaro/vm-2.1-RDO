/*
 * DecoderSetup.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#ifndef DECODERSETUP_HH_
#define DECODERSETUP_HH_

#include <string>

#include "CodecType.hh"

using namespace std;

class DecoderSetup {
 private:
  int run(int, char**);
 public:
  /* shared options */
  string input_coded_bitstream;
  string output_directory;
  CodecType type;

  /* WaSP-only options */
  string wasp_kakadu_directory;

  /* MuLE-only options */
  string mule_config_file;
  string mule_directory;

  DecoderSetup(int argc, char** argv);
  virtual ~DecoderSetup();
};

#endif /* DECODERSETUP_HH_ */
