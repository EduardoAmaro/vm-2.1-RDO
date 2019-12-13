/* predictdepth.cpp */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#include "predictdepth.hh"
#include "warping.hh"
#include "medianfilter.hh"
#include "ppm.hh"
#include "inpainting.hh"
#include "merging.hh"

#include <ctime>
#include <vector>

void WaSP_predict_depth(view* SAI, view *LF) {
  /* forward warp depth */
  if (SAI->n_depth_references > 0) {

    unsigned short **warped_texture_views_0_N = 
        new unsigned short*[SAI->n_depth_references]();
    unsigned short **warped_depth_views_0_N = 
        new unsigned short*[SAI->n_depth_references]();
    float **DispTargs_0_N = 
        new float*[SAI->n_depth_references]();

    init_warping_arrays(
        SAI->n_depth_references,
        warped_texture_views_0_N,
        warped_depth_views_0_N,
        DispTargs_0_N,
        SAI->nr,
        SAI->nc,
        SAI->ncomp);

    for (int ij = 0; ij < SAI->n_depth_references; ij++) {
      view *ref_view = LF + SAI->depth_references[ij];

      int tmp_w, tmp_r, tmp_ncomp;

      aux_read16PGMPPM(
          ref_view->path_out_pgm, 
          tmp_w, 
          tmp_r, 
          tmp_ncomp,
          ref_view->depth);

      ref_view->color = new unsigned short[ref_view->nr*ref_view->nc*3]();

      //aux_read16PGMPPM(ref_view->path_out_ppm, tmp_w, tmp_r, tmp_ncomp,
      //                 ref_view->color);

      warpView0_to_View1(
          ref_view, 
          SAI, 
          warped_texture_views_0_N[ij],
          warped_depth_views_0_N[ij],
          DispTargs_0_N[ij]);

      delete[] (ref_view->depth);
      delete[] (ref_view->color);

      ref_view->depth = nullptr;
      ref_view->color = nullptr;
    }

    /* merge depth using median*/

    int startt = clock();

    double *hole_mask = new double[SAI->nr*SAI->nc]();

//#pragma omp parallel for
    for (int ij = 0; ij < SAI->nr * SAI->nc; ij++) {

        hole_mask[ij] = INIT_DISPARITY_VALUE;

        std::vector<unsigned short> depth_values;
        for (int uu = 0; uu < SAI->n_depth_references; uu++) {
            unsigned short *pp = warped_depth_views_0_N[uu];
            float *pf = DispTargs_0_N[uu];
            if (*(pf + ij) > INIT_DISPARITY_VALUE) {
                depth_values.push_back(*(pp + ij));
            }
        }
        if (depth_values.size() > 0) {
            SAI->depth[ij] = getMedian(depth_values);
            hole_mask[ij] = 1.0f;
        }
    }

    uint32_t nholes = holefilling(
        SAI->depth, 
        SAI->nr, 
        SAI->nc, 
        INIT_DISPARITY_VALUE, 
        hole_mask);

    delete[](hole_mask);

    clean_warping_arrays(
        SAI->n_depth_references,
        warped_texture_views_0_N,
        warped_depth_views_0_N,
        DispTargs_0_N);

  }

}
