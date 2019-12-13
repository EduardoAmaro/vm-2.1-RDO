/* 
 * File:   Hierarchical4DEncoder.h
 * Author: murilo
 *
 * Created on December 28, 2017, 11:41 AM
 */
#include "Block4D.h"
#include "Aritmetico.h"

#ifndef HIERARCHICAL4DENCODER_H
#define HIERARCHICAL4DENCODER_H

//#define MAX_DEPH_CONDICIONING 9
#define BITPLANE_BYPASS -1
#define BITPLANE_BYPASS_FLAGS -1

// DSC begin
#include <iostream>
#include <fstream>
#include "Statistics.h"

using namespace std;
// DSC end

class Hierarchical4DEncoder {
public:
	// DSC begin
	int counter, mPerformRDO;
	long long int signalTotalEnergy;
	ofstream magnitudeFile;
	// DSC end
    Block4D mSubbandLF;   
    ArithmeticCoder mEntropyCoder;
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
    int mFindBitplaneArithmeticModelIndex;
    int mFindBestTreeArithmeticModelIndex;
    int mFindBestTreeFlagsArithmeticModelIndex;
    int mPreSegmentation;
    char *mSegmentationTreeCodeBuffer;
    long int mSegmentationTreeCodeBufferSize;
	// DSC begin
	Hierarchical4DEncoder(int superiorBitPlane, int performRDO);
	//Hierarchical4DEncoder(void);
	void EncodeSubblock(double lambda, Statistics *stats);
	//void EncodeSubblock(double lambda);
	double RdOptimizeHexadecaTree(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, double lambda, int bitplane, char **codeString, double &signalEnergy, Statistics *stats);
	//double RdOptimizeHexadecaTree(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, double lambda, int bitplane, char **codeString, double &signalEnergy);
	// DSC end
    ~Hierarchical4DEncoder(void);
    void LoadToSubBand4D(const Block4D &B, int sbb_t, int sbb_s, int sbb_v, int sbb_u);
    void StartEncoder(char *outputFile);
    void RestartProbabilisticModel(void);
    void EncodeBlock(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, int bitplane);
    void EncodeCoefficient(int coefficient, int bitplane, int arithmetic_model_index);
    void EncodeTernaryFlag(int flag, int base_arithmetic_model_index, int adapt);
    void EncodeAll(double lambda, int inferiorBitPlane);
    void RdEncodeHexadecatree(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, int bitplane, int &flagIndex);
    void RdEncodeCoefficient(int coefficient, int bitplane, int &flagIndex);
    void DoneEncoding(void);
    int SubbandNumber(int position_t, int position_s, int position_v, int position_u);
    void SetDimension(int length_t, int length_s, int length_v, int length_u);
    int OptimumBitplane(double lambda);
    //EDUARDO begin
    double RDCost(double lambda);
    //EDUARDO end
    void GetOptimizerProbabilisticModelState(PModel **state);
    void SetOptimizerProbabilisticModelState(PModel *state);
    void DeleteProbabilisticModelState(PModel *state);
    void EncodePartitionSymbol(int symbol);
};
#endif /* HIERARCHICAL4DENCODER_H */

