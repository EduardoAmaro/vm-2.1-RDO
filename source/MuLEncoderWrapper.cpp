/*
 * MuLEncoderWrapper.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <regex>
#include <functional>
#include <cinttypes>
#include "thirdpartlibs/json.hh"

#include "MuLEncoderWrapper.hh"
#include "IOManager.hh"
#include "exceptions.hh"
#include "ppm.hh"
#include "fileaux.hh"

using namespace std;
namespace fs = std::experimental::filesystem;
using json = nlohmann::json;

MuLEncoderWrapper::~MuLEncoderWrapper() {
}

// TODO: clean this function. it is so long
void MuLEncoderWrapper::run() {
  ifstream ifs(this->mule_config_path);
  json conf = json::parse(ifs);

  // TODO: Notice that you are reading the conf twice, you already read it in CoderSetup
  // TODO: It can be cleaner of parsed once at dedicated class
  map<string, string> parameter_value;
  parameter_value["lf"] = input_LF_directory;
  parameter_value["o"] = compressed_file;
  parameter_value["nv"] = to_string((int) conf["number_of_rows"].get<int>());
  parameter_value["nh"] = to_string((int) conf["number_of_columns"].get<int>());
  parameter_value["off_h"] = to_string((int) conf["offset"]["horizontal"].get<int>());
  parameter_value["off_v"] = to_string((int) conf["offset"]["vertical"].get<int>());
  parameter_value["v"] = to_string((int) conf["transform_size"]["maximum"]["intra-view"]["vertical"].get<int>());
  parameter_value["u"] = to_string((int) conf["transform_size"]["maximum"]["intra-view"]["horizontal"].get<int>());
  parameter_value["t"] = to_string((int) conf["transform_size"]["maximum"]["inter-view"]["vertical"].get<int>());
  parameter_value["s"] = to_string((int) conf["transform_size"]["maximum"]["inter-view"]["horizontal"].get<int>());
  parameter_value["min_v"] = to_string((int) conf["transform_size"]["minimum"]["intra-view"]["vertical"].get<int>());
  parameter_value["min_u"] = to_string((int) conf["transform_size"]["minimum"]["intra-view"]["horizontal"].get<int>());
  parameter_value["min_t"] = to_string((int) conf["transform_size"]["minimum"]["inter-view"]["vertical"].get<int>());
  parameter_value["min_s"] = to_string((int) conf["transform_size"]["minimum"]["inter-view"]["horizontal"].get<int>());
  parameter_value["lambda"] = to_string((float) conf["lambda"].get<float>());
  if (conf.find("lenslet13x13") != conf.end())
    if (conf["lenslet13x13"].get<bool>())
      parameter_value["lenslet13x13"] = "";

  stringstream command;
  command << mule_encoder_path;
  for (auto const& p : parameter_value)
    command << " -" << p.first << " " << p.second;

  IOManager::ensure_directory(this->compressed_file);
  system_1(const_cast<char*>(command.str().c_str()));
}

MuLEncoderWrapper::MuLEncoderWrapper(string source_LF_dir_path,
                                     string coded_LF_file_path,
                                     string mule_path, string config_file) {
  this->input_LF_directory = source_LF_dir_path;
  this->compressed_file = coded_LF_file_path;
  this->mule_encoder_path = mule_path + "/mule-encoder-bin";
  this->mule_decoder_path = mule_path + "/mule-decoder-bin";
  this->mule_config_path = config_file;
}
