/*
 * MuLEDecoder.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include "DecoderSetup.hh"

#ifndef SOURCE_MULEDECODER_HH_
#define SOURCE_MULEDECODER_HH_

class MuLEDecoder {
  private:
    DecoderSetup setup;
 public:
  MuLEDecoder(DecoderSetup setup);
  virtual ~MuLEDecoder();
  void run(void);
};

#endif /* SOURCE_MULEDECODER_HH_ */
