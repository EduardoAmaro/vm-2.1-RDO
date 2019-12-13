/*
 * MuLEDecoderWrapper.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>

using namespace std;

#ifndef MULEDECODERWRAPPER_HH_
#define MULEDECODERWRAPPER_HH_

class MuLEDecoderWrapper {
 public:
  MuLEDecoderWrapper(string coded, string uncompress_dir, string mule_path,
                     string config_file);
  virtual ~MuLEDecoderWrapper();
  void run();

 private:
  string output_LF_directory;
  string compressed_file;
  string mule_decoder_path;
  string config_file;
};

#endif /* MULEDECODERWRAPPER_HH_ */
