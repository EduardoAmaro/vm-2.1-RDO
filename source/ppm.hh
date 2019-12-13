/* ppm.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef PPM_HH
#define PPM_HH

bool aux_read16PGMPPM(
    const char* filename, 
    int &width, 
    int &height, 
    int &ncomp,
    unsigned short *&img);

bool aux_write16PGMPPM(
    const char* filename, 
    const int width, 
    const int height,
    const int ncomp, 
    const unsigned short *img);

#endif
