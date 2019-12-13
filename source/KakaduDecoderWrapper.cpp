/*
 * KakaduDecoderWrapper.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <experimental/filesystem>
#include <iostream>

#include "KakaduDecoderWrapper.hh"
#include "IOManager.hh"
#include "fileaux.hh"

using namespace std;
namespace fs = std::experimental::filesystem;

KakaduDecoderWrapper::KakaduDecoderWrapper(string coded, string uncompress_dir,
                                           string kakadu_path) {
  this->output_LF_directory = uncompress_dir;
  this->compressed_file = coded;
  sprintf(this->kakadu_decoder_path, "%s/kdu_expand", kakadu_path.c_str());
}

KakaduDecoderWrapper::~KakaduDecoderWrapper() {
  // TODO Auto-generated destructor stub
}

void KakaduDecoderWrapper::run() {
  IOManager::ensure_directory(this->output_LF_directory);
  unpack_file_into_directory(this->compressed_file, this->output_LF_directory);
  unpack_individual_jpeg_files(this->output_LF_directory);
  remove_temp_jp2_files(this->output_LF_directory);
}

void KakaduDecoderWrapper::unpack_file_into_directory(string input_file,
                                                      string output_directory) {
  char cmd[1024];
  sprintf(cmd, "%s -xvf %s -C %s", TARPATH, input_file.c_str(),
          output_directory.c_str());
  IOManager::mkdir(output_directory);
  system_1(cmd);
}

void KakaduDecoderWrapper::unpack_individual_jpeg_files(
    string output_directory) {
  for (auto& file : IOManager::list_directory(output_directory)) {
    string stem = file.path().stem().string();
    char cmd[1024];
    sprintf(cmd, "%s -num_threads 0 -i %s/%s.jp2 -o %s/%s.pgm",
            kakadu_decoder_path, output_directory.c_str(), stem.c_str(),
            output_directory.c_str(), stem.c_str());
    system_1(cmd);
  }
}

void KakaduDecoderWrapper::remove_temp_jp2_files(string output_directory) {
  for (auto& file : IOManager::list_directory(output_directory))
    if (file.path().extension().string() == ".jp2")
      IOManager::rm(file.path().string());
}
