#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdint>
#include <ctime>
#include <algorithm>

#include "Decoder.hh"
#include "bitdepth.hh"
#include "merging.hh"
#include "minconf.hh"
#include "sparsefilter.hh"
#include "ycbcr.hh"
#include "view.hh"
#include "warping.hh"
#include "residualjp2.hh"
#include "ppm.hh"
#include "fileaux.hh"
#include "medianfilter.hh"
#include "psnr.hh"
#include "inpainting.hh"
#include "predictdepth.hh"
#include "iolog.hh"
#include "DecoderSetup.hh"

int main(int argc, char** argv) {
  DecoderSetup setup(argc, argv);
  Decoder d(setup);
  d.run();

  exit(0);
}
