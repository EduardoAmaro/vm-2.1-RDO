/*
 * Decoder.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#ifndef DECODER_HH_
#define DECODER_HH_

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdint>
#include <ctime>
#include <cstring>
#include <cmath>
#include <experimental/filesystem>


#include "merging.hh"
#include "minconf.hh"
#include "sparsefilter.hh"
#include "ycbcr.hh"
#include "view.hh"
#include "warping.hh"

#include "ppm.hh"
#include "fileaux.hh"
#include "medianfilter.hh"
#include "psnr.hh"
#include "inpainting.hh"
#include "predictdepth.hh"
#include "iolog.hh"
#include "DecoderSetup.hh"

namespace fs = std::experimental::filesystem;

#define SAVE_PARTIAL_WARPED_VIEWS false

class Decoder {
 private:
  DecoderSetup setup;
  void runWaspDecoder();
  void runMuleDecoder();

 public:
  Decoder(DecoderSetup);
  void run(void);
  virtual ~Decoder();
};

#endif /* DECODER_HH_ */
