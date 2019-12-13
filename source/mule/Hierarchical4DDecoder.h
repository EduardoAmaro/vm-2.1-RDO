/* 
 * File:   Hierarchical4DDecoder.h
 * Author: murilo
 *
 * Created on December 28, 2017, 11:41 AM
 */
#include "Block4D.h"
#include "Aritmetico.h"
#include <stdlib.h>
#include <stdio.h>

// DSC begin
#include <fstream>

using namespace std;
// DSC end

#ifndef HIERARCHICAL4DDECODER_H
#define HIERARCHICAL4DDECODER_H

#define MAX_DEPH_CONDICIONING 9
#define BITPLANE_BYPASS -1
#define BITPLANE_BYPASS_FLAGS -1

class Hierarchical4DDecoder {
public:
	// DSC begin
	ofstream quantizedCoefficients;
	// DSC end
    Block4D mSubbandLF;
    ArithmeticDecoder mEntropyDecoder;
    int mSuperiorBitPlane, mInferiorBitPlane;
    int mTransformLength_u, mTransformLength_v,mTransformLength_s, mTransformLength_t;
    int mMinimumTransformLength_u, mMinimumTransformLength_v,mMinimumTransformLength_s, mMinimumTransformLength_t;
    int mNumberOfVerticalViews, mNumberOfHorizontalViews;
    int mNumberOfViewLines, mNumberOfViewColumns;
    int mPGMScale;
    int mDCArithmeticModelIndex;
    int mACArithmeticModelIndex;
    int mFlagsArithmeticModelIndex;
    int mSymbolsArithmeticModelIndex;
    int mPreSegmentation;
    Hierarchical4DDecoder(void);
    ~Hierarchical4DDecoder(void);
    void GetFromSubBand4D(Block4D &B, int sbb_t, int sbb_s, int sbb_v, int sbb_u);
    void StartDecoder(char *inputFile);
    void RestartProbabilisticModel(void);
    void DecodeBlock(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, int bitplane);
    int DecodeCoefficient(int bitplane);
    int DecodeTernaryFlag(int base_arithmetic_model_index, int adapt);
    int DecodePartitionSymbol(void);
    void DoneDecoding(void);
    int SubbandNumber(int position_t, int position_s, int position_v, int position_u);
};
#endif /* HIERARCHICAL4DDECODER_H */

