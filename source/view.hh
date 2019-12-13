/* view.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef VIEW_HH
#define VIEW_HH

#include "sparsefilter.hh"

#include <stdint.h>
#include <string>
#include <vector>

#define MEDFILT_DEPTH false

struct view {

  unsigned short *color;
  unsigned short *depth;
  unsigned short *residual_image;

  unsigned short *segmentation;

  unsigned char mmode; /* view merging mode, 0=LS, 1=geometric weight, 2=median*/

  int r, c;  // SAI subscript

  int nr, nc;  // image height, width

  int ncomp; //number of components

  float y, x;  // camera displacement

  int min_inv_d;  // needed only if inverse depth has negative values, [0,max]-mind = [-mind,max-mind]

  int n_references, n_depth_references;

  int *references, *depth_references; /* depth references not necessarily the same as color references
   we can have, for example, depth warping only from the externally obtained depth but we still
   warp color from neighbors that don't have depth provided. We don't want to propagate depth errors from
   badly warped depth views, thus we restrict depth warping to some high quality subset (usually meaning the
   externally obtained high quality depth maps)*/

  signed short *merge_weights;
  int32_t *sparse_weights;

  unsigned char *sparse_mask;

  float *merge_weights_float;

  double *merge_weights_double;

  int *number_of_pixels_per_region;

  bool *bmask; /* view mask for merging weights */
  unsigned short *seg_vp; /* class segmentation, used for view merging weights */
  int NB;

  bool cweight_search;

  float residual_rate_color;
  float residual_rate_depth;

  float stdd;

  int NNt, Ms;  //for global sparse, NNt defines the neighborhood size [ -NNt:NNt,-NNt:NNt ], Ms is the filter order

  int has_segmentation;
  int maxL;  // number of regions in segmentation

  int ****region_displacements; /* region displacement vectors [iar][iac][iR][xy], e.g., [13][13][25][2], for 13x13 angular views with 25 regions for segmentation */

  char path_input_pgm[1024], path_input_ppm[1024], path_input_seg[1024];
  char path_out_pgm[1024], path_out_ppm[1024];

  //char path_input_Y_pgm[1024], path_out_Y_pgm[1024];
  //char path_input_Cb_pgm[1024], path_out_Cb_pgm[1024];
  //char path_input_Cr_pgm[1024], path_out_Cr_pgm[1024];

  float *DM_ROW, *DM_COL; /* for lenslet with region displacement vectors */

  int i_order; /* view position in encoding configuration */

  bool use_median;  //use median merging or not

  bool yuv_transform;

  bool has_color_residual, has_depth_residual, use_global_sparse;
  bool has_color_references, has_depth_references;
  //bool has_min_inv_depth;

  bool has_x_displacement, has_y_displacement;

  bool has_chrominance;

  int level;  // hierarchical level

  std::string colorspace;

  float yuv_rates[3];
  float rgb_rate;

  bool depth_file_exist;

  char ppm_residual_path[1024];
  char jp2_residual_path_jp2[1024];

  char pgm_residual_Y_path[1024];
  char jp2_residual_Y_path_jp2[1024];
  char pgm_residual_Cb_path[1024];
  char jp2_residual_Cb_path_jp2[1024];
  char pgm_residual_Cr_path[1024];
  char jp2_residual_Cr_path_jp2[1024];

  char pgm_residual_depth_path[1024];
  char jp2_residual_depth_path_jp2[1024];

  char path_raw_texture_residual_at_encoder_ppm[1024];
  char path_raw_prediction_at_encoder_ppm[1024];
  char path_raw_texture_residual_at_decoder_ppm[1024];

  char path_internal_colorspace_out_ppm[1024];

  char inverse_depth_raw_pgm[1024];

  std::vector<spfilter> sparse_filters;

};

void initView(view* view);

unsigned short *read_input_ppm(
    const char *input_ppm_path,
    int &nr,
    int &nc,
    int &ncomp,
    const int bpc,
    const std::string colorspace);

void write_output_ppm(
    const unsigned short *texture_view_in_encoder_colorspace,
    const char *output_ppm_path,
    const int nr,
    const int nc,
    const int ncomp,
    const int bpc,
    const std::string colorspace);

void setPaths(
    view *SAI,
    const char *input_dir,
    const char *output_dir);

int get_highest_level(
    view *LF,
    const int n_views_total);

int* determine_levels(
    view *LF,
    const int n_views_total);

#endif
