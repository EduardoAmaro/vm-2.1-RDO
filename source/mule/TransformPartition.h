#include "MultiscaleTransform.h"
#include "Hierarchical4DEncoder.h"
#include <math.h>
#include <string.h>

// DSC begin
#include "Prediction.h"
//#include "Statistics.h"
// DSC end

#ifndef TRANSFORMPARTITION_H
#define TRANSFORMPARTITION_H

#define NOSPLITFLAG 'T'
#define INTRAVIEWSPLITFLAG 'S'
#define INTERVIEWSPLITFLAG 'V'

class TransformPartition {
public:
    char *mPartitionCode;               /*!< String of flags defining the partition tree */
    int mPartitionCodeIndex;            /*!< Scan index for the partition tree code string */
    double mLagrangianCost;             /*!< Lagrangian cost of the chosen partition */
    int mEvaluateOptimumBitPlane;       /*!< Toggles the optimum bit plane evaluation procedure on and off */
    int mUseSameBitPlane;               /*!< Forces to use the same minimum bitplane for all subblocks */
    Block4D mPartitionData;             /*!< DCT of all subblocks of the partition */
	
	// DSC begin
	int mInferiorBitPlane;
	ofstream partition_code_file;
    //TransformPartition(void);
	int mSplit;
	TransformPartition(int predType, int inferiorBitPlane, int evaluateOptimumBitPlane, int Split);
	Prediction *pred;
    ~TransformPartition(void);
	void RDoptimizeTransform(Block4D &inputBlock, MultiscaleTransform &mt, Hierarchical4DEncoder &entropyCoder, double lambda, Statistics *stats);
	//void RDoptimizeTransform(Block4D &inputBlock, MultiscaleTransform &mt, Hierarchical4DEncoder &entropyCoder, double lambda);
	void EncodePartition(Hierarchical4DEncoder &entropyCoder, double lambda, Statistics *stats);
	//void EncodePartition(Hierarchical4DEncoder &entropyCoder, double lambda);
	void EncodePartitionStep(int *position, int *length, Hierarchical4DEncoder &entropyCoder, double lambda, Statistics *stats);
	//void EncodePartitionStep(int *position, int *length, Hierarchical4DEncoder &entropyCoder, double lambda);
	double RDoptimizeTransformStep(Block4D &inputBlock, Block4D &transformedBlock, int *position, int *length, MultiscaleTransform &mt, Hierarchical4DEncoder &entropyCoder, double lambda, char **partitionCode, Statistics *stats);
	//double RDoptimizeTransformStep(Block4D &inputBlock, Block4D &transformedBlock, int *position, int *length, MultiscaleTransform &mt, Hierarchical4DEncoder &entropyCoder, double lambda, char **partitionCode);
	// DSC end
        
        //EDUARDO begin
        void RDoptimizeTransformJmin(Block4D &inputBlock, MultiscaleTransform &mt, Hierarchical4DEncoder &entropyCoder, double lambda, double *Jmin);
        //EDUARDO end

    void EncodePartitionString(Hierarchical4DEncoder &entropyCoder);
};

#endif /* TRANSFORMOPTIMIZATION_H */

