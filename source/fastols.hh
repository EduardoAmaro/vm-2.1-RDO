/* fastols.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef FASTOLS_HH
#define FASTOLS_HH

int FastOLS_new(
    double **AAA, 
    double **Ydd, 
    int *PredRegr0, 
    double *PredTheta0,
    const int Ms, 
    const int MT, 
    const int MPHI, 
    const int N);

#endif
