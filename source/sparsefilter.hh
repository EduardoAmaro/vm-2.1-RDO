/* sparsefilter.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef SPARSEFILTER_HH
#define SPARSEFILTER_HH

#include <vector>
#include <cstdint>

using std::int32_t;

#define SPARSE_BIAS_TERM 0.5

struct spfilter {

    std::vector<double> filter_coefficients;
    std::vector<int32_t> regressor_indexes;

    std::vector<int32_t> quantized_filter_coefficients;

    int32_t Ms;
    int32_t NNt;
    int32_t coeff_bit_precision;

    double bias_term_value;

};

void quantize_and_reorder_spfilter(
    spfilter &sparse_filter);

void dequantize_and_reorder_spfilter(
    spfilter &sparse_filter);

spfilter getGlobalSparseFilter(
    const unsigned short *original_image,
    const unsigned short *input_image,
    const int nr,
    const int nc,
    const int NNt,
    const int Ms,
    const double bias_term_value);

std::vector<double> applyGlobalSparseFilter(
    const unsigned short *input_image,
    const int nr,
    const int nc,
    const int Ms,
    const int NNt,
    const double bias_term_value,
    const std::vector<double> filter_coeffs);

#endif
