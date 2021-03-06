/*
 * WaSPDecoder.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>,
 *         Pekka Astola
 *         <pekka.astola@tuni.fi>
 */

#include <cstdio>
#include <cstring>
#include <cmath>

#include "WaSPDecoder.hh"
#include "IOManager.hh"
#include "DecoderSetup.hh"
#include "view.hh"
#include "minconf.hh"
#include "codestream.hh"
#include "predictdepth.hh"
#include "residualjp2.hh"
#include "iolog.hh"
#include "medianfilter.hh"
#include "ppm.hh"
#include "warping.hh"
#include "merging.hh"
#include "inpainting.hh"
#include "sparsefilter.hh"


WaSPDecoder::WaSPDecoder(DecoderSetup decoderSetup)
    : setup(decoderSetup) {
  input_LF = IOManager::fopen(setup.input_coded_bitstream.c_str(), "rb");
}

WaSPDecoder::~WaSPDecoder() {
  if (LF != nullptr)
    dealloc();
  fclose(input_LF);
}

void WaSPDecoder::run(void) {
  WaSP_decode_header();
  WaSP_decode_views();
}

void WaSPDecoder::WaSP_decode_header() {

  n_bytes_prediction += (int) fread(
      &number_of_views, 
      sizeof(int), 
      1, 
      input_LF)
      * sizeof(int);

  n_bytes_prediction += (int) fread(
      &number_of_rows, 
      sizeof(int), 
      1, 
      input_LF)
      * sizeof(int);

  n_bytes_prediction += (int) fread(
      &number_of_columns, 
      sizeof(int), 
      1,
      input_LF) * sizeof(int);

  n_bytes_prediction += (int) fread(
      &minimum_depth, 
      sizeof(unsigned short),
      1,
      input_LF) * sizeof(unsigned short);

  uint8_t colorspace_enumerator;

  n_bytes_prediction += (int)fread(
      &colorspace_enumerator,
      sizeof(uint8_t),
      1,
      input_LF) * sizeof(uint8_t);

  if (colorspace_enumerator == 0) {
      colorspace_LF = "RGB";
  }
  if (colorspace_enumerator == 1) {
      colorspace_LF = "YCbCr";
  }

  n_bytes_prediction += (int)fread(
      &maxh,
      sizeof(int),
      1,
      input_LF) * sizeof(uint8_t);

}

void WaSPDecoder::forward_warp_texture_references(
    view *LF,
    view *SAI,
    unsigned short **warped_texture_views,
    unsigned short **warped_depth_views,
    float **DispTargs) {

    for (int ij = 0; ij < SAI->n_references; ij++) {

        view *ref_view = LF + SAI->references[ij];

        int tmp_w, tmp_r, tmp_ncomp;

        aux_read16PGMPPM(
            ref_view->path_out_pgm,
            tmp_w,
            tmp_r,
            tmp_ncomp,
            ref_view->depth);

        aux_read16PGMPPM(
            ref_view->path_internal_colorspace_out_ppm,
            tmp_w,
            tmp_r,
            tmp_ncomp,
            ref_view->color);

        /* FORWARD warp color */
        warpView0_to_View1(
            ref_view,
            SAI,
            warped_texture_views[ij],
            warped_depth_views[ij],
            DispTargs[ij]);

        delete[](ref_view->depth);
        delete[](ref_view->color);

        ref_view->depth = nullptr;
        ref_view->color = nullptr;

        /*if (SAVE_PARTIAL_WARPED_VIEWS) {

            char tmp_str[1024];

            sprintf(tmp_str, "%s/%03d_%03d_warped_to_%03d_%03d.ppm",
                setup.output_directory.c_str(), (ref_view)->c, (ref_view)->r,
                SAI->c, SAI->r);
            print_io_log("aux_write16PGMPPM", __FILE__, __FUNCTION__, __LINE__,
                tmp_str);
            aux_write16PGMPPM(tmp_str, SAI->nc, SAI->nr, 3, warped_texture_views[ij]);

            sprintf(tmp_str, "%s/%03d_%03d_warped_to_%03d_%03d.pgm",
                setup.output_directory.c_str(), (ref_view)->c, (ref_view)->r,
                SAI->c, SAI->r);
            print_io_log("aux_write16PGMPPM", __FILE__, __FUNCTION__, __LINE__,
                tmp_str);
            aux_write16PGMPPM(tmp_str, SAI->nc, SAI->nr, 1, warped_depth_views[ij]);

        }*/

    }


}


void WaSPDecoder::merge_texture_views(
    view *SAI,
    view *LF,
    unsigned short **warped_texture_views,
    float **DispTargs) {

    initViewW(SAI, DispTargs);

    if (SAI->mmode == 0) {

        for (int icomp = 0; icomp < SAI->ncomp; icomp++) {
            mergeWarped_N_icomp(
                warped_texture_views,
                SAI,
                icomp);
        }

        /* hole filling for texture*/
        for (int icomp = 0; icomp < SAI->ncomp; icomp++) {
            uint32_t nholes = holefilling(
                SAI->color + icomp*SAI->nr*SAI->nc,
                SAI->nr,
                SAI->nc,
                (unsigned short)0,
                SAI->seg_vp);
        }
    }

    if (SAI->mmode == 1) {
        /* we don't use LS weights but something derived on geometric distance in view array*/
        for (int icomp = 0; icomp < SAI->ncomp; icomp++) {
            getGeomWeight_icomp(
                SAI,
                LF,
                icomp);
        }

        /* merge color with prediction */
        for (int icomp = 0; icomp < SAI->ncomp; icomp++) {
            mergeWarped_N_icomp(
                warped_texture_views,
                SAI,
                icomp);
        }

        /* hole filling for texture*/
        for (int icomp = 0; icomp < 3; icomp++) {
            uint32_t nholes = holefilling(
                SAI->color + icomp*SAI->nr*SAI->nc,
                SAI->nr,
                SAI->nc,
                (unsigned short)0,
                SAI->seg_vp);
        }
    }

    if (SAI->mmode == 2) {

        mergeMedian_N(warped_texture_views, DispTargs, SAI, 3);

        /* hole filling for texture*/
        for (int icomp = 0; icomp < 3; icomp++) {
            uint32_t nholes = holefilling(
                SAI->color + icomp*SAI->nr*SAI->nc,
                SAI->nr,
                SAI->nc,
                (unsigned short)0,
                SAI->seg_vp);
        }

    }

    delete[](SAI->seg_vp);
    SAI->seg_vp = nullptr;
    delete[](SAI->bmask);
    SAI->bmask = nullptr;
}

void WaSPDecoder::predict_texture_view(view* SAI) {

    if (SAI->n_references > 0) {

        unsigned short **warped_texture_views = nullptr;
        unsigned short **warped_depth_views = nullptr;
        float **DispTargs = nullptr;

        init_warping_arrays(
            SAI->n_references,
            warped_texture_views,
            warped_depth_views,
            DispTargs,
            SAI->nr,
            SAI->nc,
            SAI->ncomp);

        forward_warp_texture_references(
            LF,
            SAI,
            warped_texture_views,
            warped_depth_views,
            DispTargs);

        merge_texture_views(
            SAI,
            LF,
            warped_texture_views,
            DispTargs);

        clean_warping_arrays(
            SAI->n_references,
            warped_texture_views,
            warped_depth_views,
            DispTargs);

        if (SAI->use_global_sparse) {

            for (int icomp = 0; icomp < SAI->ncomp; icomp++) {

                dequantize_and_reorder_spfilter(SAI->sparse_filters.at(icomp));

                std::vector<double> filtered_icomp = applyGlobalSparseFilter(
                    SAI->color + SAI->nr*SAI->nc*icomp,
                    SAI->nr,
                    SAI->nc,
                    SAI->Ms,
                    SAI->NNt,
                    SPARSE_BIAS_TERM,
                    SAI->sparse_filters.at(icomp).filter_coefficients);

                for (int iij = 0; iij < SAI->nr*SAI->nc; iij++) {

                    double mmax = static_cast<double>((1 << BIT_DEPTH) - 1);

                    filtered_icomp[iij] =
                        clip(filtered_icomp[iij], 0.0, mmax);

                    SAI->color[SAI->nr*SAI->nc*icomp + iij] =
                        static_cast<unsigned short>(floor(filtered_icomp[iij] + 0.5));

                }
            }
        }
    }
}

void WaSPDecoder::WaSP_decode_views() {

  LF = new view[number_of_views]();

  int ii = 0; /*view index*/

  while (ii < number_of_views) {

    view *SAI = LF + ii;
    ii++;

    initView(SAI);

    SAI->nr = number_of_rows;
    SAI->nc = number_of_columns;

    SAI->colorspace = colorspace_LF;

    if (minimum_depth > 0) {
      SAI->min_inv_d = static_cast<int>(minimum_depth);
    }

    minimal_config mconf;

    codestreamToViewHeader(
        n_bytes_prediction, 
        SAI, 
        input_LF, 
        mconf);

    setPaths(
        SAI,
        "",
        setup.output_directory.c_str());

    if (feof(input_LF)) {
      printf("File reading error. Terminating\t...\n");
      exit(0);
    }

    printf("Decoding view %03d_%03d\t", SAI->c, SAI->r);

    SAI->color = new unsigned short[SAI->nr * SAI->nc * 3]();
    SAI->depth = new unsigned short[SAI->nr * SAI->nc]();
    
    /*main texture prediction here*/
    predict_texture_view(SAI);

    /*extract residuals from codestream*/
    if (SAI->has_color_residual) {
        readResidualFromDisk(
            SAI->jp2_residual_path_jp2,
            n_bytes_residual,
            input_LF,
            JP2_dict);
    }

    if (SAI->has_depth_residual) {
        readResidualFromDisk(
            SAI->jp2_residual_depth_path_jp2,
            n_bytes_residual,
            input_LF,
            JP2_dict);
    }

    /* apply texture residual */
    if (SAI->has_color_residual) {

        int Q = 1;
        int offset = 0;

        if (SAI->level > 1) {
            Q = 2;
            const int bpc = 10;
            offset = (1 << bpc) - 1; /* 10bit images currently */
        }

        unsigned short *decoded_residual_image = decode_residual_JP2(
            SAI->path_raw_texture_residual_at_decoder_ppm,
            (setup.wasp_kakadu_directory + "/kdu_expand").c_str(),
            SAI->jp2_residual_path_jp2);

        print_io_log(
            "aux_write16PGMPPM",
            __FILE__,
            __FUNCTION__,
            __LINE__,
            SAI->path_raw_texture_residual_at_decoder_ppm);

        aux_write16PGMPPM(
            SAI->path_raw_texture_residual_at_decoder_ppm,
            SAI->nc,
            SAI->nr,
            3,
            decoded_residual_image);

        double *residual = dequantize_residual(
            decoded_residual_image,
            SAI->nr,
            SAI->nc,
            SAI->ncomp,
            10,
            Q,
            offset);

        unsigned short *corrected = apply_residual(
            SAI->color,
            residual,
            SAI->nr,
            SAI->nc,
            SAI->ncomp,
            10);

        /* update SAI->color to contain
        corrected (i.e., prediction + residual) version*/
        memcpy(
            SAI->color,
            corrected,
            sizeof(unsigned short)*SAI->nr*SAI->nc*SAI->ncomp);

        delete[](residual);
        delete[](corrected);
        delete[](decoded_residual_image);

    }

    if (SAI->has_depth_residual) {

      /* has JP2 encoded depth */

      decodeKakadu(
          SAI->path_out_pgm,
          (setup.wasp_kakadu_directory + "/kdu_expand").c_str(),
          SAI->jp2_residual_depth_path_jp2);

      int nr1, nc1, ncomp1;

      aux_read16PGMPPM(
          SAI->path_out_pgm, 
          nc1, 
          nr1, 
          ncomp1, 
          SAI->depth);

    }
    else {

        /*inverse depth prediction*/

        if (SAI->level < maxh) {
            WaSP_predict_depth(SAI, LF);
        }

    }

    if (MEDFILT_DEPTH) {

        unsigned short *filtered_depth = medfilt2D(
            SAI->depth,
            3,
            SAI->nr,
            SAI->nc);

        memcpy(
            SAI->depth,
            filtered_depth,
            sizeof(unsigned short) * SAI->nr * SAI->nc);

        delete[](filtered_depth);

    }

    /*internal colorspace version*/

    print_io_log(
        "aux_write16PGMPPM", 
        __FILE__, 
        __FUNCTION__, 
        __LINE__,
        SAI->path_internal_colorspace_out_ppm);

    aux_write16PGMPPM(
        SAI->path_internal_colorspace_out_ppm,
        SAI->nc, 
        SAI->nr, 
        SAI->ncomp, 
        SAI->color);

    /*colorspace transformation back to input*/

    print_io_log(
        "aux_write16PGMPPM",
        __FILE__,
        __FUNCTION__,
        __LINE__,
        SAI->path_out_ppm);

    write_output_ppm(
        SAI->color,
        SAI->path_out_ppm,
        SAI->nr,
        SAI->nc,
        SAI->ncomp,
        10,
        SAI->colorspace);

    /*write inverse depth .pgm*/
    if (SAI->level < maxh) {
        print_io_log(
            "aux_write16PGMPPM",
            __FILE__,
            __FUNCTION__,
            __LINE__,
            SAI->path_out_pgm);

        aux_write16PGMPPM(
            SAI->path_out_pgm,
            SAI->nc,
            SAI->nr,
            1,
            SAI->depth);
    }

    if (SAI->color != nullptr) {
      delete[] (SAI->color);
      SAI->color = nullptr;
    }

    if (SAI->depth != nullptr) {
      delete[] (SAI->depth);
      SAI->depth = nullptr;
    }

    if (SAI->seg_vp != nullptr) {
      delete[] (SAI->seg_vp);
      SAI->seg_vp = nullptr;
    }

  }
}


void WaSPDecoder::dealloc() {
  for (int ii = 0; ii < number_of_views; ii++) {
    //printf("ii=%d\n", ii);
    view* SAI = LF + ii;
    if (SAI->color != nullptr)
      delete[] (SAI->color);

    if (SAI->depth != nullptr)
      delete[] (SAI->depth);

    if (SAI->references != nullptr)
      delete[] (SAI->references);

    if (SAI->depth_references != nullptr)
      delete[] (SAI->depth_references);

    if (SAI->merge_weights != nullptr)
      delete[] (SAI->merge_weights);

    if (SAI->sparse_weights != nullptr)
      delete[] (SAI->sparse_weights);

    if (SAI->bmask != nullptr)
      delete[] (SAI->bmask);

    if (SAI->seg_vp != nullptr)
      delete[] (SAI->seg_vp);

    if (SAI->sparse_mask != nullptr)
      delete[] (SAI->sparse_mask);
  }
  delete[] (LF);
}
