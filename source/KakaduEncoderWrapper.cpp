/*
 * KakaduEncoderWrapper.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */
#include <experimental/filesystem>
#include <ctime>
#include <functional>
#include <algorithm>
#include <iostream>

#include "KakaduEncoderWrapper.hh"
#include "IOManager.hh"
#include "fileaux.hh"

namespace fs = std::experimental::filesystem;

KakaduEncoderWrapper::KakaduEncoderWrapper(string source_LF_dir_path,
                                           string coded_LF_file_path,
                                           string kakadu_path,
                                           double kakadu_parameter) {
  this->input_LF_directory = source_LF_dir_path;
  this->compressed_file = coded_LF_file_path;
  sprintf(this->kakadu_encoder_path, "%s/kdu_compress", kakadu_path.c_str());
  this->kakadu_parameter = kakadu_parameter;
}

KakaduEncoderWrapper::~KakaduEncoderWrapper() {
  // TODO Auto-generated destructor stub
}

void KakaduEncoderWrapper::encode_one_view(const string filename,
                                           const float jp2rate) {
  string destiny_dir = this->input_LF_directory + "/wasp/";
  fs::create_directories(destiny_dir);

  string input_file = this->input_LF_directory + "/" + filename + ".pgm";
  string ouput_file = destiny_dir + "/" + filename + ".jp2";

  char cmd[1024];
  sprintf(
      cmd,
      "%s -full -num_threads 0 -no_weights -no_info -precise -i %s -o %s -rate %04.3f",
      this->kakadu_encoder_path, input_file.c_str(), ouput_file.c_str(),
      jp2rate);
  system_1(cmd);
}

// TODO: Too big function. Clean it, please.
void KakaduEncoderWrapper::run() {
  string destiny_dir = this->input_LF_directory + "/wasp/";
  fs::create_directories(destiny_dir);
  auto files = IOManager::list_directory(this->input_LF_directory.c_str());
  for (auto file : files) {
    if (file.path().extension() == ".pgm") {
      string basename = file.path().stem().string();
      string input_file = this->input_LF_directory + "/" + basename + ".pgm";
      string ouput_file = destiny_dir + "/" + basename + ".jp2";
      char cmd[1024];
      sprintf(
          cmd,
          "%s -full -num_threads 0 -no_weights -no_info -precise -i %s -o %s -rate %04.3f",
          this->kakadu_encoder_path, input_file.c_str(), ouput_file.c_str(),
          this->kakadu_parameter);
      system_1(cmd);
    }
  }
  IOManager::ensure_directory(this->compressed_file);
  pack_directory_into_file(destiny_dir, this->compressed_file);
}

void KakaduEncoderWrapper::pack_directory_into_file(string directory,
                                                    string output_file) {
  char cmd[1024];
  sprintf(cmd, "%s -cvf %s -C %s", TARPATH, output_file.c_str(),
          directory.c_str());
  system_1(cmd);
}
