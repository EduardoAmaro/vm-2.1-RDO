/* codestream.cpp */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#include "codestream.hh"
#include "view.hh"
#include "minconf.hh"

#include <iostream>
#include <vector>


void viewHeaderToCodestream(
    int &n_bytes_prediction, 
    view *SAI,
    FILE *output_LF_file) {

  minimal_config mconf = makeMinimalConfig(SAI);

  //printf("size of minimal_config %i bytes\n", (int)sizeof(minimal_config));

  n_bytes_prediction += (int) fwrite(
      &mconf, 
      sizeof(minimal_config), 
      1,
      output_LF_file) * sizeof(minimal_config);

  /* lets see what else needs to be written to bitstream */

  if (SAI->has_x_displacement) {
    n_bytes_prediction += (int) fwrite(&SAI->x, sizeof(float), 1,
                                       output_LF_file) * sizeof(float);
  }

  if (SAI->has_y_displacement) {
    n_bytes_prediction += (int) fwrite(&SAI->y, sizeof(float), 1,
                                       output_LF_file) * sizeof(float);
  }

  if (SAI->has_color_references) {
    unsigned char tmpNREF = (unsigned char) SAI->n_references;
    n_bytes_prediction += (int) fwrite(&tmpNREF, sizeof(unsigned char), 1,
                                       output_LF_file) * sizeof(unsigned char);
    for (int ij = 0; ij < SAI->n_references; ij++) {
      unsigned short nid = (unsigned short) *(SAI->references + ij);
      n_bytes_prediction += (int) fwrite(&nid, sizeof(unsigned short), 1,
                                         output_LF_file)
          * sizeof(unsigned short);
    }
  }

  if (SAI->has_depth_references) {
    unsigned char tmpNDREF = (unsigned char) SAI->n_depth_references;
    n_bytes_prediction += (int) fwrite(&tmpNDREF, sizeof(unsigned char), 1,
                                       output_LF_file) * sizeof(unsigned char);
    for (int ij = 0; ij < SAI->n_depth_references; ij++) {
      unsigned short nid = (unsigned short) *(SAI->depth_references + ij);
      n_bytes_prediction += (int) fwrite(&nid, sizeof(unsigned short), 1,
                                         output_LF_file)
          * sizeof(unsigned short);
    }
  }

  int MMM = (1 << SAI->n_references);
  int N_LS = SAI->ncomp*((MMM*SAI->n_references) / 2);

  if (SAI->has_color_references) {
      if (SAI->mmode == 0) {
          /* use LS merging weights */
          n_bytes_prediction += (int)fwrite(
              SAI->merge_weights,
              sizeof(signed short),
              N_LS,
              output_LF_file)
              * sizeof(signed short);
      }
      if (SAI->mmode == 1) {
          /* use standard deviation */
          n_bytes_prediction += (int)fwrite(
              &SAI->stdd,
              sizeof(float),
              1,
              output_LF_file) * sizeof(float);
      }
  }

  if (SAI->use_global_sparse) {

      unsigned char tmpNNt = (unsigned char)SAI->NNt;
      unsigned char tmpMs = (unsigned char)SAI->Ms;

      n_bytes_prediction += (int)fwrite(
          &tmpNNt,
          sizeof(unsigned char),
          1,
          output_LF_file) * sizeof(unsigned char);

      n_bytes_prediction += (int)fwrite(
          &tmpMs,
          sizeof(unsigned char),
          1,
          output_LF_file) * sizeof(unsigned char);

      for (int icomp = 0; icomp < SAI->ncomp; icomp++) {
          n_bytes_prediction += (int)fwrite(
              &SAI->sparse_filters.at(icomp).quantized_filter_coefficients[0],
              sizeof(int32_t),
              SAI->Ms,
              output_LF_file)
              * sizeof(int32_t);
      }

      int32_t Nsp = (SAI->NNt * 2 + 1)* (SAI->NNt * 2 + 1) + 1;
      int32_t sp_mask_nbytes = (Nsp % 8) ? Nsp / 8 + 1 : Nsp / 8;

      uint8_t *sparsemask = new uint8_t[sp_mask_nbytes*SAI->ncomp]();

      for (int icomp = 0; icomp < SAI->ncomp; icomp++) {

          for (int ij = 0; ij < SAI->Ms; ij++) {

              uint32_t regr_indx = 
                  SAI->sparse_filters.at(icomp).regressor_indexes.at(ij);

              uint32_t q = regr_indx / 8;

              uint8_t *sparse_mask_byte = &sparsemask[q+sp_mask_nbytes*icomp];

              *sparse_mask_byte = *sparse_mask_byte
                  | (1 << (regr_indx - q * 8));

          }
      }

      n_bytes_prediction += (int)fwrite(
          sparsemask,
          sizeof(uint8_t),
          sp_mask_nbytes*SAI->ncomp,
          output_LF_file)
          * sizeof(uint8_t);

      delete[](sparsemask);

  }

  return;

}

void codestreamToViewHeader(
    int &n_bytes_prediction, 
    view *SAI, 
    FILE *input_LF,
    minimal_config &mconf) {

  n_bytes_prediction += (int) fread(
      &mconf, 
      sizeof(minimal_config),
      1, 
      input_LF)
      * sizeof(minimal_config);

  setup_form_minimal_config(&mconf, SAI);

  if (SAI->has_x_displacement) {
    n_bytes_prediction += (int) fread(&SAI->x, sizeof(float), 1, input_LF)
        * sizeof(float);
  }

  if (SAI->has_y_displacement) {
    n_bytes_prediction += (int) fread(&SAI->y, sizeof(float), 1, input_LF)
        * sizeof(float);
  }

  if (SAI->has_color_references) {

    unsigned char tmpNREF = 0;

    n_bytes_prediction += (int) fread(&tmpNREF, sizeof(unsigned char), 1,
                                      input_LF) * sizeof(unsigned char);

    SAI->n_references = tmpNREF;

    SAI->references = new int[SAI->n_references]();
    for (int ij = 0; ij < SAI->n_references; ij++) {
      unsigned short nid;
      n_bytes_prediction += (int) fread(&nid, sizeof(unsigned short), 1,
                                        input_LF) * sizeof(unsigned short);
      *(SAI->references + ij) = (int) nid;
    }
  }

  if (SAI->has_depth_references) {

    unsigned char tmpNDREF = 0;

    n_bytes_prediction += (int) fread(&tmpNDREF, sizeof(unsigned char), 1,
                                      input_LF) * sizeof(unsigned char);

    SAI->n_depth_references = tmpNDREF;

    SAI->depth_references = new int[SAI->n_depth_references]();
    for (int ij = 0; ij < SAI->n_depth_references; ij++) {
      unsigned short nid;
      n_bytes_prediction += (int) fread(&nid, sizeof(unsigned short), 1,
                                        input_LF) * sizeof(unsigned short);
      *(SAI->depth_references + ij) = (int) nid;
    }
  }

  SAI->NB = (1 << SAI->n_references) * SAI->n_references;

  int MMM = (1 << SAI->n_references);
  int N_LS = SAI->ncomp*((MMM*SAI->n_references) / 2);

  if (SAI->has_color_references) {
      if (SAI->mmode == 0) {
          SAI->merge_weights = new signed short[N_LS]();
          /* use LS merging weights */
          n_bytes_prediction += (int)fread(
              SAI->merge_weights,
              sizeof(signed short),
              N_LS,
              input_LF)
              * sizeof(signed short);
      }
      if (SAI->mmode == 1) {
          /* use standard deviation */
          n_bytes_prediction += (int)fread(
              &SAI->stdd,
              sizeof(float),
              1,
              input_LF) * sizeof(float);
      }
  }

  if (SAI->use_global_sparse) {

      unsigned char tmpNNt = 0;
      unsigned char tmpMs = 0;

      n_bytes_prediction += (int)fread(
          &tmpNNt,
          sizeof(unsigned char),
          1,
          input_LF) * sizeof(unsigned char);

      n_bytes_prediction += (int)fread(
          &tmpMs,
          sizeof(unsigned char),
          1,
          input_LF) * sizeof(unsigned char);

      SAI->NNt = (int)tmpNNt;
      SAI->Ms = (int)tmpMs;

      SAI->sparse_filters.clear();

      for (int icomp = 0; icomp < SAI->ncomp; icomp++) {

          spfilter tmpsp;
          SAI->sparse_filters.push_back(tmpsp);

          SAI->sparse_filters.at(icomp).Ms = SAI->Ms;
          SAI->sparse_filters.at(icomp).NNt = SAI->NNt;

          SAI->sparse_filters.at(icomp).quantized_filter_coefficients.clear();

          for (int ii = 0; ii < SAI->Ms; ii++) {
              SAI->sparse_filters.at(icomp).quantized_filter_coefficients.push_back(0);
          }

          n_bytes_prediction += (int)fread(
              &SAI->sparse_filters.at(icomp).quantized_filter_coefficients[0],
              sizeof(int32_t),
              SAI->Ms,
              input_LF)
              * sizeof(int32_t);
      }


      int32_t Nsp = (SAI->NNt * 2 + 1)* (SAI->NNt * 2 + 1) + 1;
      int32_t sp_mask_nbytes = (Nsp % 8) ? Nsp / 8 + 1 : Nsp / 8;

      uint8_t *sparsemask = new uint8_t[sp_mask_nbytes*SAI->ncomp]();

      n_bytes_prediction += (int)fread(
          sparsemask,
          sizeof(uint8_t),
          sp_mask_nbytes*SAI->ncomp,
          input_LF)
          * sizeof(uint8_t);

      for (int icomp = 0; icomp < SAI->ncomp; icomp++) {

          SAI->sparse_filters.at(icomp).regressor_indexes.clear();

          for (int ii = 0; ii < Nsp; ii++) {
              SAI->sparse_filters.at(icomp).regressor_indexes.push_back(0);
          }

          uint32_t ik = 0;

          for (int ij = 0; ij < Nsp; ij++) {

              uint32_t q = ij / 8;

              uint8_t *sparse_mask_byte = &sparsemask[sp_mask_nbytes*icomp + q];

              if (*sparse_mask_byte & (1 << (ij - q * 8))) {
                  SAI->sparse_filters.at(icomp).regressor_indexes.at(ik) = ij;
                  ik++;
              }

          }
      }

      delete[](sparsemask);

  }

  return;

}
