/* ycbcr.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef YCBCR_HH
#define YCBCR_HH

#define YUV_422 false

void RGB2YUV422(
    unsigned short *rgb, 
    unsigned short **yy, 
    unsigned short **cbb,
    unsigned short **crr, 
    const int NR,
    const int NC,
    const int NCOMP, 
    const int N);

void RGB2YCbCr(
    const unsigned short *rgb, 
    unsigned short *ycbcr, 
    const int nr,
    const int nc, 
    const int N);

void YCbCr2RGB(
    const unsigned short *ycbcr,
    unsigned short *rgb, 
    const int nr,
    const int nc, 
    const int N);

#endif
