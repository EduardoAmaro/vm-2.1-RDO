#include <cstdio>
#include <cstdint>
#include <ctime>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "bitdepth.hh"
#include "CoderSetup.hh"
#include "Encoder.hh"
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

int main(int argc, char** argv) {
  CoderSetup setup(argc, argv);
  Encoder e(setup);
  e.run();

  exit(0);
}
