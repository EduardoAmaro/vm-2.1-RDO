#include "MultiscaleTransform.h"
#include "Hierarchical4DDecoder.h"
#include <math.h>
#include <string.h>

#ifndef TRANSFORMPARTITION_H
#define TRANSFORMPARTITION_H

#define NOSPLITFLAG 'T'
#define INTRAVIEWSPLITFLAG 'S'
#define INTERVIEWSPLITFLAG 'V'

class PartitionDecoder {
public:  
    char *mPartitionCode;               /*!< String of flags defining the partition tree */
    int mPartitionCodeIndex;            /*!< Scan index for the partition tree code string */
    int mPartitionCodeMaxLength;        /*!< Maximum length of the partition tree code string */
    int mEvaluateOptimumBitPlane;       /*!< Toggles the optimum bit plane evaluation procedure on and off */
    int mUseSameBitPlane;               /*!< Forces to use the same minimum bitplane for all subblocks */
    Block4D mPartitionData;             /*!< DCT of all subblocks of the partition */
    PartitionDecoder(void);
    ~PartitionDecoder(void);
    void DecodePartition(Hierarchical4DDecoder &entropyDecoder, MultiscaleTransform &mt);
    void DecodePartitionStep(int *position, int *length, Hierarchical4DDecoder &entropyDecoder, MultiscaleTransform &mt);
    void DecodePartitionString(Hierarchical4DDecoder &entropyDecoder);
    void DecodePartitionStringStep(Hierarchical4DDecoder &entropyDecoder);
};

#endif /* TRANSFORMOPTIMIZATION_H */

