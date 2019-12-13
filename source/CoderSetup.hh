/*
 * CoderSetup.h
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */
#ifndef CODERSETUP_HH_
#define CODERSETUP_HH_

#include <string>
#include <vector>

#include "CodecType.hh"

using namespace std;

class CoderSetup {
 private:
  int run(int, char**);
  void parse();
 public:
  /* shared options */
  string input_directory;
  string output_directory;
  CodecType type;

  /* Only one config file is required */
  string config_file;

  /* WaSP-only options */
  string wasp_kakadu_directory;

  /* MuLE-only options */
  string mule_directory;

  CoderSetup(int, char**);
  virtual ~CoderSetup();
};

#endif /* CODERSETUP_HH_ */
