/*
 * KakaduDecoderWrapper.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <string>
#include <utility>

using namespace std;

#ifndef KAKADUDECODERWRAPPER_HH_
#define KAKADUDECODERWRAPPER_HH_

class KakaduDecoderWrapper {
 public:
  KakaduDecoderWrapper(string coded, string uncompress_dir, string kakadu_path);
  virtual ~KakaduDecoderWrapper();
  void run();

 private:
  string output_LF_directory;
  string compressed_file;
  char kakadu_decoder_path[1024];

  void unpack_file_into_directory(string input_file, string output_directory);
  void unpack_individual_jpeg_files(string output_directory);
  void remove_temp_jp2_files(string output_directory);
};

#endif /* KAKADUDECODERWRAPPER_HH_ */
