/*
 * MuLEDecoder.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include "MuLEDecoder.hh"
#include "MuLEDecoderWrapper.hh"
#include "DecoderSetup.hh"

MuLEDecoder::MuLEDecoder(DecoderSetup decoderSetup)
    : setup(decoderSetup) {
  // TODO Auto-generated constructor stub

}

MuLEDecoder::~MuLEDecoder() {
  // TODO Auto-generated destructor stub
}

void MuLEDecoder::run(void) {
  MuLEDecoderWrapper wrapper(setup.input_coded_bitstream,
                             setup.output_directory, setup.mule_directory,
                             setup.mule_config_file);
  wrapper.run();
}
