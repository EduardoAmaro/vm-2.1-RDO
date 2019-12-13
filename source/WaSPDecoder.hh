/*
 * WaSPDecoder.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>,
 *         Pekka Astola
 *         <pekka.astola@tuni.fi>
 */

#include <vector>

#include "DecoderSetup.hh"
#include "view.hh"
#include "bitdepth.hh"


#ifndef SOURCE_WASPDECODER_HH_
#define SOURCE_WASPDECODER_HH_

class WaSPDecoder {
 private:
  DecoderSetup setup;

  int n_bytes_prediction = 0;
  int n_bytes_residual = 0;
  int number_of_views = 0;
  int number_of_rows = 0;
  int number_of_columns = 0;
  int maxh = 0; /*maximum hierarchy level*/

  std::string colorspace_LF;

  unsigned short minimum_depth = 0;

  bool use_color_transform = false;

  view* LF = nullptr;
  FILE* input_LF = nullptr;
  std::vector<std::vector<unsigned char>> JP2_dict;


  void dealloc();

 protected:
  void WaSP_decode_header();
  void WaSP_decode_views();

  void predict_texture_view(view* SAI);

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

  template<class T>
  T clip(T value, T min, T max) {
      value = value < min ? min : value;
      value = value > max ? max : value;
      return value;
  }

 public:
  WaSPDecoder(DecoderSetup decoderSetup);
  virtual ~WaSPDecoder();
  void run(void);
};

#endif /* SOURCE_WASPDECODER_HH_ */
