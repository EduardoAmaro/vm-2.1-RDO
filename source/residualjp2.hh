/* residualjp2.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef RESIDUALJP2_HH
#define RESIDUALJP2_HH

#include "view.hh"

void getJP2Header(
    unsigned char *JP2, 
    unsigned char *&header, 
    int JP2Size,
    int &headerSize);

int getJP2DictionaryIndex(
    unsigned char *JP2header, 
    int headerSize,
    std::vector<std::vector<unsigned char>> JP2_dict);

void readResidualFromDisk(
    const char *jp2_residual_path_jp2,
    int &n_bytes_residual,
    FILE *input_LF,
    std::vector<std::vector<unsigned char>> &JP2_dict);

void updateJP2Dictionary(
    std::vector<std::vector<unsigned char>> &JP2_dict,
    unsigned char *header,
    int headerSize);

void writeResidualToDisk(
    const char *jp2_residual_path_jp2,
    FILE *output_LF_file, 
    int &n_bytes_residual,
    std::vector<std::vector<unsigned char>> &JP2_dict);

char *kakadu_oparams(
    const double rate,
    const std::string colorspace);

char *kakadu_cparams(
    const double *cweights,
    const int ncomp);

int encodeKakadu(
    const char *ppm_pgm_input_path,
    const char *kdu_compress_path,
    const char *jp2_output_path,
    const char *encoding_parameters,
    const double rate);

int decodeKakadu(
    const char *ppm_pgm_output_path,
    const char *kdu_expand_path,
    const char *jp2_input_path);

double* get_residual(
    const unsigned short *original,
    const unsigned short *prediction,
    const int nr,
    const int nc,
    const int ncomp);

unsigned short* quantize_residual(
    const double *residual,
    const int nr,
    const int nc,
    const int ncomp,
    const int bpc,
    const int Q_i,
    const int offset_i);

double* dequantize_residual(
    const unsigned short *qresidual,
    const int nr,
    const int nc,
    const int ncomp,
    const int bpc,
    const int Q_i,
    const int offset_i);

unsigned short *apply_residual(
    const unsigned short *prediction,
    const double *residual,
    const int nr,
    const int nc,
    const int ncomp,
    const int bpc);

unsigned short* decode_residual_JP2(
    const char *ppm_pgm_output_path,
    const char *kdu_expand_path,
    const char *jp2_input_path);

void encode_residual_JP2(
    const char *ppm_pgm_input_path,
    const char *kdu_compress_path,
    const char *jp2_output_path,
    const char *encoding_parameters,
    const double rate);

#endif
