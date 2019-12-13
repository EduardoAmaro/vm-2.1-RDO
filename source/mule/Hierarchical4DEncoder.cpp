#include "Hierarchical4DEncoder.h"
#include <string.h>
#include <stdlib.h>

// DSC begin
#include <iostream>
#include <fstream>

using namespace std;
// DSC end
/*******************************************************************************/
/*                        Hierachical4DEncoder class methods                   */
/*******************************************************************************/

// DSC begin
// Hierarchical4DEncoder :: Hierarchical4DEncoder(void) {
//     mSuperiorBitPlane = 30;

//     mInferiorBitPlane = 0;
//     mPreSegmentation = 1;
//     mSegmentationTreeCodeBuffer = NULL;
//     mSegmentationTreeCodeBufferSize = 0;
    
// }
Hierarchical4DEncoder :: Hierarchical4DEncoder(int superiorBitPlane, int performRDO) {
    mSuperiorBitPlane = superiorBitPlane;
	mPerformRDO = performRDO;

    mInferiorBitPlane = 0;
    mPreSegmentation = 1;
    mSegmentationTreeCodeBuffer = NULL;
    mSegmentationTreeCodeBufferSize = 0;
    
}
Hierarchical4DEncoder :: ~Hierarchical4DEncoder(void) {
    if(mSegmentationTreeCodeBuffer != NULL)
        delete [] mSegmentationTreeCodeBuffer;
}

void Hierarchical4DEncoder :: StartEncoder(char *outputFile) {
    
    mEntropyCoder.add_model();                  //make new context (0)
    mEntropyCoder.start_model(2,0);             //initialize new binary context
    mEntropyCoder.add_model();                  //make new context (1)
    mEntropyCoder.start_model(65536,1);         //initialize new context with 65536 symbols
    mEntropyCoder.add_model();                  //make new context (2)
    mEntropyCoder.start_model(32,2);            //initialize new context with 32 symbols
    mEntropyCoder.add_model();                  //make new context (3)
    mEntropyCoder.start_model(3,3);             //initialize new context with 3 symbols

    mSymbolsArithmeticModelIndex = 4;
    mFlagsArithmeticModelIndex = 4;
    //for(int bitplane = 0; bitplane <= mSuperiorBitPlane-mInferiorBitPlane+1; bitplane++) {
    for(int bitplane = 0; bitplane <= 2*(mSuperiorBitPlane+1); bitplane++) {
        mEntropyCoder.add_model();                  //make new context (mFlagsArithmeticModelIndex+bitplane) for Flags
        //mEntropyCoder.start_model(3,bitplane+mFlagsArithmeticModelIndex);             //initialize new ternary context 
        mEntropyCoder.start_model(2,bitplane+mFlagsArithmeticModelIndex);             //initialize new binary context 
        mSymbolsArithmeticModelIndex++;
    }

    mFindBitplaneArithmeticModelIndex = mSymbolsArithmeticModelIndex;
    //for(int bitplane = 0; bitplane <= mSuperiorBitPlane-mInferiorBitPlane+1; bitplane++) {
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyCoder.add_model();                  //make new context (mSymbolsArithmeticModelIndex+bitplane) for Symbols
        mEntropyCoder.start_model(2,bitplane+mSymbolsArithmeticModelIndex);             //initialize new binary context 
        mFindBitplaneArithmeticModelIndex++;
    }

    mFindBestTreeArithmeticModelIndex = mFindBitplaneArithmeticModelIndex;
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyCoder.add_model();                  //make new context (mDraftArithmeticModelIndex+bitplane) for rd optimization
        mEntropyCoder.start_model(2,bitplane+mFindBitplaneArithmeticModelIndex);             //initialize new binary context 
        mFindBestTreeArithmeticModelIndex++;
    }

    mFindBestTreeFlagsArithmeticModelIndex = mFindBestTreeArithmeticModelIndex;
     for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyCoder.add_model();                  //make new context (mDraftArithmeticModelIndex+bitplane) for rd optimization
        mEntropyCoder.start_model(2,bitplane+mFindBestTreeArithmeticModelIndex);             //initialize new binary context 
        mFindBestTreeFlagsArithmeticModelIndex++;
    }

    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyCoder.add_model();                  //make new context (mDraftArithmeticModelIndex+bitplane) for rd optimization
        mEntropyCoder.start_model(3,bitplane+mFindBestTreeFlagsArithmeticModelIndex);             //initialize new ternary context 
   }
    
    mEntropyCoder.set_output_file(outputFile);  //opens output file
       
    mEntropyCoder.encode_symbol(mSuperiorBitPlane, 1);
    
    mEntropyCoder.encode_symbol(mTransformLength_t, 1);
    mEntropyCoder.encode_symbol(mTransformLength_s, 1);
    mEntropyCoder.encode_symbol(mTransformLength_v, 1);
    mEntropyCoder.encode_symbol(mTransformLength_u, 1);
    
    mEntropyCoder.encode_symbol(mMinimumTransformLength_t, 1);
    mEntropyCoder.encode_symbol(mMinimumTransformLength_s, 1);
    mEntropyCoder.encode_symbol(mMinimumTransformLength_v, 1);
    mEntropyCoder.encode_symbol(mMinimumTransformLength_u, 1);
    
    mEntropyCoder.encode_symbol(mNumberOfVerticalViews, 1);
    mEntropyCoder.encode_symbol(mNumberOfHorizontalViews, 1);
    
    mEntropyCoder.encode_symbol(mNumberOfViewLines, 1);
    mEntropyCoder.encode_symbol(mNumberOfViewColumns, 1);

    mEntropyCoder.encode_symbol(mPGMScale, 1);


}

void Hierarchical4DEncoder :: RestartProbabilisticModel(void) {
    
    mSymbolsArithmeticModelIndex = 4;
    mFlagsArithmeticModelIndex = 4;
    for(int bitplane = 0; bitplane <= 2*(mSuperiorBitPlane+1); bitplane++) {
        mEntropyCoder.restart_model(bitplane+mFlagsArithmeticModelIndex);             //restart context        
        mSymbolsArithmeticModelIndex++;
    }
    
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyCoder.restart_model(bitplane+mSymbolsArithmeticModelIndex);             //restart context        
    }
    
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyCoder.restart_model(bitplane+mFindBitplaneArithmeticModelIndex);             //restart context        
    }
    
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyCoder.restart_model(bitplane+mFindBestTreeArithmeticModelIndex);             //restart context        
    }
    
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyCoder.restart_model(bitplane+mFindBestTreeFlagsArithmeticModelIndex);             //restart context        
    }
}

void Hierarchical4DEncoder :: EncodeSubblock(double lambda, Statistics *stats) {
    
    int flagSearchIndex = 0;
    double Energy;
    
    strcpy(mSegmentationTreeCodeBuffer,"");
    RdOptimizeHexadecaTree(0, 0, 0, 0, mSubbandLF.mlength_t, mSubbandLF.mlength_s, mSubbandLF.mlength_v, mSubbandLF.mlength_u, lambda, mSuperiorBitPlane, &mSegmentationTreeCodeBuffer, Energy, stats);
  
    flagSearchIndex = 0;
    RdEncodeHexadecatree(0, 0, 0, 0, mSubbandLF.mlength_t, mSubbandLF.mlength_s, mSubbandLF.mlength_v, mSubbandLF.mlength_u, mSuperiorBitPlane, flagSearchIndex);
}

double Hierarchical4DEncoder :: RdOptimizeHexadecaTree(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, double lambda, int bitplane, char **codeString, double &signalEnergy, Statistics *stats) {

    double J0, J1;
    double SignalEnergySum;
    PModel currentFlagsArithmeticModelState[33];
    PModel currentCoefficientsArithmeticModelState[33];
    
    if(bitplane < mInferiorBitPlane) {
      	//printf("END\n");
        signalEnergy = 0;
        for(int index_t = 0; index_t < length_t; index_t++) {
            for(int index_s = 0; index_s < length_s; index_s++) {
                for(int index_v = 0; index_v < length_v; index_v++) {
                    for(int index_u = 0; index_u < length_u; index_u++) {
                        int coefficient = mSubbandLF.mPixel[position_t+index_t][position_s+index_s][position_v+index_v][position_u+index_u];
                        J0 = coefficient;
                        signalEnergy += J0*J0;
                    }
                }
            }
        }
        return (signalEnergy);
    }
    
    if(length_t*length_s*length_v*length_u == 1) {
		//printf("size 1\n");
        //evaluate the cost to encode coefficient
        int magnitude = mSubbandLF.mPixel[position_t][position_s][position_v][position_u];
        int signal = 0;
        if(magnitude < 0) {
            magnitude = -magnitude;
            signal = 1;
        }
        int allZeros = 1;
        
        signalEnergy = magnitude;
                
        signalEnergy *= signalEnergy;
        
        int onesMask = 0;
        onesMask = ~onesMask;

        double J = 0;
        double accumulatedRate = 0;
        for(int bit_position = bitplane; bit_position >= mInferiorBitPlane; bit_position--) {
            int bit = (magnitude >> bit_position)&01;
            accumulatedRate += mEntropyCoder.rate(bit, bit_position+1+mFindBestTreeArithmeticModelIndex);
            if(bit_position > BITPLANE_BYPASS) 
                mEntropyCoder.update_model(bit, bit_position+1+mFindBestTreeArithmeticModelIndex);
            
            if(bit == 1)
                allZeros = 0;
       }
        if(allZeros == 0)
            accumulatedRate += 1.0;
        int bitMask = onesMask << mInferiorBitPlane;
        int quantizedMagnitude = magnitude&bitMask;
        if(allZeros == 0) {
            quantizedMagnitude += (1 << mInferiorBitPlane)/2;
        }  
        J = magnitude - quantizedMagnitude;
        
        J = J*J + lambda*(accumulatedRate);
        *codeString[0] = 0;
		//printf("s1-J: %.0lf, %.0lf\n", J, lambda*(accumulatedRate));
        return(J);
    }
   
    for(int bit_position = bitplane; bit_position >= 0; bit_position--) {
        mEntropyCoder.start_pmodel(2, currentCoefficientsArithmeticModelState[bit_position]);    
        mEntropyCoder.get_model(bit_position+mFindBestTreeArithmeticModelIndex, currentCoefficientsArithmeticModelState[bit_position]);   
        mEntropyCoder.start_pmodel(3, currentFlagsArithmeticModelState[bit_position]);    
        mEntropyCoder.get_model(bit_position+mFindBestTreeFlagsArithmeticModelIndex, currentFlagsArithmeticModelState[bit_position]);   
    }
   
    char *codeString_0 = new char [2];
    strcpy(codeString_0, "");
    
    int Significance = 0;
    
    int Threshold = 1 << bitplane;
    
    for(int index_t = position_t; index_t < position_t+length_t; index_t++) {
        for(int index_s = position_s; index_s < position_s+length_s; index_s++) {           
            for(int index_v = position_v; index_v < position_v+length_v; index_v++) {               
                for(int index_u = position_u; index_u < position_u+length_u; index_u++) {
                    
                    if((index_t < mSubbandLF.mlength_t)&&(index_s < mSubbandLF.mlength_s)&&(index_v < mSubbandLF.mlength_v)&&(index_u < mSubbandLF.mlength_u)) {
                        
                        if(mSubbandLF.mPixel[index_t][index_s][index_v][index_u] >= Threshold) Significance = 1;
                        if(mSubbandLF.mPixel[index_t][index_s][index_v][index_u] <= -Threshold) Significance = 1;
                        if(Significance == 1) {
                            index_t = position_t+length_t;
                            index_s = position_s+length_s;
                            index_v = position_v+length_v;
                            index_u = position_u+length_u;
                        }
                    
                    }
                    
                }
                
            }
            
        }
        
    }
    
    
    //evaluate the cost J0 to encode this subblock
    if(Significance == 0) {
		// DSC begin
		stats->incBcDecrease();
        //printf("Bc decrease\n");
		// DSC end
        J0 = RdOptimizeHexadecaTree(position_t, position_s, position_v, position_u, length_t, length_s, length_v, length_u, lambda, bitplane-1, &codeString_0, SignalEnergySum, stats);
        
    }
    else {
        //printf("divide light field\n");
        SignalEnergySum = 0;
        double Energy;
        J0 = 0;
		
        int half_length_t = (length_t > 1) ? length_t/2 : 1;
        int half_length_s = (length_s > 1) ? length_s/2 : 1;
        int half_length_v = (length_v > 1) ? length_v/2 : 1;
        int half_length_u = (length_u > 1) ? length_u/2 : 1;
        
        int number_of_subdivisions_t = (length_t > 1) ? 2 : 1;
        int number_of_subdivisions_s = (length_s > 1) ? 2 : 1;
        int number_of_subdivisions_v = (length_v > 1) ? 2 : 1;
        int number_of_subdivisions_u = (length_u > 1) ? 2 : 1;

		// DSC begin
		stats->incSplitHexaDecaTree();
        // DSC end

        for(int index_t = 0; index_t < number_of_subdivisions_t; index_t++) {
            
            for(int index_s = 0; index_s < number_of_subdivisions_s; index_s++) {
                
                for(int index_v = 0; index_v < number_of_subdivisions_v; index_v++) {
                    
                    for(int index_u = 0; index_u < number_of_subdivisions_u; index_u++) {
                        
                        int new_position_t = position_t+index_t*half_length_t;
                        int new_position_s = position_s+index_s*half_length_s;
                        int new_position_v = position_v+index_v*half_length_v;
                        int new_position_u = position_u+index_u*half_length_u;
                        
                        int new_length_t = (index_t == 0) ? half_length_t : (length_t-half_length_t);
                        int new_length_s = (index_s == 0) ? half_length_s : (length_s-half_length_s);
                        int new_length_v = (index_v == 0) ? half_length_v : (length_v-half_length_v);
                        int new_length_u = (index_u == 0) ? half_length_u : (length_u-half_length_u);
                        
                        char *codeString_1 = new char [2];
                            
                        strcpy(codeString_1, "");
                        J0 += RdOptimizeHexadecaTree(new_position_t, new_position_s, new_position_v, new_position_u, new_length_t, new_length_s, new_length_v, new_length_u, lambda, bitplane, &codeString_1, Energy, stats);
                        char *tempString = new char[strlen(codeString_0)+strlen(codeString_1)+2];
                        strcpy(tempString, codeString_0);
                        strcat(tempString, codeString_1);
                        delete [] codeString_0;
                        delete [] codeString_1;
                        codeString_0 = tempString;
						//printf("energy: %.0lf\n", Energy);
                        SignalEnergySum += Energy;
                    }
                    
                }
                
            }
            
        }       
             
    }    
    
    //evaluate the cost J1 to skip this subblock
    J1 = SignalEnergySum + lambda*mEntropyCoder.rate(2, mFindBestTreeFlagsArithmeticModelIndex+bitplane);
    J0 += lambda*mEntropyCoder.rate(Significance, mFindBestTreeFlagsArithmeticModelIndex+bitplane);
	//printf("[SES, J1, J0]: %.0lf, %.0lf, %.0lf, %.0lf, %.0lf\n", SignalEnergySum, J1, J0, lambda*mEntropyCoder.rate(2, mFindBestTreeFlagsArithmeticModelIndex+bitplane));
    
	//Choose the lowest cost
	// DSC begin
	if(mPerformRDO == 1) {
		if((J0 < J1)||((bitplane == mInferiorBitPlane)&&(Significance == 0))) {
			char *tempString = new char[strlen(*codeString)+strlen(codeString_0)+3];
			strcpy(tempString, *codeString);
			delete [] *codeString;
			*codeString = tempString;
			
			if(Significance == 1) {
				strcat(*codeString, "1");         
			}
			else {
				strcat(*codeString, "0");
			}
			strcat(*codeString, codeString_0);
			if(bitplane > BITPLANE_BYPASS_FLAGS)   
				mEntropyCoder.update_model(Significance, mFindBestTreeFlagsArithmeticModelIndex+bitplane);
		}
		else {
			char *tempString = new char[strlen(*codeString)+3];
			strcpy(tempString, *codeString);
			delete [] *codeString;
			*codeString = tempString;
			strcat(*codeString, "2");
			J0 = J1;
			
			for(int bit_position = bitplane; bit_position >= 0; bit_position--) {

				mEntropyCoder.load_model(bit_position+mFindBestTreeArithmeticModelIndex, currentCoefficientsArithmeticModelState[bit_position]);   
				mEntropyCoder.load_model(bit_position+mFindBestTreeFlagsArithmeticModelIndex, currentFlagsArithmeticModelState[bit_position]);   

			}
			
			if(bitplane > BITPLANE_BYPASS_FLAGS) 
				mEntropyCoder.update_model(2, mFindBestTreeFlagsArithmeticModelIndex+bitplane);
		}
	}
	else {
		char *tempString = new char[strlen(*codeString)+strlen(codeString_0)+3];
		strcpy(tempString, *codeString);
		delete [] *codeString;
		*codeString = tempString;
		
		if(Significance == 1) {
			strcat(*codeString, "1");         
		}
		else {
			strcat(*codeString, "0");
		}
		strcat(*codeString, codeString_0);
		if(bitplane > BITPLANE_BYPASS_FLAGS)   
			mEntropyCoder.update_model(Significance, mFindBestTreeFlagsArithmeticModelIndex+bitplane);
	}
	// DSC end

    delete [] codeString_0;
    
    signalEnergy = SignalEnergySum;
    
    for(int bit_position = bitplane; bit_position >= 0; bit_position--) {
        mEntropyCoder.delete_pmodel(currentCoefficientsArithmeticModelState[bit_position]);    
        mEntropyCoder.delete_pmodel(currentFlagsArithmeticModelState[bit_position]);    
    }
    
    return(J0);
    
}
void Hierarchical4DEncoder :: RdEncodeHexadecatree(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, int bitplane, int &flagIndex) {
   
    if(bitplane < mInferiorBitPlane) {
        return;
    }
    
    if(length_t*length_s*length_v*length_u == 1) {
        //rd encode coefficient        
        RdEncodeCoefficient(mSubbandLF.mPixel[position_t][position_s][position_v][position_u], bitplane, flagIndex);
        return;
    }
    
    if(mSegmentationTreeCodeBuffer[flagIndex] == '0') {

        EncodeTernaryFlag(0, mFlagsArithmeticModelIndex+2*bitplane, bitplane > BITPLANE_BYPASS_FLAGS);
    
        flagIndex++;
        RdEncodeHexadecatree(position_t, position_s, position_v, position_u, length_t, length_s, length_v, length_u, bitplane-1, flagIndex);
        
        return;
    }
    
    if(mSegmentationTreeCodeBuffer[flagIndex] == '2') {

        EncodeTernaryFlag(2, mFlagsArithmeticModelIndex+2*bitplane, bitplane > BITPLANE_BYPASS_FLAGS);
    
        flagIndex++;

        return;
    }

    if(mSegmentationTreeCodeBuffer[flagIndex] == '1') {
        
        EncodeTernaryFlag(1, mFlagsArithmeticModelIndex+2*bitplane, bitplane > BITPLANE_BYPASS_FLAGS);
    
        flagIndex++;
        
        int half_length_t = (length_t > 1) ? length_t/2 : 1;
        int half_length_s = (length_s > 1) ? length_s/2 : 1;
        int half_length_v = (length_v > 1) ? length_v/2 : 1;
        int half_length_u = (length_u > 1) ? length_u/2 : 1;
        
        int number_of_subdivisions_t = (length_t > 1) ? 2 : 1;
        int number_of_subdivisions_s = (length_s > 1) ? 2 : 1;
        int number_of_subdivisions_v = (length_v > 1) ? 2 : 1;
        int number_of_subdivisions_u = (length_u > 1) ? 2 : 1;
        
        
        for(int index_t = 0; index_t < number_of_subdivisions_t; index_t++) {
            
            for(int index_s = 0; index_s < number_of_subdivisions_s; index_s++) {
                
                for(int index_v = 0; index_v < number_of_subdivisions_v; index_v++) {
                    
                    for(int index_u = 0; index_u < number_of_subdivisions_u; index_u++) {
                        
                        int new_position_t = position_t+index_t*half_length_t;
                        int new_position_s = position_s+index_s*half_length_s;
                        int new_position_v = position_v+index_v*half_length_v;
                        int new_position_u = position_u+index_u*half_length_u;
                        
                        int new_length_t = (index_t == 0) ? half_length_t : (length_t-half_length_t);
                        int new_length_s = (index_s == 0) ? half_length_s : (length_s-half_length_s);
                        int new_length_v = (index_v == 0) ? half_length_v : (length_v-half_length_v);
                        int new_length_u = (index_u == 0) ? half_length_u : (length_u-half_length_u);
                        
                        RdEncodeHexadecatree(new_position_t, new_position_s, new_position_v, new_position_u, new_length_t, new_length_s, new_length_v, new_length_u, bitplane, flagIndex);
                
                   }
                    
                }
                
            }
            
        }
        
        return;
    }
   
}

void Hierarchical4DEncoder :: RdEncodeCoefficient(int coefficient, int bitplane, int &flagIndex) {
    
    int signal = 0;
    int allZeros = 1;
    int magnitude = coefficient;
    if(magnitude < 0) {
        magnitude = -magnitude;
        signal = 1;
    }
    
    //if(magnitude >= (1 << mInferiorBitPlane)) 
    for(int bit_position = bitplane; bit_position >= mInferiorBitPlane; bit_position--) {
        int bit = (magnitude >> (bit_position))&01;
        mEntropyCoder.encode_symbol(bit, bit_position+1+mSymbolsArithmeticModelIndex);
        if(bit_position > BITPLANE_BYPASS) 
            mEntropyCoder.update_model(bit, bit_position+1+mSymbolsArithmeticModelIndex);
        
         if(bit == 1) {
            allZeros = 0;
        }
    }
    if(allZeros == 0) {
        mEntropyCoder.encode_symbol(signal, 0);
    }
    
    int quantizedMagnitude = magnitude >> mInferiorBitPlane;
    quantizedMagnitude = quantizedMagnitude << mInferiorBitPlane;
    if(allZeros == 0) {
        quantizedMagnitude += (1 << mInferiorBitPlane)/2;
    }
    double D = magnitude-quantizedMagnitude;
    
}

void Hierarchical4DEncoder :: EncodeTernaryFlag(int flag, int base_arithmetic_model_index, int adapt) {
    
    if(flag == 0) {
        mEntropyCoder.encode_symbol(0, base_arithmetic_model_index);
        mEntropyCoder.encode_symbol(0, base_arithmetic_model_index+1);
        if(adapt) {
            mEntropyCoder.update_model(0, base_arithmetic_model_index);
            mEntropyCoder.update_model(0, base_arithmetic_model_index+1);
        }
    }
    if(flag == 1) {
        mEntropyCoder.encode_symbol(0, base_arithmetic_model_index);
        mEntropyCoder.encode_symbol(1, base_arithmetic_model_index+1);
        if(adapt) {
            mEntropyCoder.update_model(0, base_arithmetic_model_index);
            mEntropyCoder.update_model(1, base_arithmetic_model_index+1);
        }
    }
    if(flag == 2) {
        mEntropyCoder.encode_symbol(1, base_arithmetic_model_index);
        if(adapt) {
            mEntropyCoder.update_model(1, base_arithmetic_model_index);
        }
    }
}

void Hierarchical4DEncoder :: LoadToSubBand4D(const Block4D &B, int sbb_t, int sbb_s, int sbb_v, int sbb_u) {
    
    for(int index_t = 0; index_t < B.mlength_t; index_t++) {
        
        for(int index_s = 0; index_s < B.mlength_s; index_s++) {
            
            for(int index_v = 0; index_v < B.mlength_v; index_v++) {
                
                for(int index_u = 0; index_u < B.mlength_u; index_u++) {
                    
                    int position_t = index_t*mSubbandLF.mlength_t/B.mlength_t+sbb_t;
                    int position_s = index_s*mSubbandLF.mlength_s/B.mlength_s+sbb_s;
                    int position_v = index_v*mSubbandLF.mlength_v/B.mlength_v+sbb_v;
                    int position_u = index_u*mSubbandLF.mlength_u/B.mlength_u+sbb_u;
                    if((position_t < mSubbandLF.mlength_t)&&(position_s < mSubbandLF.mlength_s)&&(position_v < mSubbandLF.mlength_v)&&(position_u < mSubbandLF.mlength_u)) {
                        mSubbandLF.mPixel[position_t][position_s][position_v][position_u] = B.mPixel[index_t][index_s][index_v][index_u];
                    }
                }
                
            }
            
        }
        
    }
}

void Hierarchical4DEncoder :: DoneEncoding(void) {
    
    mEntropyCoder.done_encoding();      //flushes entropy encoder
    
}

int Hierarchical4DEncoder :: SubbandNumber(int position_t, int position_s, int position_v, int position_u) {
    
    int subband_size_t = mSubbandLF.mlength_t/mTransformLength_t;
    int subband_size_s = mSubbandLF.mlength_s/mTransformLength_s;
    int subband_size_v = mSubbandLF.mlength_v/mTransformLength_v;
    int subband_size_u = mSubbandLF.mlength_u/mTransformLength_u;
    
    
    int band = position_t/subband_size_t * mTransformLength_s * mTransformLength_v * mTransformLength_u;
    band += position_s/subband_size_s * mTransformLength_v * mTransformLength_u;
    band += mTransformLength_u * position_v/subband_size_v + position_u/subband_size_u;
    
    return(band);
}

void Hierarchical4DEncoder :: SetDimension(int length_t, int length_s, int length_v, int length_u)  {
    
    mSubbandLF.SetDimension(length_t, length_s, length_v, length_u);

    if(mSegmentationTreeCodeBuffer != NULL)
        delete [] mSegmentationTreeCodeBuffer;
        
    mSegmentationTreeCodeBufferSize = 2*length_t*length_s*length_v*length_u+1; //enough for at most one bit per pixel rates

    mSegmentationTreeCodeBuffer = new char [mSegmentationTreeCodeBufferSize];
}

int Hierarchical4DEncoder :: OptimumBitplane(double lambda) {
    long int subbandSize = mSubbandLF.mlength_t*mSubbandLF.mlength_s;
    subbandSize *= mSubbandLF.mlength_v*mSubbandLF.mlength_u;
    double Jmin;
	//double minDistortion, minRate;
    int optimumBitplane;
    double accumulatedRate = 0;
//    double estimatedRate;
//    double estimatedDistortion;
//    int numberOfCoefficientsEstimated = 0;

    for(int bit_position = mSuperiorBitPlane; bit_position >= 0; bit_position--) {
        
        double distortion = 0.0;
        double coefficientsDistortion = 0.0;
        double signalRate = 0.0;
        double J;
        int numberOfCoefficients = 0;
        
        int onesMask = 0;
        onesMask = ~onesMask;
        int bitMask = onesMask << bit_position;
       
        for(long int coefficient_index=0; coefficient_index < subbandSize; coefficient_index++) {
            int magnitude = mSubbandLF.mPixelData[coefficient_index];
            if(magnitude < 0) {
                magnitude = -magnitude;
            }
            int Threshold = (1 << bit_position);
            if(magnitude >= Threshold) {
                int bit = (magnitude >> bit_position)&01;
                accumulatedRate += mEntropyCoder.rate(bit, bit_position+1+mFindBitplaneArithmeticModelIndex);
                mEntropyCoder.update_model(bit, bit_position+1+mFindBitplaneArithmeticModelIndex);
                //if(bit == 1)
                int quantizedMagnitude = magnitude&bitMask;
                if(quantizedMagnitude > 0) {
                    signalRate += 1.0;
                }
                numberOfCoefficients++;
            }
            int quantizedMagnitude = magnitude&bitMask;
            if(quantizedMagnitude > 0) {
                quantizedMagnitude += (1 << bit_position)/2;
            } 
            double magnitude_error = magnitude - quantizedMagnitude;
			//printf("[mag_error, mag, qMag]: %.0lf, %d, %d\n", magnitude_error, magnitude, quantizedMagnitude);
            distortion += magnitude_error*magnitude_error;
			//printf("[distortion]: %.0lf\n", distortion);
            if(magnitude >= (1 << bit_position)) {
                coefficientsDistortion += magnitude_error*magnitude_error;
            }
        }
        J = distortion + lambda*(accumulatedRate + signalRate);
		// DSC begin
		//printf("[d, r]: %.0lf, %.0lf\n", distortion, lambda*(accumulatedRate + signalRate));
		// DSC end
        //if((J < Jmin)||(bit_position == mSuperiorBitPlane)) {
        if((J <= Jmin)||(bit_position == mSuperiorBitPlane)) {
            Jmin = J;
			// minDistortion = distortion;
			// minRate = accumulatedRate + signalRate;
            // optimumBitplane = bit_position;
            //estimatedRate = accumulatedRate + signalRate;
            //estimatedDistortion = coefficientsDistortion;
            //numberOfCoefficientsEstimated = numberOfCoefficients;
        }
       
    }
    return(optimumBitplane);
}

double Hierarchical4DEncoder :: RDCost(double lambda) {
    long int subbandSize = mSubbandLF.mlength_t*mSubbandLF.mlength_s;
    subbandSize *= mSubbandLF.mlength_v*mSubbandLF.mlength_u;
    double Jmin;
    //int optimumBitplane;
    double accumulatedRate = 0;
//    double estimatedRate;
//    double estimatedDistortion;
//    int numberOfCoefficientsEstimated = 0;
	
	// DSC begin
	int magnitude;
        for(long int coefficient_index=0; coefficient_index < subbandSize; coefficient_index++) {
            magnitude = mSubbandLF.mPixelData[coefficient_index];
			magnitudeFile << magnitude << '\n';
			signalTotalEnergy += magnitude*magnitude;
			//counter++;
	}
	//printf("\t\tenergy: %lld\n", signalTotalEnergy);
	// DSC end

    //EDUARDO begin
    //for(int bit_position = mSuperiorBitPlane; bit_position >= 0; bit_position--) {
        int bit_position = mInferiorBitPlane;
        
        double distortion = 0.0;
        double coefficientsDistortion = 0.0;
        double signalRate = 0.0;
        double J;
        int numberOfCoefficients = 0;
        
        int onesMask = 0;
        onesMask = ~onesMask;
        int bitMask = onesMask << bit_position;
       
        for(long int coefficient_index=0; coefficient_index < subbandSize; coefficient_index++) {
            int magnitude = mSubbandLF.mPixelData[coefficient_index];
            if(magnitude < 0) {
                magnitude = -magnitude;
            }
            int Threshold = (1 << bit_position);
            if(magnitude >= Threshold) {
                int bit = (magnitude >> bit_position)&01;
                accumulatedRate += mEntropyCoder.rate(bit, bit_position+1+mFindBitplaneArithmeticModelIndex);
                //mEntropyCoder.update_model(bit, bit_position+1+mFindBitplaneArithmeticModelIndex);
                //if(bit == 1)
                int quantizedMagnitude = magnitude&bitMask;
                if(quantizedMagnitude > 0) {
                    signalRate += 1.0;
                }
                numberOfCoefficients++;
            }
            int quantizedMagnitude = magnitude&bitMask;
            if(quantizedMagnitude > 0) {
                quantizedMagnitude += (1 << bit_position)/2;
            } 
            double magnitude_error = magnitude - quantizedMagnitude;
            distortion += magnitude_error*magnitude_error;
            if(magnitude >= (1 << bit_position)) {
                coefficientsDistortion += magnitude_error*magnitude_error;
            }
        }
        J = distortion + lambda*(accumulatedRate + signalRate);
        //printf("[dist, rate]: %.0lf, %.0lf\n", distortion, accumulatedRate + signalRate);
        //if((J < Jmin)||(bit_position == mSuperiorBitPlane)) {
        //EDUARDO begin
        //if((J <= Jmin)||(bit_position == mSuperiorBitPlane)) {
        Jmin = J;
            //optimumBitplane = bit_position;
            //estimatedRate = accumulatedRate + signalRate;
            //estimatedDistortion = coefficientsDistortion;
            //numberOfCoefficientsEstimated = numberOfCoefficients;
        //}
       
    //}
    //EDUARDO end
    return(Jmin);
}

void Hierarchical4DEncoder :: GetOptimizerProbabilisticModelState(PModel **state) {
       
    PModel *pmodelArray = new PModel [2*(mSuperiorBitPlane+2)];
    for(int bit_position = 0; bit_position <= mSuperiorBitPlane+1; bit_position++) {
        mEntropyCoder.start_pmodel(2, pmodelArray[bit_position]);    
        mEntropyCoder.get_model(bit_position+mFindBestTreeArithmeticModelIndex, pmodelArray[bit_position]);   
        mEntropyCoder.start_pmodel(3, pmodelArray[bit_position+mSuperiorBitPlane+2]);    
        mEntropyCoder.get_model(bit_position+mFindBestTreeFlagsArithmeticModelIndex, pmodelArray[bit_position+mSuperiorBitPlane+2]);   
    }
    *state = pmodelArray;
}

void Hierarchical4DEncoder :: SetOptimizerProbabilisticModelState(PModel *state) {

     for(int bit_position = 0; bit_position <= mSuperiorBitPlane+1; bit_position++) {
        mEntropyCoder.load_model(bit_position+mFindBestTreeArithmeticModelIndex, state[bit_position]);   
        mEntropyCoder.load_model(bit_position+mFindBestTreeFlagsArithmeticModelIndex, state[bit_position+mSuperiorBitPlane+2]);   
    }
   
}

void Hierarchical4DEncoder :: DeleteProbabilisticModelState(PModel *state) {
    
    if(state != NULL) {
        for(int bit_position = 0; bit_position <= mSuperiorBitPlane+1; bit_position++) {
            mEntropyCoder.delete_pmodel(state[bit_position]);   
            mEntropyCoder.delete_pmodel(state[bit_position+mSuperiorBitPlane+2]);   
        }
        delete [] state;
        state = NULL;
    }
     
}

void Hierarchical4DEncoder :: EncodePartitionSymbol(int symbol) {
    //mEntropyCoder.encode_symbol(symbol, 3);
     
    if(symbol == 2) {
        mEntropyCoder.encode_symbol(1, 0);
        mEntropyCoder.encode_symbol(1, 0);
    }
    if(symbol == 1) {
        mEntropyCoder.encode_symbol(1, 0);
        mEntropyCoder.encode_symbol(0, 0);
    }
    if(symbol == 0) {
        mEntropyCoder.encode_symbol(0, 0);
    }
   
}