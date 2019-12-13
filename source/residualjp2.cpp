/* codestream.cpp */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include "IOManager.hh"
#include "residualjp2.hh"
#include "ycbcr.hh"
#include "ppm.hh"
#include "fileaux.hh"
#include "clip.hh"
#include "medianfilter.hh"
#include "iolog.hh"

#define USE_JP2_DICTIONARY 1

/*
 * TODO:PLEASE, modularize this module. A single function doing many things.
 * https://herbertograca.com/2016/09/03/clean-code-3-functions-by-robert-c-martin-uncle-bob/
 */

void getJP2Header(
    unsigned char *JP2, 
    unsigned char *&header, 
    int JP2Size,
    int &headerSize) {

  for (int ii = 0; ii < JP2Size - 1; ii++) {
    if ((unsigned short) (JP2[ii] << 8 | JP2[ii + 1]) == 0xFF90) { /*we have first tile*/
      headerSize = ii + 2; /*there was a bug*/
      header = new unsigned char[headerSize];
      memcpy(header, JP2, headerSize);
      return;
    }
  }
  return;
}

int getJP2DictionaryIndex(
    unsigned char *header, 
    int headerSize,
    std::vector<std::vector<unsigned char>> JP2_dict) {

  for (int ii = 0; ii < JP2_dict.size(); ii++) {

    int L = (int) JP2_dict.at(ii).size();

    if (L == headerSize) {
      if (memcmp(header, &JP2_dict.at(ii)[0], L) == 0) {
        return ii;
      }
    }

  }

  return -1;
}

void updateJP2Dictionary(
    std::vector<std::vector<unsigned char>> &JP2_dict,
    unsigned char *JP2header, 
    int headerSize) {

  std::vector<unsigned char> new_dict_element;

  for (int ii = 0; ii < headerSize; ii++) {
    new_dict_element.push_back(JP2header[ii]);
  }

  JP2_dict.push_back(new_dict_element);

}

void readResidualFromDisk(
    const char *jp2_residual_path_jp2,
    int &n_bytes_residual, 
    FILE *input_LF,
    std::vector<std::vector<unsigned char>> &JP2_dict) {

  int n_bytes_JP2 = 0;
  unsigned char *jp2_residual = 0;

  if (USE_JP2_DICTIONARY) {

    int dict_index = 0, headerSize = 0;
    unsigned char dict_index_char = 0;

    n_bytes_residual += (int) fread(&dict_index_char, sizeof(unsigned char), 1,
                                    input_LF) * sizeof(unsigned char);

    dict_index = (int) dict_index_char;

    if (JP2_dict.size() == 0 || dict_index > (int) JP2_dict.size() - 1) {

      n_bytes_residual += (int) fread(&headerSize, sizeof(int), 1, input_LF)
          * sizeof(int);

      unsigned char *JP2header = new unsigned char[headerSize]();

      n_bytes_residual += (int) fread(JP2header, sizeof(unsigned char),
                                      headerSize, input_LF)
          * sizeof(unsigned char);

      updateJP2Dictionary(JP2_dict, JP2header, headerSize);

      delete[] (JP2header);

    }

    unsigned char *jp2_residual_tmp;

    n_bytes_residual += (int) fread(&n_bytes_JP2, sizeof(int), 1, input_LF)
        * sizeof(int);
    jp2_residual_tmp = new unsigned char[n_bytes_JP2]();
    n_bytes_residual += (int) fread(jp2_residual_tmp, sizeof(unsigned char),
                                    n_bytes_JP2, input_LF);

    headerSize = (int) JP2_dict.at(dict_index).size();
    jp2_residual = new unsigned char[n_bytes_JP2 + headerSize]();

    memcpy(jp2_residual, &JP2_dict.at(dict_index)[0], headerSize);
    memcpy(jp2_residual + headerSize, jp2_residual_tmp, n_bytes_JP2);

    n_bytes_JP2 += headerSize;

    delete[] (jp2_residual_tmp);

  } else {

    n_bytes_residual += (int) fread(&n_bytes_JP2, sizeof(int), 1, input_LF)
        * sizeof(int);
    jp2_residual = new unsigned char[n_bytes_JP2]();
    n_bytes_residual += (int) fread(jp2_residual, sizeof(unsigned char),
                                    n_bytes_JP2, input_LF)
        * sizeof(unsigned char);

  }

  FILE *jp2_res_file;
  jp2_res_file = IOManager::fopen(jp2_residual_path_jp2, "wb");
  fwrite(jp2_residual, sizeof(unsigned char), n_bytes_JP2, jp2_res_file);
  fclose(jp2_res_file);

  delete[] (jp2_residual);
}

void writeResidualToDisk(
    const char *jp2_residual_path_jp2,
    FILE *output_LF_file, 
    int &n_bytes_residual,
    std::vector<std::vector<unsigned char>> &JP2_dict) {

  int n_bytes_JP2 = aux_GetFileSize(jp2_residual_path_jp2);

  unsigned char *jp2_residual = new unsigned char[n_bytes_JP2]();
  FILE *jp2_color_residual_file = IOManager::fopen(jp2_residual_path_jp2, "rb");
  fread(jp2_residual, sizeof(unsigned char), n_bytes_JP2,
        jp2_color_residual_file);
  fclose(jp2_color_residual_file);

  if (USE_JP2_DICTIONARY) {
    bool updateDictionary = false;
    /* get header */
    unsigned char *JP2header;
    int headerSize = 0;
    getJP2Header(jp2_residual, JP2header, n_bytes_JP2, headerSize);
    /* get index in dictionary */
    int dict_index = getJP2DictionaryIndex(JP2header, headerSize, JP2_dict);
    /* update dictionary if needed */
    if (dict_index == -1) {
      updateDictionary = true;
      updateJP2Dictionary(JP2_dict, JP2header, headerSize);
      dict_index = (int) JP2_dict.size() - 1;
      //printf("Dictonary update, index=%i\n", dict_index);
    }

    //printf("Using dictionary index:\t%i\n", dict_index);

    delete[] (JP2header);

    /* write index of dictionary to bitstream */
    unsigned char dict_index_char = (unsigned char) dict_index;
    n_bytes_residual += (int) fwrite(&dict_index_char, sizeof(unsigned char), 1,
                                     output_LF_file) * sizeof(unsigned char);

    if (updateDictionary) { /* add update information if necessary */
      n_bytes_residual += (int) fwrite(&headerSize, sizeof(int), 1,
                                       output_LF_file) * sizeof(int);
      n_bytes_residual += (int) fwrite(&JP2_dict.at(dict_index)[0],
                                       sizeof(unsigned char), headerSize,
                                       output_LF_file) * sizeof(unsigned char);
    }

    /* write to codestream without header */

    n_bytes_JP2 = n_bytes_JP2 - headerSize;

    n_bytes_residual += (int) fwrite(&n_bytes_JP2, sizeof(int), 1,
                                     output_LF_file) * sizeof(int);
    n_bytes_residual += (int) fwrite(jp2_residual + headerSize,
                                     sizeof(unsigned char), n_bytes_JP2,
                                     output_LF_file) * sizeof(unsigned char);

  } else {

    n_bytes_residual += (int) fwrite(&n_bytes_JP2, sizeof(int), 1,
                                     output_LF_file) * sizeof(int);
    n_bytes_residual += (int) fwrite(jp2_residual, sizeof(unsigned char),
                                     n_bytes_JP2, output_LF_file)
        * sizeof(unsigned char);

    /*n_bytes_JP2 = n_bytes_JP2 - jp2_headersize;
     n_bytes_residual += (int)fwrite(&n_bytes_JP2, sizeof(int), 1, output_LF_file) * sizeof(int);
     n_bytes_residual += (int)fwrite(jp2_residual + jp2_headersize, sizeof(unsigned char), n_bytes_JP2, output_LF_file) * sizeof(unsigned char);*/

  }

  delete[] (jp2_residual);

}

char *kakadu_oparams(
    const double rate,
    const std::string colorspace) {

    char *oparams = new char[65536]();

    std::string cycc;

    if (colorspace.compare("RGB")==0) {
        cycc = "yes";
    }
    if (colorspace.compare("YCbCr")==0) {
        cycc = "no";
    }

    sprintf(
        oparams, 
        " -num_threads 0"
        " -no_weights"
        " -full"
        " -precise"
        " -no_info"
        " Clevels=6"
        " -rate %2.3f"
        " Cycc=%s ",
        rate,
        cycc.c_str());

    return oparams;

}

char *kakadu_cparams(
    const double *cweights,
    const int ncomp) {

    char *cparams = new char[65536]();

    int s_index = 0;

    for (int icomp = 0; icomp < ncomp; icomp++) {
        s_index += sprintf(
            cparams + s_index, 
            "%s%1d%s%2.2f",
            " Cweight:T0C",
            icomp,
            "=",
            cweights[icomp]);
    }

    return cparams;
}

int encodeKakadu(
    const char *ppm_pgm_input_path,
    const char *kdu_compress_path,
    const char *jp2_output_path,
    const char *encoding_parameters,
    const double rate) {

    IOManager::ensure_directory(jp2_output_path);

    char kdu_compress_s[1024];

    sprintf(
        kdu_compress_s,
        "\"%s\"%s%s%s%s%s",
        kdu_compress_path,
        " -i ",
        ppm_pgm_input_path,
        " -o ",
        jp2_output_path,
        encoding_parameters);

    return system_1(kdu_compress_s);

}

int decodeKakadu(
    const char *ppm_pgm_output_path,
    const char *kdu_expand_path,
    const char *jp2_input_path) {

    IOManager::ensure_directory(jp2_input_path);
    IOManager::ensure_directory(ppm_pgm_output_path);

    char kdu_expand_s[1024];

    sprintf(
        kdu_expand_s,
        "\"%s\"%s%s%s%s", 
        kdu_expand_path, 
        " -i ",
        jp2_input_path, 
        " -o ", 
        ppm_pgm_output_path);

    return system_1(kdu_expand_s);

}

double* get_residual(
    const unsigned short *original,
    const unsigned short *prediction,
    const int nr,
    const int nc,
    const int ncomp) {

    double *residual = new double[nr*nc*ncomp]();

    for (int ii = 0; ii < nr*nc*ncomp; ii++) {
        double o = static_cast<double>(original[ii]);
        double p = static_cast<double>(prediction[ii]);
        residual[ii] = o - p;
    }

    return residual;

}

unsigned short* quantize_residual(
    const double *residual,
    const int nr,
    const int nc,
    const int ncomp,
    const int bpc,
    const int Q_i,
    const int offset_i) {

    double Q = static_cast<double>(Q_i);
    double offset = static_cast<double>(offset_i);

    unsigned short *qresidual = new unsigned short[nr*nc*ncomp]();

    for (int ii = 0; ii < nr*nc*ncomp; ii++) {
        double qpred = (residual[ii] + offset) / Q;
        qpred = floor(qpred + 0.5);
        qpred = qpred >(1 << bpc) - 1 ? (1 << bpc) - 1 : qpred;
        qpred = qpred < 0 ? 0 : qpred;
        qresidual[ii] = static_cast<unsigned short>(qpred);
    }

    return qresidual;
}

double* dequantize_residual(
    const unsigned short *qresidual,
    const int nr,
    const int nc,
    const int ncomp,
    const int bpc,
    const int Q_i,
    const int offset_i) {

    double Q = static_cast<double>(Q_i);
    double offset = static_cast<double>(offset_i);

    double *residual = new double[nr*nc*ncomp]();

    for (int ii = 0; ii < nr*nc*ncomp; ii++) {
        double qres = static_cast<double>(qresidual[ii]);
        residual[ii] = qres * Q - offset;
    }

    return residual;

}

unsigned short *apply_residual(
    const unsigned short *prediction,
    const double *residual,
    const int nr,
    const int nc,
    const int ncomp,
    const int bpc) {

    unsigned short *corrected = new unsigned short[nr*nc*ncomp]();

    for (int ii = 0; ii < nr*nc*ncomp; ii++) {

        double pred = static_cast<double>(prediction[ii]);
        double corrected_d = pred + residual[ii];
        double maxval = static_cast<double>((1 << bpc) - 1);

        corrected_d = clip(corrected_d, 0.0, maxval);
        corrected_d = floor(corrected_d + 0.5);

        corrected[ii] = static_cast<unsigned short>(corrected_d);
    }

    return corrected;
}

unsigned short* decode_residual_JP2(
    const char *ppm_pgm_output_path,
    const char *kdu_expand_path,
    const char *jp2_input_path) {

    unsigned short *residual_image_decoded = nullptr;

    print_codec_JP2_call(
        "decodeKakadu",
        jp2_input_path,
        ppm_pgm_output_path,
        __FILE__,
        __FUNCTION__, __LINE__);

    decodeKakadu(
        ppm_pgm_output_path,
        kdu_expand_path,
        jp2_input_path);

    int nr1, nc1, ncomp1;

    aux_read16PGMPPM(
        ppm_pgm_output_path,
        nc1,
        nr1,
        ncomp1,
        residual_image_decoded);

    return residual_image_decoded;
}

void encode_residual_JP2(
    const char *ppm_pgm_input_path,
    const char *kdu_compress_path,
    const char *jp2_output_path,
    const char *encoding_parameters,
    const double rate) {

    print_codec_JP2_call(
        "encodeKakadu",
        ppm_pgm_input_path,
        jp2_output_path,
        __FILE__,
        __FUNCTION__,
        __LINE__);

    int status = encodeKakadu(
        ppm_pgm_input_path,
        kdu_compress_path,
        jp2_output_path,
        encoding_parameters,
        rate);

}