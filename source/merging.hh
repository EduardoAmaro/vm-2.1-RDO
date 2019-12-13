/* merging.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef MERGING_HH
#define MERGING_HH

#include "view.hh"

void init_warping_arrays(
    const int N,
    unsigned short **&warped_texture_views,
    unsigned short **&warped_depth_views,
    float **&DispTargs,
    const int nr,
    const int nc,
    const int ncomp);

void clean_warping_arrays(
    const int N,
    unsigned short **warped_texture_views,
    unsigned short **warped_depth_views,
    float **DispTargs);

void setBMask(view *view0);

void initSegVp(
    view *view0, 
    float **DispTargs);

void initViewW(
    view *view0, 
    float **DispTargs);

void mergeMedian_N(
    unsigned short **warpedColorViews, 
    float **DispTargs,  
    view *view0, 
    const int ncomponents);

void mergeWarped_N_icomp(
    unsigned short **warpedColorViews,
    view *view0,
    const int icomp);

void getViewMergingLSWeights_icomp(
    view *view0,
    unsigned short **warpedColorViews,
    const unsigned short *original_color_view,
    const int icomp);

void getGeomWeight_icomp(
    view *view0,
    view *LF,
    const int icomp);


#endif
