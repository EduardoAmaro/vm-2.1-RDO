/* sparsefilter.cpp */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#include "sparsefilter.hh"
#include "fastols.hh"
#include "bitdepth.hh"

#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdint>

using std::int32_t;

spfilter getGlobalSparseFilter(
    const unsigned short *original_image,
    const unsigned short *input_image,
    const int nr,
    const int nc,
    const int NNt,
    const int Ms,
    const double bias_term_value) {

    int MT = (NNt * 2 + 1) * (NNt * 2 + 1) + 1; /* number of regressors */

    int Npp = (nr - NNt * 2) * (nc - NNt * 2);

    double *AA = new double[Npp * MT]();
    double *Yd = new double[Npp]();

    for (int ii = 0; ii < Npp; ii++) {
        *(AA + ii + (NNt * 2 + 1) * (NNt * 2 + 1) * Npp) = bias_term_value;
    }

    int iiu = 0;

    double Q = ((double)(1 << BIT_DEPTH) - 1);

    for (int ir = NNt; ir < nr - NNt; ir++) {
        for (int ic = NNt; ic < nc - NNt; ic++) {
            int ai = 0;
            for (int dy = -NNt; dy <= NNt; dy++) {
                for (int dx = -NNt; dx <= NNt; dx++) {

                    int offset = ir + dy + nr * (ic + dx);

                    /* get the desired Yd*/
                    if (dy == 0 && dx == 0) {
                        *(Yd + iiu) +=
                            ((double) *(original_image + offset)) / Q;	// (pow(2, BIT_DEPTH) - 1);
                    }

                    /* get the regressors */
                    *(AA + iiu + ai * Npp) += 
                        ((double) *(input_image + offset)) / Q;	// (pow(2, BIT_DEPTH) - 1);

                    ai++;
                }
            }

            iiu++;
        }
    }

    int32_t *PredRegr0 = new int[MT]();
    double *PredTheta0 = new double[MT]();

    int Mtrue = FastOLS_new(
        &AA,
        &Yd,
        PredRegr0,
        PredTheta0,
        Ms,
        MT,
        MT,
        Npp);

    if (AA != nullptr) {
        delete[](AA);
    }
    if (Yd != nullptr) {
        delete[](Yd);
    }

    spfilter sparse_filter;

    for (int ii = 0; ii < MT; ii++) {
        sparse_filter.regressor_indexes.push_back(PredRegr0[ii]);
        sparse_filter.filter_coefficients.push_back(PredTheta0[ii]);
    }

    sparse_filter.Ms = Ms;
    sparse_filter.NNt = NNt;
    sparse_filter.bias_term_value = bias_term_value;

    return sparse_filter;
}

void quantize_and_reorder_spfilter(
    spfilter &sparse_filter) {

    std::vector<std::pair<int32_t, double>> sparsefilter_pair;

    for (int ij = 0; ij < sparse_filter.Ms; ij++) {
        std::pair<int32_t, double> tmp_sp;
        tmp_sp.first = sparse_filter.regressor_indexes.at(ij);
        tmp_sp.second = sparse_filter.filter_coefficients.at(ij);
        sparsefilter_pair.push_back(tmp_sp);
    }

    /*ascending sort based on regressor index 
    (e.g., integer between 1 and 50 since we have 50 regressors */
    sort(sparsefilter_pair.begin(),
        sparsefilter_pair.end());

    sparse_filter.quantized_filter_coefficients.clear();
    sparse_filter.regressor_indexes.clear();

    double Q = static_cast<double>(1 << BIT_DEPTH_SPARSE);

    for (int ii = 0; ii < sparse_filter.Ms; ii++) {
        sparse_filter.regressor_indexes.push_back(sparsefilter_pair.at(ii).first);
        sparse_filter.quantized_filter_coefficients.push_back(static_cast<int32_t>( floor(sparsefilter_pair.at(ii).second * Q + 0.5)));
    }

}

void dequantize_and_reorder_spfilter(
    spfilter &sparse_filter) {

    sparse_filter.filter_coefficients.clear();

    int MT = (sparse_filter.NNt * 2 + 1) * (sparse_filter.NNt * 2 + 1) + 1;

    for (int ii = 0; ii < MT; ii++) {
        sparse_filter.filter_coefficients.push_back(0.0);
    }

    double Q = static_cast<double>(1 << BIT_DEPTH_SPARSE);

    for (int ii = 0; ii < sparse_filter.Ms; ii++) {
        if (sparse_filter.regressor_indexes.at(ii) > 0) {

            double theta0 = 
                static_cast<double>(sparse_filter.quantized_filter_coefficients.at(ii));

            sparse_filter.filter_coefficients.at(sparse_filter.regressor_indexes.at(ii)) = theta0 / Q;

        }
    }
}

std::vector<double> applyGlobalSparseFilter(
    const unsigned short *input_image,
    const int nr,
    const int nc,
    const int Ms,
    const int NNt,
    const double bias_term_value,
    const std::vector<double> filter_coeffs) {

    //double *final_view = new double[nr * nc]();

    std::vector<double> final_view;

    double Q = ((double)(1 << BIT_DEPTH) - 1);

    for (int ii = 0; ii < nr * nc; ii++) {
        final_view.push_back(static_cast<double>(input_image[ii])/Q);
    }

    for (int rr = NNt; rr < nr - NNt; rr++) {
        for (int cc = NNt; cc < nc - NNt; cc++) {

            final_view.at(rr + cc * nr) = 0.0;

            int ee = 0;

            for (int dy = -NNt; dy <= NNt; dy++) {
                for (int dx = -NNt; dx <= NNt; dx++) {

                    double regr_value = 
                        static_cast<double>(input_image[rr + dy + (cc + dx) * nr])/Q;

                    final_view.at(rr + cc * nr) += filter_coeffs.at(ee)*regr_value;

                    ee++;
                }
            }

            /* bias term */
            final_view.at(rr + cc * nr) += 
                bias_term_value*filter_coeffs.at((2 * NNt + 1)* (2 * NNt + 1));

        }
    }

    for (int ii = 0; ii < nr * nc; ii++) {
        final_view.at(ii) = final_view.at(ii) * Q;
    }

    return final_view;

}