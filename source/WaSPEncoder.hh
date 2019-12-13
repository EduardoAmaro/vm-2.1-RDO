/*
 * WaSPEncoder.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>,
 *         Pekka Astola
 *         <pekka.astola@tuni.fi>
 */
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdint>
#include <ctime>
#include <cstring>
#include <cmath>

#include "bitdepth.hh"
#include "merging.hh"
#include "minconf.hh"
#include "sparsefilter.hh"
#include "ycbcr.hh"
#include "view.hh"
#include "warping.hh"
#include "residualjp2.hh"
#include "ppm.hh"
#include "medianfilter.hh"
#include "psnr.hh"
#include "inpainting.hh"
#include "predictdepth.hh"
#include "iolog.hh"

#include "CoderSetup.hh"

using namespace std;

#ifndef SOURCE_WASPENCODER_HH_
#define SOURCE_WASPENCODER_HH_

#define USE_difftest_ng false

#define STD_SEARCH_LOW 10
#define STD_SEARCH_HIGH 250
#define STD_SEARCH_STEP 10

#define SAVE_PARTIAL_WARPED_VIEWS false

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F        // max value
#endif

class WaSPEncoder {
 private:
  CoderSetup setup;
  int n_views_total = 0;

  int std_search_s;
  view* LF;
  view ***LF_mat;
  FILE *output_results_file;

  int nr, nc; /*number of rows in SAI, number of columns in SAI, i.e., V and U*/

  int MINIMUM_DEPTH = 0;

  int maxh; /*maximum hierarchy level*/

  bool STD_SEARCH = false;

  std::string colorspace_LF;

  /* to get efficiency from multiple JP2 files, we remove parts of the files
   which are repetitive over all files. For this we have a minimalistic
   dictionary method. */

  vector<vector<unsigned char>> JP2_dict;

  char path_out_LF_data[1024];

 protected:
  void load_config_json(string config_json_file);
  void write_config(string config_json_file_out); /*debug reasons*/

  void generate_WaSP_bitstream();

  void merge_texture_views(
      view *SAI,
      view *LF,
      unsigned short **warped_texture_views,
      float **DispTargs);
  
  void forward_warp_texture_references(
      view *LF,
      view *SAI,
      unsigned short **warped_texture_views,
      unsigned short **warped_depth_views,
      float **DispTargs);

  void generate_texture_residual_level_wise();

  void generate_inverse_depth_levelwise();

 public:
  WaSPEncoder(CoderSetup coderSetup);
  virtual ~WaSPEncoder();
  void run(void);
};

#endif /* SOURCE_WASPENCODER_HH_ */
