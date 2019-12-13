/*
 * Decoder.cpp
 *
 *  Created on: 13 de set de 2018
 *      Author: pedrogf
 */

#include "Decoder.hh"

#include "CodecType.hh"
#include "MuLEDecoder.hh"
#include "WaSPDecoder.hh"

Decoder::Decoder(DecoderSetup decoderSetup)
    : setup(decoderSetup) {

}

void Decoder::run(void) {
  if (setup.type == CodecType::WaSP)
    runWaspDecoder();
  else if (setup.type == CodecType::MuLE)
    runMuleDecoder();
}

Decoder::~Decoder() {

}

void Decoder::runWaspDecoder() {
  WaSPDecoder decoder(setup);
  decoder.run();
}

void Decoder::runMuleDecoder() {
  MuLEDecoder decoder(setup);
  decoder.run();
}
