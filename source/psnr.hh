/* psnr.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef PSNR_HH
#define PSNR_HH

#include <cstdio>

double PSNR(
    unsigned short *im0, 
    unsigned short* im1, 
    const int NR,
    const int NC, 
    const int NCOMP, 
    double maxval);

double PSNR(
    unsigned short *im0, 
    unsigned short* im1,
    const int NR,
    const int NC, 
    const int NCOMP);

double getYCbCr_444_PSNR(
    unsigned short *im0,
    unsigned short* im1,
    const int NR,
    const int NC,
    const int NCOMP,
    const int N);

double getYCbCr_422_PSNR(
    unsigned short *im0, 
    unsigned short* im1, 
    const int NR,
    const int NC, 
    const int NCOMP, 
    const int N);

#endif
