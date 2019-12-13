/*
 * KakaduEncoderWrapper.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>
#include <utility>

using namespace std;

#ifndef KAKADUENCODERWRAPPER_HH_
#define KAKADUENCODERWRAPPER_HH_

class KakaduEncoderWrapper {
 public:
  KakaduEncoderWrapper(string source_LF_dir_path, string coded_LF_file_path,
                       string kakadu_path, double kakadu_parameter);
  virtual ~KakaduEncoderWrapper();
  void run();
  void encode_one_view(const string filename, const float jp2rate);

 private:
  string input_LF_directory;
  string compressed_file;
  char kakadu_encoder_path[1024];
  char kakadu_decoder_path[1024];
  double kakadu_parameter;

  void pack_directory_into_file(string directory, string output_file);
};

#endif /* KAKADUENCODERWRAPPER_HH_ */
