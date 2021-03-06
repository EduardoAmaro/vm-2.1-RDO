/* warping.cpp */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#include "warping.hh"
#include "bitdepth.hh"

#include <cstdlib>
#include <cmath>
#include <cstring>


void warpView0_to_View1(
    view *view0, 
    view *view1, 
    unsigned short *warpedColor,
    unsigned short *warpedDepth, 
    float *DispTarg) {

  /*this function forward warps from view0 to view1 for both color and depth*/

  float ddy = view0->y - view1->y;
  float ddx = view0->x - view1->x;

  unsigned short *AA1 = view0->color;
  unsigned short *DD1 = view0->depth;

  memset(warpedColor, 0, sizeof(unsigned short)*view0->nr*view0->nc * 3);
  memset(warpedDepth, 0, sizeof(unsigned short)*view0->nr*view0->nc);
  memset(DispTarg, 0, sizeof(float)*view0->nr*view0->nc);

  for (int ij = 0; ij < view0->nr * view0->nc; ij++) {
    DispTarg[ij] = INIT_DISPARITY_VALUE;
  }

  for (int ij = 0; ij < view0->nr * view0->nc; ij++) {

    float disp = ((float) DD1[ij] - (float) view0->min_inv_d)
        / (float) (1 << D_DEPTH);

    float DM_COL = disp * ddx;
    float DM_ROW = disp * ddy;

    int iy = ij % view0->nr;  //row
    int ix = (ij - iy) / view0->nr;  //col

    int ixnew = ix + (int)floor(DM_COL + 0.5);
    int iynew = iy + (int)floor(DM_ROW + 0.5);

    if (
        iynew >= 0 &&  
        ixnew >= 0 && 
        ixnew < view0->nc && 
        iynew < view0->nr) {

      int indnew = iynew + ixnew * view0->nr;

      if (DispTarg[indnew] < disp) {

        DispTarg[indnew] = disp;
        warpedDepth[indnew] = DD1[ij];

        for (int icomp = 0; icomp < view0->ncomp; icomp++) {

            warpedColor[indnew + view0->nr * view0->nc*icomp] = 
                AA1[ij+ view0->nr * view0->nc*icomp];

        }
      }
    }
  }
}
