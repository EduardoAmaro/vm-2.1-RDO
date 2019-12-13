// No subbands version (encoding is performed on a block-by-block basis)
#include "LightField.h"
#include "Hierarchical4DEncoder.h"
#include "MultiscaleTransform.h"
#include "TransformPartition.h"

//DSC begin
//#include "Prediction.h"
//#include "Statistics.h"
#include <iostream>
#include <fstream>

using namespace std;
//DSC end

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

enum ExtensionType {
    ZERO_PADDING, REPEAT_LAST, CYCLIC, EXTENDED_DCT, TRANSFORM_DOMAIN_ZERO_PADDING, MEAN_VALUE, NONE
};
void ExtendDCT(Matrix &extendedDCT, ExtensionType extensionMethod, int transformLength, int extensionLength);
void ExtendBlock4D(Block4D &extendedblock, ExtensionType extensionMethod, int extensionLength, char direction);
void RGB2YCbCr_BT601(Block4D &Y, Block4D &Cb, Block4D &Cr, Block4D const &R, Block4D const &G, Block4D const &B, int Scale);

class EncoderParameters {
public:
    // DSC begin
    char Prediction[20];
    int SuperiorBitPlane;
    int InferiorBitPlane;
    int EvaluateOptimumBitPlane;
    int Split;
    int PerformRDO;
    // DSC end
    double Lambda;
    int transformLength_t;
    int transformLength_s;
    int transformLength_v;
    int transformLength_u;
    int min_transformLength_t;
    int min_transformLength_s;
    int min_transformLength_v;
    int min_transformLength_u;
    int inputNumberOfVerticalViews;
    int inputNumberOfHorizontalViews;
    int firstHorizontalViewNumber;
    int firstVerticalViewNumber;
    char isLenslet13x13;
    char inputDirectory[1024];
    char outputFileName[1024];
    char configurationFileName[1024];
    ExtensionType extensionMethod;
    double transform_scale_t;
    double transform_scale_s;
    double transform_scale_v;
    double transform_scale_u;
    int verbosity;
    void ReadConfigurationFile(char *parametersFileName);
    void DisplayConfiguration(void);
};

void EncoderParameters::ReadConfigurationFile(char *parametersFileName) {
    char command[128];
    FILE *parametersFilepointer;

    if ((parametersFilepointer = fopen(parametersFileName, "r")) == NULL) {
        printf("ERROR: unable to open configuration file %s\n", parametersFileName);
        exit(0);
    }

    fscanf(parametersFilepointer, "%s", command);
    while (feof(parametersFilepointer) == 0) {
        if (strcmp(command, "-lambda") == 0) {
            fscanf(parametersFilepointer, "%lf", &Lambda);
        }
        if (strcmp(command, "-l") == 0) {
            fscanf(parametersFilepointer, "%d", &transformLength_u);
            fscanf(parametersFilepointer, "%d", &transformLength_v);
        }
        if (strcmp(command, "-u") == 0) fscanf(parametersFilepointer, "%d", &transformLength_u);
        if (strcmp(command, "-v") == 0) fscanf(parametersFilepointer, "%d", &transformLength_v);
        if (strcmp(command, "-k") == 0) {
            fscanf(parametersFilepointer, "%d", &transformLength_t);
            fscanf(parametersFilepointer, "%d", &transformLength_s);
        }
        if (strcmp(command, "-s") == 0) fscanf(parametersFilepointer, "%d", &transformLength_s);
        if (strcmp(command, "-t") == 0) fscanf(parametersFilepointer, "%d", &transformLength_t);
        if (strcmp(command, "-lf") == 0) fscanf(parametersFilepointer, "%s", &inputDirectory);
        if (strcmp(command, "-o") == 0) fscanf(parametersFilepointer, "%s", &outputFileName);
        if (strcmp(command, "-nv") == 0) fscanf(parametersFilepointer, "%d", &inputNumberOfVerticalViews);
        if (strcmp(command, "-nh") == 0) fscanf(parametersFilepointer, "%d", &inputNumberOfHorizontalViews);
        if (strcmp(command, "-off_h") == 0) fscanf(parametersFilepointer, "%d", &firstHorizontalViewNumber);
        if (strcmp(command, "-off_v") == 0) fscanf(parametersFilepointer, "%d", &firstVerticalViewNumber);
        if (strcmp(command, "-lenslet13x13") == 0) isLenslet13x13 = 1;
        if (strcmp(command, "-extension_repeat") == 0) extensionMethod = REPEAT_LAST;
        if (strcmp(command, "-extension_dct_ext") == 0) extensionMethod = EXTENDED_DCT;
        if (strcmp(command, "-extension_dct_zero") == 0) extensionMethod = TRANSFORM_DOMAIN_ZERO_PADDING;
        if (strcmp(command, "-extension_zero") == 0) extensionMethod = ZERO_PADDING;
        if (strcmp(command, "-extension_mean") == 0) extensionMethod = MEAN_VALUE;
        if (strcmp(command, "-extension_none") == 0) extensionMethod = NONE;
        if (strcmp(command, "-extension_cyclic") == 0) extensionMethod = CYCLIC;
        if (strcmp(command, "-t_scale") == 0) fscanf(parametersFilepointer, "%lf", &transform_scale_t);
        if (strcmp(command, "-s_scale") == 0) fscanf(parametersFilepointer, "%lf", &transform_scale_s);
        if (strcmp(command, "-v_scale") == 0) fscanf(parametersFilepointer, "%lf", &transform_scale_v);
        if (strcmp(command, "-u_scale") == 0) fscanf(parametersFilepointer, "%lf", &transform_scale_u);
        if (strcmp(command, "-v0") == 0) verbosity = 0;
        if (strcmp(command, "-min_u") == 0) fscanf(parametersFilepointer, "%d", &min_transformLength_u);
        if (strcmp(command, "-min_v") == 0) fscanf(parametersFilepointer, "%d", &min_transformLength_v);
        if (strcmp(command, "-min_s") == 0) fscanf(parametersFilepointer, "%d", &min_transformLength_s);
        if (strcmp(command, "-min_t") == 0) fscanf(parametersFilepointer, "%d", &min_transformLength_t);
        fscanf(parametersFilepointer, "%s", command);
    }
}

void EncoderParameters::DisplayConfiguration(void) {
    // DSC begin
    printf("Prediction = %s\n", Prediction);
    printf("SuperiorBitPlane = %d\n", SuperiorBitPlane);
    printf("InferiorBitPlane = %d\n", InferiorBitPlane);
    printf("EvaluateOptimumBitPlane = %d\n", EvaluateOptimumBitPlane);
    printf("Split = %d\n", Split);
    printf("Perform RDO: %d\n", PerformRDO);
    // DSC end
    printf("Lambda = %f\n", Lambda);
    printf("transformLength_t = %d\n", transformLength_t);
    printf("transformLength_s = %d\n", transformLength_s);
    printf("transformLength_v = %d\n", transformLength_v);
    printf("transformLength_u = %d\n", transformLength_u);
    printf("min_transformLength_t = %d\n", min_transformLength_t);
    printf("min_transformLength_s = %d\n", min_transformLength_s);
    printf("min_transformLength_v = %d\n", min_transformLength_v);
    printf("min_transformLength_u = %d\n", min_transformLength_u);
    printf("inputNumberOfVerticalViews = %d\n", inputNumberOfVerticalViews);
    printf("inputNumberOfHorizontalViews = %d\n", inputNumberOfHorizontalViews);
    printf("firstHorizontalViewNumber = %d\n", firstHorizontalViewNumber);
    printf("firstVerticalViewNumber = %d\n", firstVerticalViewNumber);
    printf("isLenslet13x13 = %d\n", isLenslet13x13);
    printf("inputDirectory = %s\n", inputDirectory);
    printf("outputFileName = %s\n", outputFileName);
    printf("configurationFileName = %s\n", configurationFileName);
    printf("extensionMethod = %d\n", extensionMethod);
    printf("transform_scale_t = %f\n", transform_scale_t);
    printf("transform_scale_s = %f\n", transform_scale_s);
    printf("transform_scale_v = %f\n", transform_scale_v);
    printf("transform_scale_u = %f\n", transform_scale_u);
    printf("verbosity = %d\n", verbosity);
}

int main(int argc, char **argv) {

    EncoderParameters par;

    //Set default parameters
    // DSC begin
    strcpy(par.Prediction, "mule");
    par.EvaluateOptimumBitPlane = 1;
    par.Split = 1;
    par.SuperiorBitPlane = 30;
    par.InferiorBitPlane = 0;
    par.PerformRDO = 1;
    // DSC end
    par.Lambda = 1024;
    par.transformLength_t = 13;
    par.transformLength_s = 13;
    par.transformLength_v = 15;
    par.transformLength_u = 15;
    par.min_transformLength_t = 4;
    par.min_transformLength_s = 4;
    par.min_transformLength_v = 4;
    par.min_transformLength_u = 4;
    par.inputNumberOfVerticalViews = 13;
    par.inputNumberOfHorizontalViews = 13;
    par.firstHorizontalViewNumber = 0;
    par.firstVerticalViewNumber = 0;
    par.isLenslet13x13 = 0;

    strcpy(par.inputDirectory, "./");
    strcpy(par.outputFileName, "out.comp");
    strcpy(par.configurationFileName, "");

    par.extensionMethod = REPEAT_LAST;

    par.transform_scale_t = 1.0;
    par.transform_scale_s = 1.0;
    par.transform_scale_v = 1.0;
    par.transform_scale_u = 1.0;

    par.verbosity = 1;

    for (int n = 0; n < argc; n++) {
        if (strcmp(argv[n], "-cf") == 0) {
            strcpy(par.configurationFileName, argv[n + 1]);
            par.ReadConfigurationFile(par.configurationFileName);
        }
    }
    for (int n = 0; n < argc; n++) {
        // DSC begin
        if (strcmp(argv[n], "-split") == 0) {
            par.Split = atoi(argv[n + 1]);
        }
        if (strcmp(argv[n], "-evaluateOptimumBitPlane") == 0) {
            par.EvaluateOptimumBitPlane = atoi(argv[n + 1]);
        }
        if (strcmp(argv[n], "-superiorBitPlane") == 0) {
            par.SuperiorBitPlane = atoi(argv[n + 1]);
        }
        if (strcmp(argv[n], "-inferiorBitPlane") == 0) {
            par.InferiorBitPlane = atoi(argv[n + 1]);
        }
        if (strcmp(argv[n], "-prediction") == 0) {
            strcpy(par.Prediction, argv[n + 1]);
        }
        if (strcmp(argv[n], "-performRDO") == 0) {
            par.PerformRDO = atoi(argv[n + 1]);
        }
        // DSC end
        if (strcmp(argv[n], "-lambda") == 0) {
            par.Lambda = atof(argv[n + 1]);
        }
        if (strcmp(argv[n], "-l") == 0) {
            par.transformLength_u = atoi(argv[n + 1]);
            par.transformLength_v = atoi(argv[n + 1]);
        }
        if (strcmp(argv[n], "-u") == 0) par.transformLength_u = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-v") == 0) par.transformLength_v = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-k") == 0) {
            par.transformLength_t = atoi(argv[n + 1]);
            par.transformLength_s = atoi(argv[n + 1]);
        }
        if (strcmp(argv[n], "-s") == 0) par.transformLength_s = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-t") == 0) par.transformLength_t = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-lf") == 0) strcpy(par.inputDirectory, argv[n + 1]);
        if (strcmp(argv[n], "-o") == 0) strcpy(par.outputFileName, argv[n + 1]);
        if (strcmp(argv[n], "-nv") == 0) par.inputNumberOfVerticalViews = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-nh") == 0) par.inputNumberOfHorizontalViews = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-off_h") == 0) par.firstHorizontalViewNumber = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-off_v") == 0) par.firstVerticalViewNumber = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-lenslet13x13") == 0) par.isLenslet13x13 = 1;
        if (strcmp(argv[n], "-extension_repeat") == 0) par.extensionMethod = REPEAT_LAST;
        if (strcmp(argv[n], "-extension_dct_ext") == 0) par.extensionMethod = EXTENDED_DCT;
        if (strcmp(argv[n], "-extension_dct_zero") == 0) par.extensionMethod = TRANSFORM_DOMAIN_ZERO_PADDING;
        if (strcmp(argv[n], "-extension_zero") == 0) par.extensionMethod = ZERO_PADDING;
        if (strcmp(argv[n], "-extension_mean") == 0) par.extensionMethod = MEAN_VALUE;
        if (strcmp(argv[n], "-extension_none") == 0) par.extensionMethod = NONE;
        if (strcmp(argv[n], "-extension_cyclic") == 0) par.extensionMethod = CYCLIC;
        if (strcmp(argv[n], "-t_scale") == 0) par.transform_scale_t = atof(argv[n + 1]);
        if (strcmp(argv[n], "-s_scale") == 0) par.transform_scale_s = atof(argv[n + 1]);
        if (strcmp(argv[n], "-v_scale") == 0) par.transform_scale_v = atof(argv[n + 1]);
        if (strcmp(argv[n], "-u_scale") == 0) par.transform_scale_u = atof(argv[n + 1]);
        if (strcmp(argv[n], "-v0") == 0) par.verbosity = 0;
        if (strcmp(argv[n], "-min_u") == 0) par.min_transformLength_u = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-min_v") == 0) par.min_transformLength_v = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-min_s") == 0) par.min_transformLength_s = atoi(argv[n + 1]);
        if (strcmp(argv[n], "-min_t") == 0) par.min_transformLength_t = atoi(argv[n + 1]);
    }

    if (par.min_transformLength_t > par.transformLength_t)
        par.min_transformLength_t = par.transformLength_t;
    if (par.min_transformLength_s > par.transformLength_s)
        par.min_transformLength_s = par.transformLength_s;
    if (par.min_transformLength_v > par.transformLength_v)
        par.min_transformLength_v = par.transformLength_v;
    if (par.min_transformLength_u > par.transformLength_u)
        par.min_transformLength_u = par.transformLength_u;

    par.DisplayConfiguration();

    int numberOfCacheViewLines = par.transformLength_v;
    LightField inputLF(par.inputNumberOfVerticalViews, par.inputNumberOfHorizontalViews, numberOfCacheViewLines);

    Block4D lfBlock, rBlock, gBlock, bBlock, yBlock, cbBlock, crBlock;
    // DSC begin
    //Hierarchical4DEncoder hdt;
    Hierarchical4DEncoder hdt(par.SuperiorBitPlane, par.PerformRDO);
    //TransformPartition tp;
    double energyRefPlane = 0;
    double energyOtherPlanes = 0;
    Block4D lfBlockResidue;
    //EDUARDO begin
    Block4D lfBlockResidueRaster, lfBlockResidueCentral;
    //lfBlockResidueRasterDC, lfBlockResidueCentralDC;
    double JminRaster = 0;
    double JminCentral = 0;
    double JminMuLE = 0;
    //double JminRasterDC = 0;
    //double JminCentralDC = 0;
    //EDUARDO end
    Block4D yOrigBlock, crOrigBlock, cbOrigBlock;
    int predictionType = 2;
    /*
    if (strcmp(par.Prediction, "diffR") == 0) {
        predictionType = 0;
    } else if (strcmp(par.Prediction, "diffC") == 0) {
        predictionType = 1;
    } else if (strcmp(par.Prediction, "mule") == 0) {
        predictionType = 2;
    } else if (strcmp(par.Prediction, "diffRDC") == 0) {
        predictionType = 3;
    } else if (strcmp(par.Prediction, "diffCDC") == 0) {
        predictionType = 4;
    }
    */
    TransformPartition tp(predictionType, par.InferiorBitPlane, par.EvaluateOptimumBitPlane, par.Split);

    Prediction pred(predictionType);
    Statistics stats(predictionType);
    ofstream DCPredictorFile;
    DCPredictorFile.open("DC_predictors.txt");
    // DSC end
    //EDUARDO begin
    ofstream predictionFile("prediction.txt");
    //EDUARDO end

    tp.mPartitionData.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);

    lfBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    rBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    gBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    bBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    yBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    cbBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    crBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);

    // DSC begin
    /* initialize 4D blocks */
    lfBlockResidue.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    yOrigBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    cbOrigBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    crOrigBlock.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    // DSC end

    //EDUARDO begin
    lfBlockResidueRaster.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    lfBlockResidueCentral.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    //lfBlockResidueRasterDC.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    //lfBlockResidueCentralDC.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u);
    //EDUARDO end

    inputLF.mVerticalViewNumberOffset = par.firstVerticalViewNumber;
    inputLF.mHorizontalViewNumberOffset = par.firstHorizontalViewNumber;
    inputLF.OpenLightFieldPPM(par.inputDirectory, ".ppm", par.inputNumberOfVerticalViews, par.inputNumberOfHorizontalViews, 3, 3, 'r');

    int extensionLength_t = par.inputNumberOfVerticalViews % par.transformLength_t;
    int extensionLength_s = par.inputNumberOfHorizontalViews % par.transformLength_s;
    int extensionLength_v = inputLF.mNumberOfViewLines % par.transformLength_v;
    int extensionLength_u = inputLF.mNumberOfViewColumns % par.transformLength_u;

    MultiscaleTransform DCTarray;

    DCTarray.SetDimension(par.transformLength_t, par.transformLength_s, par.transformLength_v, par.transformLength_u, par.min_transformLength_t, par.min_transformLength_s, par.min_transformLength_v, par.min_transformLength_u);
    DCTarray.LoadDCT();
    DCTarray.mTransformGain_t = par.transform_scale_t;
    DCTarray.mTransformGain_s = par.transform_scale_s;
    DCTarray.mTransformGain_v = par.transform_scale_v;
    DCTarray.mTransformGain_u = par.transform_scale_u;

    hdt.mTransformLength_t = par.transformLength_t;
    hdt.mTransformLength_s = par.transformLength_s;
    hdt.mTransformLength_v = par.transformLength_v;
    hdt.mTransformLength_u = par.transformLength_u;
    hdt.mMinimumTransformLength_t = par.min_transformLength_t;
    hdt.mMinimumTransformLength_s = par.min_transformLength_s;
    hdt.mMinimumTransformLength_v = par.min_transformLength_v;
    hdt.mMinimumTransformLength_u = par.min_transformLength_u;
    hdt.mNumberOfVerticalViews = inputLF.mNumberOfVerticalViews;
    hdt.mNumberOfHorizontalViews = inputLF.mNumberOfHorizontalViews;
    hdt.mNumberOfViewLines = inputLF.mNumberOfViewLines;
    hdt.mNumberOfViewColumns = inputLF.mNumberOfViewColumns;
    hdt.mPGMScale = inputLF.mPGMScale;
    hdt.StartEncoder(par.outputFileName);
    for (int verticalView = 0; verticalView < inputLF.mNumberOfVerticalViews; verticalView += par.transformLength_t) {
        for (int horizontalView = 0; horizontalView < inputLF.mNumberOfHorizontalViews; horizontalView += par.transformLength_s) {
            for (int viewLine = 0; viewLine < inputLF.mNumberOfViewLines; viewLine += par.transformLength_v) {
                for (int viewColumn = 0; viewColumn < inputLF.mNumberOfViewColumns; viewColumn += par.transformLength_u) {
                    if (par.verbosity > 0)
                        printf("transforming the 4D block at position (%d %d %d %d)\n", verticalView, horizontalView, viewLine, viewColumn);
                    rBlock.Zeros();
                    gBlock.Zeros();
                    bBlock.Zeros();
                    inputLF.ReadBlock4DfromLightField(&rBlock, verticalView, horizontalView, viewLine, viewColumn, 0);
                    inputLF.ReadBlock4DfromLightField(&gBlock, verticalView, horizontalView, viewLine, viewColumn, 1);
                    inputLF.ReadBlock4DfromLightField(&bBlock, verticalView, horizontalView, viewLine, viewColumn, 2);
                    if (par.isLenslet13x13 == 1) {
                        if (verticalView == 0) {
                            if (horizontalView == 0) {
                                rBlock.Shift_UVPlane(2, 0, 0);
                                gBlock.Shift_UVPlane(2, 0, 0);
                                bBlock.Shift_UVPlane(2, 0, 0);
                            }
                            if ((horizontalView + par.transformLength_s >= inputLF.mNumberOfHorizontalViews)&&(horizontalView <= inputLF.mNumberOfHorizontalViews)) {
                                rBlock.Shift_UVPlane(2, 0, inputLF.mNumberOfHorizontalViews - horizontalView - 1);
                                gBlock.Shift_UVPlane(2, 0, inputLF.mNumberOfHorizontalViews - horizontalView - 1);
                                bBlock.Shift_UVPlane(2, 0, inputLF.mNumberOfHorizontalViews - horizontalView - 1);
                            }
                        }
                        if ((verticalView + par.transformLength_t >= inputLF.mNumberOfVerticalViews)&&(verticalView <= inputLF.mNumberOfVerticalViews)) {
                            if (horizontalView == 0) {
                                rBlock.Shift_UVPlane(2, inputLF.mNumberOfVerticalViews - verticalView - 1, 0);
                                gBlock.Shift_UVPlane(2, inputLF.mNumberOfVerticalViews - verticalView - 1, 0);
                                bBlock.Shift_UVPlane(2, inputLF.mNumberOfVerticalViews - verticalView - 1, 0);
                            }
                            if ((horizontalView + par.transformLength_s >= inputLF.mNumberOfHorizontalViews)&&(horizontalView <= inputLF.mNumberOfHorizontalViews)) {
                                rBlock.Shift_UVPlane(2, inputLF.mNumberOfVerticalViews - verticalView - 1, inputLF.mNumberOfHorizontalViews - horizontalView - 1);
                                gBlock.Shift_UVPlane(2, inputLF.mNumberOfVerticalViews - verticalView - 1, inputLF.mNumberOfHorizontalViews - horizontalView - 1);
                                bBlock.Shift_UVPlane(2, inputLF.mNumberOfVerticalViews - verticalView - 1, inputLF.mNumberOfHorizontalViews - horizontalView - 1);
                            }
                        }
                    }

                    RGB2YCbCr_BT601(yOrigBlock, cbOrigBlock, crOrigBlock, rBlock, gBlock, bBlock, inputLF.mPGMScale);

                    for (int spectralComponent = 0; spectralComponent < 3; spectralComponent++) {
                        //printf("\tProcessing spectral component %d\n", spectralComponent);
                        if (spectralComponent == 0)
                            lfBlock.CopySubblockFrom(yOrigBlock, 0, 0, 0, 0);
                        if (spectralComponent == 1)
                            lfBlock.CopySubblockFrom(cbOrigBlock, 0, 0, 0, 0);
                        if (spectralComponent == 2)
                            lfBlock.CopySubblockFrom(crOrigBlock, 0, 0, 0, 0);

                        lfBlock = lfBlock - (inputLF.mPGMScale + 1) / 2;
                        if (viewColumn + par.transformLength_u > inputLF.mNumberOfViewColumns)
                            ExtendBlock4D(lfBlock, par.extensionMethod, extensionLength_u, 'u');
                        if (viewLine + par.transformLength_v > inputLF.mNumberOfViewLines)
                            ExtendBlock4D(lfBlock, par.extensionMethod, extensionLength_v, 'v');
                        if (horizontalView + par.transformLength_s > inputLF.mNumberOfHorizontalViews)
                            ExtendBlock4D(lfBlock, par.extensionMethod, extensionLength_s, 's');
                        if (verticalView + par.transformLength_t > inputLF.mNumberOfVerticalViews)
                            ExtendBlock4D(lfBlock, par.extensionMethod, extensionLength_t, 't');
                        
                        /*

                        // DSC begin
                        if (strcmp(par.Prediction, "diffR") == 0) {
                            //printf("\tPrediction: %s\n", par.Prediction);
                            pred.differentialPredictionRaster(&lfBlockResidue, &lfBlock, spectralComponent);
                        } else if (strcmp(par.Prediction, "diffC") == 0) {
                            //printf("\tPrediction: %s\n", par.Prediction);
                            pred.differentialPredictionCentral(&lfBlockResidue, &lfBlock, spectralComponent);
                        } else if (strcmp(par.Prediction, "mule") == 0) {
                            //printf("\tPrediction: %s\n", par.Prediction);
                            pred.saveSamplesMule(&lfBlockResidue, &lfBlock, spectralComponent);
                        } else if (strcmp(par.Prediction, "diffRDC") == 0) {
                            pred.differentialPredictionRasterDCRefPlane(&lfBlockResidue, &lfBlock, spectralComponent);

                            for (int vView = 0; vView < 13; vView++) {
                                int DCPredictor = pred.DCPredictorRefPlaneRaster(&lfBlock, vView);
                                DCPredictorFile << DCPredictor << '\n';
                            }
                        } else if (strcmp(par.Prediction, "diffCDC") == 0) {
                            pred.differentialPredictionCentralDCRefPlane(&lfBlockResidue, &lfBlock, spectralComponent);

                            for (int vView = 0; vView < 13; vView++) {
                                int DCPredictor = pred.DCPredictorRefPlaneRaster(&lfBlock, vView);
                                DCPredictorFile << DCPredictor << '\n';
                            }
                        }
                        */

                        //EDUARDO begin
                        pred.differentialPredictionRaster(&lfBlockResidueRaster, &lfBlock, spectralComponent);
                        tp.RDoptimizeTransformJmin(lfBlockResidueRaster, DCTarray, hdt, par.Lambda, &JminRaster);

                        pred.differentialPredictionCentral(&lfBlockResidueCentral, &lfBlock, spectralComponent);
                        tp.RDoptimizeTransformJmin(lfBlockResidueCentral, DCTarray, hdt, par.Lambda, &JminCentral);

                        pred.saveSamplesMule(&lfBlockResidue, &lfBlock, spectralComponent);
                        tp.RDoptimizeTransformJmin(lfBlockResidue, DCTarray, hdt, par.Lambda, &JminMuLE);
                        
                        /*
                        pred.differentialPredictionRasterDCRefPlane(&lfBlockResidueRasterDC, &lfBlock, spectralComponent);
                        tp.RDoptimizeTransformJmin(lfBlockResidueRasterDC, DCTarray, hdt, par.Lambda, &JminRasterDC);

                        pred.differentialPredictionCentralDCRefPlane(&lfBlockResidueCentralDC, &lfBlock, spectralComponent);
                        tp.RDoptimizeTransformJmin(lfBlockResidueCentralDC, DCTarray, hdt, par.Lambda, &JminCentralDC);
                        */
                        
                        //EDUARDO end

                        printf("MuLE: %lf\n", JminMuLE);
                        printf("Raster: %lf\n", JminRaster);
                        printf("Central: %lf\n", JminCentral);
                        //printf("RasterDC: %lf\n", JminRasterDC);
                        //printf("CentralDC: %lf\n\n", JminCentralDC);
                        
                        
                        //EDUARDO begin
                        if (JminRaster < JminCentral && JminRaster < JminMuLE) {
                            predictionFile << '0' << endl;
                            tp.RDoptimizeTransform(lfBlockResidueRaster, DCTarray, hdt, par.Lambda, &stats);
                            lfBlockResidue = lfBlockResidueRaster;
                        } else if (JminCentral < JminRaster && JminCentral < JminMuLE) {
                            predictionFile << '1' << endl;
                            tp.RDoptimizeTransform(lfBlockResidueCentral, DCTarray, hdt, par.Lambda, &stats);
                            lfBlockResidue = lfBlockResidueCentral;
                        } else {
                            predictionFile << '2' << endl;
                            tp.RDoptimizeTransform(lfBlockResidue, DCTarray, hdt, par.Lambda, &stats);
                        }
                        
                        /*else if (JminRasterDC < JminCentral && JminRasterDC < JminMuLE && JminRasterDC <= JminRaster && JminRasterDC < JminCentralDC) {
                            predictionFile << '2' << endl;
                            tp.RDoptimizeTransform(lfBlockResidueRasterDC, DCTarray, hdt, par.Lambda, &stats);
                            lfBlockResidue = lfBlockResidueRasterDC;
                            
                            for (int vView = 0; vView < 13; vView++) {
                                int DCPredictor = pred.DCPredictorRefPlaneRaster(&lfBlock, vView);
                                DCPredictorFile << DCPredictor << '\n';
                            }
                        } else if (JminCentralDC <= JminCentral && JminCentralDC < JminMuLE && JminCentralDC < JminRaster && JminCentralDC < JminRasterDC) {
                            predictionFile << '3' << endl;
                            tp.RDoptimizeTransform(lfBlockResidueCentralDC, DCTarray, hdt, par.Lambda, &stats);
                            lfBlockResidue = lfBlockResidueCentralDC;
                            
                            for (int vView = 0; vView < 13; vView++) {
                                int DCPredictor = pred.DCPredictorRefPlaneRaster(&lfBlock, vView);
                                DCPredictorFile << DCPredictor << '\n';
                            }
                        } */
                        
                        
                        if (spectralComponent == 0) {
                            stats.calcSumYRefPlaneSamples(&lfBlockResidue);
                            stats.calcSumYOtherPlanesSamples(&lfBlockResidue);
                        }
                        if (spectralComponent == 1) {
                            stats.calcSumCbRefPlaneSamples(&lfBlockResidue);
                            stats.calcSumCbOtherPlanesSamples(&lfBlockResidue);
                        }
                        if (spectralComponent == 2) {
                            stats.calcSumCrRefPlaneSamples(&lfBlockResidue);
                            stats.calcSumCrOtherPlanesSamples(&lfBlockResidue);
                        }

                        stats.calcReferencePlaneEnergy(&lfBlockResidue, spectralComponent);
                        stats.calcOtherPlanesEnergy(&lfBlockResidue, spectralComponent);
                        // DSC end
                        
                        //tp.RDoptimizeTransform(lfBlockResidue, DCTarray, hdt, par.Lambda, &stats);
                        
                        //EDUARDO end
                        
                        tp.EncodePartition(hdt, par.Lambda, &stats);

                    }
                }
            }
        }
    }

    hdt.DoneEncoding();

    inputLF.CloseLightField();

    // DSC begin
    printf("\nSUMMARY ----------------------------------------------------------------------\n");
    printf("\tPrediction: %s\n", par.Prediction);
    //printf("\tLambda: %.0lf\n", par.Lambda);
    printf("\t\tALL PLANES ----------------------------------------------------\n");
    printf("\t\t\tNumber of partitionings: %d\n", stats.getPartitioningCounter());
    printf("\t\t\tPercentage of partitioned blocks: %.2f%%\n", ((float) stats.getPartitioningCounter() / (42.0 * 29.0 * 3.0))*100);
    printf("\t\tREFERENCE PLANE RESIDUES --------------------------------------\n");
    printf("\t\t\tY energy: %.0lf\n", stats.getYRefPlaneEnergy());
    printf("\t\t\tCb energy: %.0lf\n", stats.getCbRefPlaneEnergy());
    printf("\t\t\tCr energy: %.0lf\n", stats.getCrRefPlaneEnergy());
    printf("\t\t\tY average: %.2lf\n", stats.getYRefPlaneSamplesAverage());
    printf("\t\t\tCb average: %.2lf\n", stats.getCbRefPlaneSamplesAverage());
    printf("\t\t\tCr average: %.2lf\n", stats.getCrRefPlaneSamplesAverage());
    printf("\t\t\tY standard deviation: %.2lf\n", stats.calcStdYRefPlane());
    printf("\t\t\tCb standard deviation: %.2lf\n", stats.calcStdCbRefPlane());
    printf("\t\t\tCr standard deviation: %.2lf\n", stats.calcStdCrRefPlane());
    printf("\t\t\tMax value: %d\n", stats.getMaxRefPlane());
    printf("\t\t\tMin value: %d\n", stats.getMinRefPlane());
    printf("\t\tOTHER PLANES RESIDUES -----------------------------------------\n");
    printf("\t\t\tY energy: %.0lf\n", stats.getYOtherPlanesEnergy());
    printf("\t\t\tCb energy: %.0lf\n", stats.getCbOtherPlanesEnergy());
    printf("\t\t\tCr energy: %.0lf\n", stats.getCrOtherPlanesEnergy());
    printf("\t\t\tY average: %.2lf\n", stats.getYOtherPlanesSamplesAverage());
    printf("\t\t\tCb average: %.2lf\n", stats.getCbOtherPlanesSamplesAverage());
    printf("\t\t\tCr average: %.2lf\n", stats.getCrOtherPlanesSamplesAverage());
    printf("\t\t\tY standard deviation: %.2lf\n", stats.calcStdYOtherPlanes());
    printf("\t\t\tCb standard deviation: %.2lf\n", stats.calcStdCbOtherPlanes());
    printf("\t\t\tCr standard deviation: %.2lf\n", stats.calcStdCrOtherPlanes());
    printf("\t\t\tMax value: %d\n", stats.getMaxOtherPlanes());
    printf("\t\t\tMin value: %d\n", stats.getMinOtherPlanes());
    printf("\t\tCOEFFICIENTS --------------------------------------------------\n");
    printf("\t\t\tEnergy: %.2lf\n", stats.getEnergyCoeff());
    printf("\t\t\tZero coefficients: %.0lf\n", stats.getZeroCoefficients());
    printf("\t\t\tAll coefficients: %.0lf\n", stats.getTotalCoefficients());
    printf("\t\t\tMax coefficient: %d\n", stats.getMaxCoeff());
    printf("\t\t\tMin coefficient: %d\n", stats.getMinCoeff());
    printf("\t\t\tCoefficients per BitPlane:\n");
    stats.getCoeffsPerBitPlane();
    printf("\t\tHEXADECA TREE --------------------------------------------------\n");
    printf("\t\t\tNumber of Bc decrease: %d\n", stats.getBcDecreaseCounter());
    printf("\t\t\tNumber of Splits: %d\n", stats.getSplitHexaDecaTreeCounter());
    //predictionFile.close();
    // DSC end

}

void ExtendDCT(Matrix &extendedDCT, ExtensionType extensionMethod, int transformLength, int extensionLength) {

    extendedDCT.SetDimension(transformLength, transformLength);
    if (extensionMethod == EXTENDED_DCT) {
        Matrix C00, C01, C10, C11, INVC11;
        extendedDCT.DCT();
        C00.SetDimension(extensionLength, extensionLength);
        C01.SetDimension(extensionLength, transformLength - extensionLength);
        C10.SetDimension(transformLength - extensionLength, extensionLength);
        C11.SetDimension(transformLength - extensionLength, transformLength - extensionLength);
        INVC11.SetDimension(transformLength - extensionLength, transformLength - extensionLength);
        C00.CopyFrom(extendedDCT);
        C01.CopyFrom(extendedDCT, 0, extensionLength);
        C10.CopyFrom(extendedDCT, extensionLength, 0);
        C11.CopyFrom(extendedDCT, extensionLength, extensionLength);
        INVC11.Inverse(C11);
        INVC11.Multiply(-1.0);
        C10.PreMultiply(INVC11); //C10 <- - INVC11 * C10
        C10.PreMultiply(C01); //C10 <- - C01 * INVC11 * C10
        C00.Add(C10); //C00 <- C00 - C01 * INVC11 * C10
        extendedDCT.Zeros();
        extendedDCT.CopyFrom(C00);
    }
    if (extensionMethod == REPEAT_LAST) {
        extendedDCT.DCT();
        extendedDCT.AccumulateFromColumn(extensionLength - 1);
    }
    if (extensionMethod == TRANSFORM_DOMAIN_ZERO_PADDING) {
        Matrix C00;
        C00.SetDimension(extensionLength, extensionLength);
        C00.DCT();
        C00.Multiply((1.0 * transformLength) / extensionLength);
        extendedDCT.Zeros();
        extendedDCT.CopyFrom(C00);
    }
    if (extensionMethod == ZERO_PADDING) {
        Matrix C00;
        C00.SetDimension(transformLength, extensionLength);
        extendedDCT.DCT();
        C00.CopyFrom(extendedDCT);
        extendedDCT.Zeros();
        extendedDCT.CopyFrom(C00);
    }
    if (extensionMethod == NONE) {
        extendedDCT.DCT();
    }

}

void ExtendBlock4D(Block4D &extendedBlock, ExtensionType extensionMethod, int extensionLength, char direction) {

    if (extensionMethod == REPEAT_LAST) {

        if (direction == 't')
            extendedBlock.Extend_T(extensionLength - 1);
        if (direction == 's')
            extendedBlock.Extend_S(extensionLength - 1);
        if (direction == 'v')
            extendedBlock.Extend_V(extensionLength - 1);
        if (direction == 'u')
            extendedBlock.Extend_U(extensionLength - 1);

    }
    if (extensionMethod == CYCLIC) {

        if (direction == 't')
            extendedBlock.CopySubblockFrom(extendedBlock, 0, 0, 0, 0, extensionLength, 0, 0, 0);
        if (direction == 's')
            extendedBlock.CopySubblockFrom(extendedBlock, 0, 0, 0, 0, 0, extensionLength, 0, 0);
        if (direction == 'v')
            extendedBlock.CopySubblockFrom(extendedBlock, 0, 0, 0, 0, 0, 0, extensionLength, 0);
        if (direction == 'u')
            extendedBlock.CopySubblockFrom(extendedBlock, 0, 0, 0, 0, 0, 0, 0, extensionLength);

    }
    if (extensionMethod == NONE) {

    }
}

void RGB2YCbCr_BT601(Block4D &Y, Block4D &Cb, Block4D &Cr, Block4D const &R, Block4D const &G, Block4D const &B, int Scale) {

    for (int n = 0; n < R.mlength_t * R.mlength_s * R.mlength_v * R.mlength_u; n++) {
        double pixel = 0.299 * R.mPixelData[n] + 0.587 * G.mPixelData[n] + 0.114 * B.mPixelData[n];
        Y.mPixelData[n] = pixel;
        pixel = -0.16875 * R.mPixelData[n] - 0.33126 * G.mPixelData[n] + 0.5 * B.mPixelData[n];
        Cb.mPixelData[n] = pixel + (Scale + 1) / 2;
        pixel = 0.5 * R.mPixelData[n] - 0.41869 * G.mPixelData[n] - 0.08131 * B.mPixelData[n];
        Cr.mPixelData[n] = pixel + (Scale + 1) / 2;
    }

}