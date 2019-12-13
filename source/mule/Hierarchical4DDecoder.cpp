#include "Hierarchical4DDecoder.h"
/*******************************************************************************/
/*                        Hierachical4DDeccoder class methods                  */
/*******************************************************************************/

Hierarchical4DDecoder :: Hierarchical4DDecoder(void) {
	// DSC begin
	quantizedCoefficients.open("quantized_coefficients.txt");
	// DSC end
    mSuperiorBitPlane = 30;

    mInferiorBitPlane = 0;
    mPreSegmentation = 1;
}
Hierarchical4DDecoder :: ~Hierarchical4DDecoder(void) {
    
}

void Hierarchical4DDecoder :: StartDecoder(char *inputFile) {
    
    mEntropyDecoder.add_model();                  //make new context (0)
    mEntropyDecoder.start_model(2,0);             //initialize new binary context
    mEntropyDecoder.add_model();                  //make new context (1)
    mEntropyDecoder.start_model(65536,1);         //initialize new context with 65536 symbols
    mEntropyDecoder.add_model();                  //make new context (2)
    mEntropyDecoder.start_model(32,2);            //initialize new context with 32 symbols
    mEntropyDecoder.add_model();                    //make new context (3)
    mEntropyDecoder.start_model(3,3);               //initialize new context with 3 symbols
    
    mEntropyDecoder.set_input_file(inputFile);  //opens output file
    
    //mSubbandLF.mlength_t = mEntropyDecoder.decode_symbol(1);
    //mSubbandLF.mlength_s = mEntropyDecoder.decode_symbol(1);
    //mSubbandLF.mlength_v = mEntropyDecoder.decode_symbol(1);
    //mSubbandLF.mlength_u = mEntropyDecoder.decode_symbol(1);
    
    //mSubbandLF.SetDimension(mSubbandLF.mlength_t, mSubbandLF.mlength_s, mSubbandLF.mlength_v, mSubbandLF.mlength_u);
    //mSubbandLF.Zeros();
    
    mSuperiorBitPlane = mEntropyDecoder.decode_symbol(1);
    //mInferiorBitPlane = mEntropyDecoder.decode_symbol(1);
/*            
    mACArithmeticModelIndex = 4;
    mDCArithmeticModelIndex = 4;
    int max_number_of_symbols = 1 << MAX_DEPH_CONDICIONING;
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane; bitplane++) {
        mEntropyDecoder.add_model();                  //make new context (mACArithmeticModelIndex+bitplane) for AC coefficients
        unsigned long int number_of_symbols = 1 << (bitplane+2);
        if(number_of_symbols > max_number_of_symbols) number_of_symbols = max_number_of_symbols;
        mEntropyDecoder.start_model(number_of_symbols,bitplane+mACArithmeticModelIndex);             //initialize new context with at most 2^MAX_DEPH_CONDICIONING symbols        
        mDCArithmeticModelIndex++;
    }
    
    mFlagsArithmeticModelIndex = mDCArithmeticModelIndex;
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane; bitplane++) {
        mEntropyDecoder.add_model();                  //make new context (mDCArithmeticModelIndex+bitplane) for DC coefficients
        unsigned long int number_of_symbols = 1 << (bitplane+2);
        if(number_of_symbols > max_number_of_symbols) number_of_symbols = max_number_of_symbols;
        mEntropyDecoder.start_model(number_of_symbols,bitplane+mDCArithmeticModelIndex);             //initialize new context with at most 2^MAX_DEPH_CONDICIONING symbols        
        mFlagsArithmeticModelIndex++;
    }
    mSymbolsArithmeticModelIndex = mFlagsArithmeticModelIndex;

*/
    mSymbolsArithmeticModelIndex = 4;
    mFlagsArithmeticModelIndex = 4;
    //for(int bitplane = 0; bitplane <= mSuperiorBitPlane-mInferiorBitPlane+1; bitplane++) {
    for(int bitplane = 0; bitplane <= 2*(mSuperiorBitPlane+1); bitplane++) {
        mEntropyDecoder.add_model();                  //make new context (mFlagsArithmeticModelIndex+bitplane) for Flags
        //mEntropyDecoder.start_model(3,bitplane+mFlagsArithmeticModelIndex);             //initialize new ternary context        
        mEntropyDecoder.start_model(2,bitplane+mFlagsArithmeticModelIndex);             //initialize new binary context        
        mSymbolsArithmeticModelIndex++;
    }
    
    //for(int bitplane = 0; bitplane <= mSuperiorBitPlane-mInferiorBitPlane+1; bitplane++) {
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyDecoder.add_model();                  //make new context (mSymbolsArithmeticModelIndex+bitplane) for Symbols
        //mEntropyDecoder.start_model(3,bitplane+mSymbolsArithmeticModelIndex);             //initialize new ternary context 
        mEntropyDecoder.start_model(2,bitplane+mSymbolsArithmeticModelIndex);             //initialize new binary context 
    }
 
    mTransformLength_t = mEntropyDecoder.decode_symbol(1);
    mTransformLength_s = mEntropyDecoder.decode_symbol(1);
    mTransformLength_v = mEntropyDecoder.decode_symbol(1);
    mTransformLength_u = mEntropyDecoder.decode_symbol(1);
    
    mMinimumTransformLength_t = mEntropyDecoder.decode_symbol(1);
    mMinimumTransformLength_s = mEntropyDecoder.decode_symbol(1);
    mMinimumTransformLength_v = mEntropyDecoder.decode_symbol(1);
    mMinimumTransformLength_u = mEntropyDecoder.decode_symbol(1);
    
    mNumberOfVerticalViews = mEntropyDecoder.decode_symbol(1);
    mNumberOfHorizontalViews = mEntropyDecoder.decode_symbol(1);
    
    mNumberOfViewLines = mEntropyDecoder.decode_symbol(1);
    mNumberOfViewColumns = mEntropyDecoder.decode_symbol(1);
    
    mPGMScale = mEntropyDecoder.decode_symbol(1);

}

void Hierarchical4DDecoder :: RestartProbabilisticModel(void) {

    mSymbolsArithmeticModelIndex = 4;
    mFlagsArithmeticModelIndex = 4;
    for(int bitplane = 0; bitplane <= 2*(mSuperiorBitPlane+1); bitplane++) {
        mEntropyDecoder.restart_model(bitplane+mFlagsArithmeticModelIndex);             //restart context     
        mSymbolsArithmeticModelIndex++;
    }
    
    for(int bitplane = 0; bitplane <= mSuperiorBitPlane+1; bitplane++) {
        mEntropyDecoder.restart_model(bitplane+mSymbolsArithmeticModelIndex);             //restart context        
    }

}

void Hierarchical4DDecoder :: DecodeBlock(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, int bitplane) {
    
    if(bitplane < mInferiorBitPlane) {
        return;
    }
    
    if(length_t*length_s*length_v*length_u == 1) {
        int coefficient =  DecodeCoefficient(bitplane);     

        mSubbandLF.mPixel[position_t][position_s][position_v][position_u] = coefficient;

		// DSC begin
		quantizedCoefficients << coefficient << '\n';
		//printf("qCoeff: %d\n", coefficient);
		// DSC end

        return;
    }
    
    //int Significance = mEntropyDecoder.decode_symbol(mFlagsArithmeticModelIndex+bitplane);
    //if(bitplane > BITPLANE_BYPASS_FLAGS) 
    //    mEntropyDecoder.update_model(Significance, mFlagsArithmeticModelIndex+bitplane);
    int Significance = DecodeTernaryFlag(mFlagsArithmeticModelIndex+2*bitplane, bitplane > BITPLANE_BYPASS_FLAGS);
            
    if(Significance == 0) {
        
        DecodeBlock(position_t, position_s, position_v, position_u, length_t, length_s, length_v, length_u, bitplane-1);
        
        return;
    }
    if(Significance == 1) {
                
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
                                           
                        DecodeBlock(new_position_t, new_position_s, new_position_v, new_position_u, new_length_t, new_length_s, new_length_v, new_length_u, bitplane);
                
                   }
                    
                }
                
            }
            
        }
       
        return;
    }
    if(Significance == 2) {
 /*       
        for(int index_t = 0; index_t < length_t; index_t++) {
            for(int index_s = 0; index_s < length_s; index_s++) {
                for(int index_v = 0; index_v < length_v; index_v++) {
                    for(int index_u = 0; index_u < length_u; index_u++) {
                        mSubbandLF.mPixel[position_t+index_t][position_s+index_s][position_v+index_v][position_u+index_u] = 0;
                    }

                }

            }
           
        }*/
        return;
    }
}

int Hierarchical4DDecoder :: DecodeCoefficient(int bitplane) {
    
        int magnitude = 0;
        int bit;
        int bit_position=bitplane;
        
        for(bit_position = bitplane; bit_position >= mInferiorBitPlane; bit_position--) {
            
            magnitude = magnitude << 1;
            bit = mEntropyDecoder.decode_symbol(bit_position+1+mSymbolsArithmeticModelIndex);
            if(bit_position > BITPLANE_BYPASS) 
                mEntropyDecoder.update_model(bit, bit_position+1+mSymbolsArithmeticModelIndex);
            
            if(bit == 1) {
                magnitude++;
             }
            
        }

        magnitude = magnitude << mInferiorBitPlane;
        if(magnitude > 0) {
            magnitude += (1 << mInferiorBitPlane)/2;
            if(mEntropyDecoder.decode_symbol(0) == 1) {
                magnitude = -magnitude;
            }
        }            
        

        return(magnitude);
}

int Hierarchical4DDecoder :: DecodeTernaryFlag(int base_arithmetic_model_index, int adapt)  {

    int bit0=0;
    int bit1 = mEntropyDecoder.decode_symbol(base_arithmetic_model_index);
    if(adapt)
        mEntropyDecoder.update_model(bit1, base_arithmetic_model_index);
    if(bit1 == 0) {
        bit0 = mEntropyDecoder.decode_symbol(base_arithmetic_model_index+1);
        if(adapt)
            mEntropyDecoder.update_model(bit0, base_arithmetic_model_index+1);
    }
    return(bit0+2*bit1);
}

int Hierarchical4DDecoder :: DecodePartitionSymbol(void)  {

    //int symbol = mEntropyDecoder.decode_symbol(3);
/*    
    int bit0=0;
    int bit1 = mEntropyDecoder.decode_symbol(0);
    if(bit1 == 1) {
        bit0 = mEntropyDecoder.decode_symbol(0);
    }
    
    int symbol=0;
    if(bit1 == 1) {
        symbol = 1;
        if(bit0 == 1)
            symbol++;
    }
*/
    int symbol = mEntropyDecoder.decode_symbol(0);
    if(symbol == 1) {
        int bit = mEntropyDecoder.decode_symbol(0);
        if(bit == 1)
            symbol++;
    }
    return(symbol);
        
}

void Hierarchical4DDecoder :: GetFromSubBand4D(Block4D &B, int sbb_t, int sbb_s, int sbb_v, int sbb_u) {
    
    for(int index_t = 0; index_t < B.mlength_t; index_t++) {
        
        for(int index_s = 0; index_s < B.mlength_s; index_s++) {
            
            for(int index_v = 0; index_v < B.mlength_v; index_v++) {
                
                for(int index_u = 0; index_u < B.mlength_u; index_u++) {
                    
                    int position_t = index_t*mSubbandLF.mlength_t/B.mlength_t+sbb_t;
                    int position_s = index_s*mSubbandLF.mlength_s/B.mlength_s+sbb_s;
                    int position_v = index_v*mSubbandLF.mlength_v/B.mlength_v+sbb_v;
                    int position_u = index_u*mSubbandLF.mlength_u/B.mlength_u+sbb_u;
                    if((position_t < mSubbandLF.mlength_t)&&(position_s < mSubbandLF.mlength_s)&&(position_v < mSubbandLF.mlength_v)&&(position_u < mSubbandLF.mlength_u)) {
                        B.mPixel[index_t][index_s][index_v][index_u] = mSubbandLF.mPixel[position_t][position_s][position_v][position_u];
                    }
                }
                
            }
            
        }
        
    }
}

void Hierarchical4DDecoder :: DoneDecoding(void) {
    
    mEntropyDecoder.done_decoding();      //flushes entropy decoder
    
}

int Hierarchical4DDecoder :: SubbandNumber(int position_t, int position_s, int position_v, int position_u) {
    
    int subband_size_t = mSubbandLF.mlength_t/mTransformLength_t;
    int subband_size_s = mSubbandLF.mlength_s/mTransformLength_s;
    int subband_size_v = mSubbandLF.mlength_v/mTransformLength_v;
    int subband_size_u = mSubbandLF.mlength_u/mTransformLength_u;
    
    
    int band = position_t/subband_size_t * mTransformLength_s * mTransformLength_v * mTransformLength_u;
    band += position_s/subband_size_s * mTransformLength_v * mTransformLength_u;
    band += mTransformLength_u * position_v/subband_size_v + position_u/subband_size_u;
    
    return(band);
}
    