/*
 * MuLEncoderWrapper.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>
#include <utility>


using namespace std;

#ifndef MULENCODERWRAPPER_HH_
#define MULENCODERWRAPPER_HH_

class MuLEncoderWrapper {
 public:
  MuLEncoderWrapper(string source_LF_dir_path, string coded_LF_file_path,
                    string mule_path, string config_file);

  virtual ~MuLEncoderWrapper();
  void run();

 private:
  string input_LF_directory;
  string compressed_file;
  string mule_encoder_path;
  string mule_decoder_path;
  string mule_config_path;
};

#endif /* MULENCODERWRAPPER_HH_ */
