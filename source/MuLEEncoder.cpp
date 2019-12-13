/*
 * MuLEEncoder.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include "MuLEEncoder.hh"
#include "MuLEncoderWrapper.hh"

MuLEEncoder::MuLEEncoder(CoderSetup coderSetup)
    : setup(coderSetup) {
  // TODO Auto-generated constructor stub

}

MuLEEncoder::~MuLEEncoder() {
  // TODO Auto-generated destructor stub
}

void MuLEEncoder::run(void) {
  MuLEncoderWrapper wrapper(setup.input_directory,
                            setup.output_directory + "/output.LF",
                            setup.mule_directory, setup.config_file);
  wrapper.run();
}
