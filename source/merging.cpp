/* merging.cpp */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#include "merging.hh"
#include "medianfilter.hh"
#include "fastols.hh"
#include "bitdepth.hh"
#include "warping.hh"

#include <cstring>
#include <cmath>

void init_warping_arrays(
    const int N,
    unsigned short **&warped_texture_views,
    unsigned short **&warped_depth_views,
    float **&DispTargs,
    const int nr,
    const int nc,
    const int ncomp) {

    /* holds partial warped views for ii */
    warped_texture_views = new unsigned short*[N]();
    warped_depth_views = new unsigned short*[N]();
    DispTargs = new float*[N]();

    for (int ij = 0; ij < N; ij++) {
        warped_texture_views[ij] = new unsigned short[nr*nc*ncomp]();
        warped_depth_views[ij] = new unsigned short[nr*nc]();
        DispTargs[ij] = new float[nr*nc]();
    }
}

void clean_warping_arrays(
    const int N,
    unsigned short **warped_texture_views,
    unsigned short **warped_depth_views,
    float **DispTargs) {

    /* clean */
    for (int ij = 0; ij < N; ij++) {
        delete[](warped_texture_views[ij]);
        delete[](warped_depth_views[ij]);
        delete[](DispTargs[ij]);
    }

    delete[](warped_texture_views);
    delete[](warped_depth_views);
    delete[](DispTargs);
}

void setBMask(view *view0) {
  /* sets the binary mask used to derive view availability in each of the MMM classes,
   size of the binary mask is [MMM x n_references] */
  int MMM = 1 << view0->n_references;  // pow(2, view0->n_references);
  bool *bmask = new bool[MMM * view0->n_references]();
  (view0)->bmask = bmask;
  for (int ij = 0; ij < MMM; ij++) {
    int uu = ij;
    for (int ik = view0->n_references - 1; ik >= 0; ik--) {
      //if (floor(uu / pow(2, ik)) > 0)
      if (floor(uu / (1 << ik)) > 0) {
        //uu = uu - pow(2, ik);
        uu = uu - (1 << ik);
        bmask[ij + ik * MMM] = 1;
      }
    }
  }
}

void initSegVp(view *view0, float **DispTargs) {

  int nr = view0->nr;
  int nc = view0->nc;
  int n_references = view0->n_references;

  unsigned short *seg_vp = new unsigned short[nr * nc]();

  (view0)->seg_vp = seg_vp;

  int MMM = 1 << view0->n_references;  // pow(2, (view0)->n_references);

  int *number_of_pixels_per_region = new int[MMM]();

  for (int ii = 0; ii < nr * nc; ii++) {

    unsigned short ci = 0;

    for (int ik = 0; ik < n_references; ik++) {
      float *pf = DispTargs[ik];
      if (*(pf + ii) > INIT_DISPARITY_VALUE)
        ci = ci + (unsigned short) (1 << ik);  // pow(2, ik);
    }

    seg_vp[ii] = ci;

    number_of_pixels_per_region[ci]++;

  }

  view0->number_of_pixels_per_region = number_of_pixels_per_region;
}

void initViewW(view *view0, float **DispTargs) {

    /* sets some of the parameters for a view in the light view structure */

    int MMM = (1 << view0->n_references);

    (view0)->NB = MMM * view0->n_references;

    if ((view0)->merge_weights == nullptr) {
        signed short *merge_weights = 
            new signed short[view0->ncomp*(view0->NB / 2)]();
        (view0)->merge_weights = merge_weights;
    }

    double *merge_weights_double = new double[MMM*view0->n_references]();

    (view0)->merge_weights_double = merge_weights_double;

    setBMask(view0);

    initSegVp(view0, DispTargs);

}

void mergeMedian_N(unsigned short **warpedColorViews, float **DispTargs,
                   view *view0, const int ncomponents) {

  unsigned short *AA2 =
      new unsigned short[view0->nr * view0->nc * ncomponents]();

//#pragma omp parallel for
  for (int ii = 0; ii < view0->nr * view0->nc; ii++) {
    for (int icomp = 0; icomp < ncomponents; icomp++) {
      std::vector<unsigned short> vals;
      for (int ik = 0; ik < view0->n_references; ik++) {

        float *pf = DispTargs[ik];
        unsigned short *ps = warpedColorViews[ik];

        if (*(pf + ii) > INIT_DISPARITY_VALUE) {

          vals.push_back(*(ps + ii + icomp * view0->nr * view0->nc));

        }

      }

      if (vals.size() > 0) {
        *(AA2 + ii + icomp * view0->nr * view0->nc) = getMedian(vals);
      }
    }
  }

  memcpy(view0->color, AA2, sizeof(unsigned short) * view0->nr * view0->nc * 3);
  delete[] (AA2);

}

void printMatrix(
    const double *matrix,
    const int nr,
    const int nc) {

    printf("----------\n");
    for (int r = 0; r < nr; r++) {
        for (int c = 0; c < nc; c++) {
            int lin_ind = c*nr + r;
            printf("%2.4f\t", *(matrix + lin_ind));
        }
        printf("\n");
    }
    printf("----------\n");

}

void mergeWarped_N_icomp(
    unsigned short **warpedColorViews,
    view *view0,
    const int icomp) {

    int MMM = 1 << view0->n_references;  // pow(2, (view0)->n_references);

    int N_LS = MMM*view0->n_references;

    bool *bmask = view0->bmask;

    int uu = icomp*((MMM*view0->n_references) / 2);

    for (int ii = 0; ii < N_LS; ii++) {
        if (bmask[ii]) {
            view0->merge_weights_double[ii] =
                ((double)(view0)->merge_weights[uu++])
                / (double)(1 << BIT_DEPTH_MERGE);
        }
        else {
            (view0)->merge_weights_double[ii] = 0.0;
        }
    }

    /*printMatrix(
        view0->merge_weights_double,
        MMM,
        view0->n_references);*/

    int nr = view0->nr;
    int nc = view0->nc;
    int n_views = view0->n_references;

    double *LSw = view0->merge_weights_double;

    unsigned short *seg_vp = view0->seg_vp;

    double *AA1 = new double[nr * nc]();
    unsigned short *AA2 = new unsigned short[nr * nc]();

    for (int ii = 0; ii < nr * nc; ii++) {

        int ci = seg_vp[ii]; /* occlusion class index and row index in thetas */

        for (int ik = 0; ik < n_views; ik++) {
            unsigned short *ps = warpedColorViews[ik];
            AA1[ii] +=
                LSw[ci + ik * MMM] * ((double)(*(ps + ii + icomp * nr * nc)));
        }

        if (AA1[ii] < 0)
            AA1[ii] = 0;
        if (AA1[ii] > (1 << BIT_DEPTH) - 1)
            AA1[ii] = (1 << BIT_DEPTH) - 1;

        AA2[ii] = (unsigned short)(floor(AA1[ii] + 0.5));

    }

    memcpy(
        view0->color + icomp*nr*nc,
        AA2,
        sizeof(unsigned short) * nr * nc);

    delete[](AA1);
    delete[](AA2);

}

void getViewMergingLSWeights_icomp(
    view *view0,
    unsigned short **warpedColorViews,
    const unsigned short *original_color_view,
    const int icomp) {

    /* This function puts the LS view merging weights into LSw */

    int n_references = (view0)->n_references;
    int nr = (view0)->nr;
    int nc = (view0)->nc;

    int MMM = 1 << n_references;  // pow(2, n_references);

    signed short *LScoeffs = view0->merge_weights;

    bool *bmask = (view0)->bmask;

    unsigned short *seg_vp = (view0)->seg_vp;

    /* go through all regions, collect regressors from references */
    unsigned short **reference_view_pixels_in_classes = new unsigned short*[MMM
        * n_references]();

    /* also collect desired values */
    unsigned short **original_view_in_classes = new unsigned short*[MMM]();

    int *number_of_pixels_per_region = (view0)->number_of_pixels_per_region;

    for (int ij = 1; ij < MMM; ij++) {  // region

        int NN = number_of_pixels_per_region[ij];

        original_view_in_classes[ij] = new unsigned short[NN]();
        unsigned short *p3s = original_view_in_classes[ij];

        int jj = 0;

        for (int ii = 0; ii < nr * nc; ii++) {
            if (seg_vp[ii] == ij) {
                *(p3s + jj) = *(original_color_view + ii + icomp*nr*nc);
                jj++;
            }
        }

        for (int ik = 0; ik < n_references; ik++) {  // reference view

            if (bmask[ij + ik * MMM]) {

                /* allocate pixels for region */
                reference_view_pixels_in_classes[ij + ik * MMM] =
                    new unsigned short[NN]();

                unsigned short *ps = reference_view_pixels_in_classes[ij + ik * MMM];
                unsigned short *pss = warpedColorViews[ik];

                jj = 0;

                for (int ii = 0; ii < nr * nc; ii++) {
                    if (seg_vp[ii] == ij) {
                        *(ps + jj) = *(pss + ii + icomp*nr*nc);
                        jj++;
                    }
                }
            }
        }
    }

    /* run fastOLS on the classes */
    signed short *thetas = new signed short[MMM * n_references]();
    for (int ij = 1; ij < MMM; ij++) {
        /* form A for this class, compute A'*A (phi)
        also compute A'*y (psi), where y is the desired data from the original view */

        int M = 0; /* number of active reference views for class ij */

        for (int ik = 0; ik < n_references; ik++) {
            if (bmask[ij + MMM * ik])
                M++;
        }

        int N = number_of_pixels_per_region[ij];  // number of rows in A

        double *AA = new double[N * M](); 
        double *Yd = new double[N]();

        unsigned short *ps;

        int ikk = 0;

        for (int ik = 0; ik < n_references; ik++) {
            if (bmask[ij + ik * MMM]) {
                ps = reference_view_pixels_in_classes[ij + ik * MMM];
                for (int ii = 0; ii < N; ii++) {
                    *(AA + ii + ikk * N) = ((double) *(ps + ii))
                        / (double)((1 << BIT_DEPTH) - 1);
                }
                ikk++;
            }
        }

        ps = original_view_in_classes[ij];

        for (int ii = 0; ii < N; ii++) {
            *(Yd + ii) = ((double) *(ps + ii))
                / (double)((1 << BIT_DEPTH) - 1);
        }

        /* fastols */

        int *PredRegr0 = new int[M]();
        double *PredTheta0 = new double[M]();

        //int Mtrue = FastOLS(ATA, ATYd, YdTYd, PredRegr0, PredTheta0, M, M, M);

        int Mtrue = FastOLS_new(
            &AA,
            &Yd,
            PredRegr0,
            PredTheta0,
            M, M, M, N);

        if (AA != nullptr) {
            delete[](AA);
        }
        if (Yd != nullptr) {
            delete[](Yd);
        }

        /* establish the subset of reference views available for class */
        int *iks = new int[M]();
        int ee = 0;
        for (int ik = 0; ik < n_references; ik++) {
            if (bmask[ij + ik * MMM]) {
                *(iks + ee) = ik;
                ee++;
            }
        }

        for (int ii = 0; ii < M; ii++) {
            double quant_theta = floor(*(PredTheta0 + ii) * static_cast<double>(1 << BIT_DEPTH_MERGE) + 0.5);
            double max_t_val = static_cast<double>((1 << (16 - 1)));
            quant_theta = quant_theta > max_t_val ? max_t_val : quant_theta;
            quant_theta = quant_theta < -max_t_val ? -max_t_val : quant_theta;
            thetas[ij + MMM * iks[PredRegr0[ii]]] = static_cast<signed short>(quant_theta);  // pow(2, BIT_DEPTH_MERGE) + 0.5);
        }

        delete[](iks);

        delete[](PredRegr0);
        delete[](PredTheta0);

    }

    /* columnwise collecting of thetas */
    int N_LS = (MMM*n_references) / 2;
    int in = N_LS*icomp;
    for (int ik = 0; ik < n_references; ik++) {
        for (int ij = 0; ij < MMM; ij++) {
            if (bmask[ij + ik * MMM]) {
                LScoeffs[in] = thetas[ij + MMM * ik];
                in++;
            }
        }
    }

    delete[](thetas);

    for (int ij = 0; ij < MMM; ij++) {

        delete[](original_view_in_classes[ij]);

        for (int ik = 0; ik < n_references; ik++) {
            delete[](reference_view_pixels_in_classes[ij + MMM * ik]);
        }

    }

    delete[](original_view_in_classes);
    delete[](reference_view_pixels_in_classes);
    //delete[](seg_vp);
    //delete[](number_of_pixels_per_region);

}

void getGeomWeight_icomp(
    view *view0,
    view *LF,
    const int icomp) {

    float stdd = view0->stdd;

    int MMM = 1 << view0->n_references;

    double *thetas = new double[MMM*(view0->n_references)]();

    bool *bmask = (view0)->bmask;

    for (int ii = 0; ii < MMM; ii++) {
        double sumw = 0;

        for (int ij = 0; ij < (view0)->n_references; ij++) {
            view *view1 = LF + (view0)->references[ij];
            double vdistance = (view0->x - view1->x) * (view0->x - view1->x)
                + (view0->y - view1->y) * (view0->y - view1->y);
            if (bmask[ii + ij * MMM]) {
                thetas[ii + ij * MMM] = exp(-(vdistance) / (2 * stdd * stdd));
                sumw = sumw + thetas[ii + ij * MMM];
            }
        }
        for (int ij = 0; ij < (view0)->n_references; ij++)
            thetas[ii + ij * MMM] = thetas[ii + ij * MMM] / sumw;
    }

    /* columnwise collecting of thetas */
    signed short *LScoeffs = (view0)->merge_weights;
    int in = icomp*((MMM*view0->n_references) / 2);
    for (int ik = 0; ik < (view0)->n_references; ik++) {
        for (int ij = 0; ij < MMM; ij++) {
            if (bmask[ij + ik * MMM]) {
                LScoeffs[in] = (signed short)floor(
                    thetas[ij + MMM * ik] * (signed short)(1 << BIT_DEPTH_MERGE)
                    + 0.5);  // pow(2, BIT_DEPTH_MERGE) + 0.5);
                in++;
            }
        }
    }

    delete[](thetas);

}