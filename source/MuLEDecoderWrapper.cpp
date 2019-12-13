/*
 * MuLEDecoderWrapper.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <fstream>

#include "MuLEDecoderWrapper.hh"
#include "IOManager.hh"
#include "fileaux.hh"
#include "thirdpartlibs/json.hh"

using json = nlohmann::json;

MuLEDecoderWrapper::MuLEDecoderWrapper(string compressed_file,
                                       string uncompress_dir,
                                       string mule_path, string config_file) {
  this->output_LF_directory = uncompress_dir;
  this->compressed_file = compressed_file;
  this->config_file = config_file;
  this->mule_decoder_path = mule_path + "/mule-decoder-bin";
}

MuLEDecoderWrapper::~MuLEDecoderWrapper() {
  // TODO Auto-generated destructor stub
}

void MuLEDecoderWrapper::run() {
  ifstream ifs(this->config_file); //TODO: Use same name of encoder
    json conf = json::parse(ifs);

    // TODO: Notice that you are reading the conf twice, you already read it in CoderSetup
    // TODO: It can be cleaner of parsed once at dedicated class
    map<string, string> parameter_value;
    parameter_value["lf"] = output_LF_directory;
    parameter_value["i"] = compressed_file;
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
    command << mule_decoder_path;
    for (auto const& p : parameter_value)
      command << " -" << p.first << " " << p.second;

    IOManager::mkdir(output_LF_directory);
    system_1(const_cast<char*>(command.str().c_str()));
}
