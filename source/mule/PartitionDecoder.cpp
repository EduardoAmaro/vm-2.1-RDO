#include "PartitionDecoder.h"
#include "Aritmetico.h"

/*******************************************************************************/
/*                      PartitionDecoder class methods                    */
/*******************************************************************************/

PartitionDecoder :: PartitionDecoder(void) {
    mPartitionCode = NULL;
    mUseSameBitPlane = 1;
}
PartitionDecoder :: ~PartitionDecoder(void) {
    if(mPartitionCode != NULL)
        delete [] mPartitionCode;
}

   
void PartitionDecoder :: DecodePartition(Hierarchical4DDecoder &entropyDecoder, MultiscaleTransform &mt) {
    
    
    int position[4];
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    position[3] = 0;

    int length[4];
    length[0] = mPartitionData.mlength_t;
    length[1] = mPartitionData.mlength_s;
    length[2] = mPartitionData.mlength_v;
    length[3] = mPartitionData.mlength_u;
    
    mPartitionCodeIndex = 0;
    
    DecodePartitionString(entropyDecoder);

    mPartitionCodeIndex = 0;
    entropyDecoder.mInferiorBitPlane = entropyDecoder.mEntropyDecoder.decode_symbol(2);
	//printf("mInferiorBitPlane: %d\n", entropyDecoder.mInferiorBitPlane);
    DecodePartitionStep(position, length, entropyDecoder, mt);
        
}

void PartitionDecoder :: DecodePartitionStep(int *position, int *length, Hierarchical4DDecoder &entropyDecoder, MultiscaleTransform &mt) {

    
    if(mPartitionCode[mPartitionCodeIndex] == NOSPLITFLAG) {
 
        mPartitionCodeIndex++;
        entropyDecoder.mSubbandLF.SetDimension(length[0], length[1], length[2], length[3]);
        entropyDecoder.mSubbandLF.Zeros();
        //entropyDecoder.mSubbandLF.CopySubblockFrom(mPartitionData, position[0], position[1], position[2], position[3]);
        //entropyDecoder.DecodeAll(lambda, entropyDecoder.mInferiorBitPlane);
        entropyDecoder.DecodeBlock(0, 0, 0, 0, length[0], length[1], length[2], length[3], entropyDecoder.mSuperiorBitPlane);
        mt.InverseTransform4D(entropyDecoder.mSubbandLF);  
        mPartitionData.CopySubblockFrom(entropyDecoder.mSubbandLF, 0, 0, 0, 0, position[0], position[1], position[2], position[3]);
        return;
    }
    if(mPartitionCode[mPartitionCodeIndex] == INTRAVIEWSPLITFLAG) {
        
        mPartitionCodeIndex++;

        int new_position[4], new_length[4];
        
        new_position[0] = position[0];
        new_position[1] = position[1];
        new_position[2] = position[2];
        new_position[3] = position[3];
        
        new_length[0] = length[0];
        new_length[1] = length[1];
        new_length[2] = length[2]/2;
        new_length[3] = length[3]/2;
        
        //Decode four spatial subblocks 
        DecodePartitionStep(new_position, new_length, entropyDecoder, mt);

        new_position[3] = position[3] + length[3]/2;
        new_length[3] = length[3] - length[3]/2;
        
        DecodePartitionStep(new_position, new_length, entropyDecoder, mt);

        new_position[2] = position[2] + length[2]/2;
        new_length[2] = length[2] - length[2]/2;
        
        DecodePartitionStep(new_position, new_length, entropyDecoder, mt);
        
        new_position[3] = position[3];
        new_length[3] = length[3]/2;
        
        DecodePartitionStep(new_position, new_length, entropyDecoder, mt);
        return;
    }
    if(mPartitionCode[mPartitionCodeIndex] == INTERVIEWSPLITFLAG) {
        
        mPartitionCodeIndex++;

        int new_position[4], new_length[4];
        
        new_position[0] = position[0];
        new_position[1] = position[1];
        new_position[2] = position[2];
        new_position[3] = position[3];
        
        new_length[0] = length[0]/2;
        new_length[1] = length[1]/2;
        new_length[2] = length[2];
        new_length[3] = length[3];
        
        //Decode four view subblocks 
        DecodePartitionStep(new_position, new_length, entropyDecoder, mt);
        //optimize partition for Block_V returning JV, the transformed Block_V, partitionCode_S and arithmetic_model_S

        new_position[1] = position[1] + length[1]/2;
        new_length[1] = length[1] - length[1]/2;
        
        DecodePartitionStep(new_position, new_length, entropyDecoder, mt);

        new_position[0] = position[0] + length[0]/2;
        new_length[0] = length[0] - length[0]/2;
        
        DecodePartitionStep(new_position, new_length, entropyDecoder, mt);
        
        new_position[1] = position[1];
        new_length[1] = length[1]/2;
        
        DecodePartitionStep(new_position, new_length, entropyDecoder, mt);
        return;
    }
}

void PartitionDecoder :: DecodePartitionString(Hierarchical4DDecoder &entropyDecoder) {
 
    if(mPartitionCode == NULL) {
       mPartitionCodeMaxLength = 512; 
       mPartitionCode = new char [mPartitionCodeMaxLength];
    }
    strcpy(mPartitionCode, "");
    mPartitionCodeIndex = 0;
    
    DecodePartitionStringStep(entropyDecoder);
    
    mPartitionCode[mPartitionCodeIndex] = 0;
	// DSC begin
	/* commenting */
    //printf("partitionCode = %s\n", mPartitionCode);
	// DSC end   
}

void PartitionDecoder :: DecodePartitionStringStep(Hierarchical4DDecoder &entropyDecoder) {
    
    if(mPartitionCodeIndex >= mPartitionCodeMaxLength) {
        mPartitionCodeMaxLength += 512;
        char *code = new char [mPartitionCodeMaxLength];
        memcpy(code, mPartitionCode, sizeof(char)*(mPartitionCodeMaxLength-512));
        delete [] mPartitionCode;
        mPartitionCode = code;
    }
    
    //int flagCode = entropyDecoder.mEntropyDecoder.decode_symbol(3);
    int flagCode = entropyDecoder.DecodePartitionSymbol();
    
    if(flagCode == 0) {
        mPartitionCode[mPartitionCodeIndex] = NOSPLITFLAG;
        mPartitionCodeIndex++;
        return;
    }
    if(flagCode == 1) {
        mPartitionCode[mPartitionCodeIndex] = INTRAVIEWSPLITFLAG;
        mPartitionCodeIndex++;
        DecodePartitionStringStep(entropyDecoder);
        DecodePartitionStringStep(entropyDecoder);
        DecodePartitionStringStep(entropyDecoder);
        DecodePartitionStringStep(entropyDecoder);
        return;
    }
    if(flagCode == 2) {
        mPartitionCode[mPartitionCodeIndex] = INTERVIEWSPLITFLAG;
        mPartitionCodeIndex++;
        DecodePartitionStringStep(entropyDecoder);
        DecodePartitionStringStep(entropyDecoder);
        DecodePartitionStringStep(entropyDecoder);
        DecodePartitionStringStep(entropyDecoder);
        return;    
    }
}

