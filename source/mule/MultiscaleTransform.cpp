#include "MultiscaleTransform.h"
/*******************************************************************************/
/*                        MultiscaleTransform class methods                    */
/*******************************************************************************/
MultiscaleTransform :: MultiscaleTransform(void) {
    
    mlength_t_max = 0;
    mlength_s_max = 0;
    mlength_v_max = 0;
    mlength_u_max = 0;
    mPartitionCode = NULL;
    mTransform = NULL;
    mTransformGain_t = 1.0;
    mTransformGain_s = 1.0;
    mTransformGain_v = 1.0;
    mTransformGain_u = 1.0;
   
}
MultiscaleTransform :: ~MultiscaleTransform(void) {
 
    if(mPartitionCode != NULL) {
        delete [] mPartitionCode;
    }
    if(mTransform != NULL) {
        delete [] mTransform;
    }
    
}

void MultiscaleTransform :: SetDimension(int max_t, int max_s, int max_v, int max_u, int min_t, int min_s, int min_v, int min_u) {

    
    if((min_t > max_t)||(min_s > max_s)||(min_v > max_v)||(min_u > max_u)) {
        printf("ERROR: minimum transform length is greater than the maximum\n");
        exit(0);
    }
    
    if(mPartitionCode != NULL) {
        delete [] mPartitionCode;
        mPartitionCode = NULL;
    }
    if(mTransform != NULL) {
        for(int n = 0; n < mNumberOfScales; n++) {
            mTransform[n].SetDimension(0,0);
        }
        delete [] mTransform;
        mTransform = NULL;
    }
    
    if(max_t*max_s*max_v*max_u == 0) {
        mlength_t_max = 0;
        mlength_s_max = 0;
        mlength_v_max = 0;
        mlength_u_max = 0;
        return;
    }
    
    //calculate the maximum possible number of different transform sizes
    int depth_t = 0;
    int length = max_t;
    while(length > min_t) {
        length = length/2;
        depth_t++;
    }
    int depth_s = 0;
    length = max_s;
    while(length > min_s) {
        length = length/2;
        depth_s++;
    }
    int depth_v = 0;
    length = max_v;
    while(length > min_v) {
        length = length/2;
        depth_v++;
    }
    int depth_u = 0;
    length = max_u;
    while(length > min_u) {
        length = length/2;
        depth_u++;
    }
    
    int *lengths_list = new int [2*(depth_t+1)*(depth_s+1)*(depth_v+1)*(depth_u+1)];
    int number_of_different_lengths = 0;
    
    AddSublengthsToList(max_t, min_t, lengths_list, number_of_different_lengths);
    AddSublengthsToList(max_s, min_s, lengths_list, number_of_different_lengths);
    AddSublengthsToList(max_v, min_v, lengths_list, number_of_different_lengths);
    AddSublengthsToList(max_u, min_u, lengths_list, number_of_different_lengths);
    
    for(int n = 0; n < number_of_different_lengths; n++) {
        for(int next_index = n+1; next_index < number_of_different_lengths; next_index++) {
            if(lengths_list[n] < lengths_list[next_index]) {
                int greater_length = lengths_list[next_index];
                lengths_list[next_index] = lengths_list[n];
                lengths_list[n] = greater_length;
            }
        }
    }
    
    mNumberOfScales = number_of_different_lengths;
    mTransform = new Matrix [mNumberOfScales];
    for(int n = 0; n < mNumberOfScales; n++) {
        mTransform[n].SetDimension(lengths_list[n], lengths_list[n]);
printf("lengths_list[%d] = %d\n", n, lengths_list[n]) ;       
    }
   
    
    mlength_t_max = max_t;
    mlength_s_max = max_s;
    mlength_v_max = max_v;
    mlength_u_max = max_u;

    mlength_t_min = min_t;
    mlength_s_min = min_s;
    mlength_v_min = min_v;
    mlength_u_min = min_u;
    
}

void MultiscaleTransform :: LoadDCT(void) {
/*! loads the DCT coefficients to all matrices of the mTransform array */    
    for(int n = 0; n < mNumberOfScales; n++) {
        mTransform[n].DCT();
    }
}

void MultiscaleTransform :: LoadIDCT(void) {
/*! loads the IDCT coefficients to all matrices of the mTransform array */    
    for(int n = 0; n < mNumberOfScales; n++) {
        mTransform[n].IDCT();
    }
    
}

void MultiscaleTransform :: MultiscaleTransformBlock4D(Block4D &originalBlock, Block4D &transformedBlock) {
    
}

void MultiscaleTransform :: AddSublengthsToList(int max_length, int min_length, int *list, int &list_size) {
   
    if(max_length < min_length) {
        return;
    }
    
    int already_in_the_list = 0;
    for(int search_index = 0; search_index < list_size; search_index++) {
        if(max_length == list[search_index]) {
            already_in_the_list = 1;
            search_index = list_size+1;
        }
    }
    if(already_in_the_list == 0) {
        list[list_size] = max_length;
        list_size++;
    }
    if(max_length < 2) {
        return;
    }
    AddSublengthsToList(max_length/2, min_length, list, list_size);
    if(max_length%2 == 1) {
        AddSublengthsToList(max_length - max_length/2, min_length, list, list_size);
    }
    
}

double* MultiscaleTransform :: TransformCoefficients(int length) {
    
    for(int scale = 0; scale < mNumberOfScales; scale++) {
        if(length == mTransform[scale].mLines) {
            return mTransform[scale].mElementData;
        }
    }
    return(NULL);
}

void MultiscaleTransform :: Transform4D(Block4D &block) {
  
    double transform_gain_u = 1.0*mlength_u_max;
    transform_gain_u = sqrt(transform_gain_u/block.mlength_u);
    block.TRANSFORM_U(mTransformGain_u*transform_gain_u, TransformCoefficients(block.mlength_u));
    double transform_gain_v = 1.0*mlength_v_max;
    transform_gain_v = sqrt(transform_gain_v/block.mlength_v);
    block.TRANSFORM_V(mTransformGain_v*transform_gain_v, TransformCoefficients(block.mlength_v));
    double transform_gain_s = 1.0*mlength_s_max;
    transform_gain_s = sqrt(transform_gain_s/block.mlength_s);
    block.TRANSFORM_S(mTransformGain_s*transform_gain_s, TransformCoefficients(block.mlength_s));
    double transform_gain_t = 1.0*mlength_t_max;
    transform_gain_t = sqrt(transform_gain_t/block.mlength_t);
    block.TRANSFORM_T(mTransformGain_t*transform_gain_t, TransformCoefficients(block.mlength_t)); 
    
}

void MultiscaleTransform :: InverseTransform4D(Block4D &block) {
    
    double transform_gain_u = 1.0*mlength_u_max;
    transform_gain_u = sqrt(transform_gain_u/block.mlength_u);
    block.TRANSFORM_U(mTransformGain_u/transform_gain_u, TransformCoefficients(block.mlength_u));
    double transform_gain_v = 1.0*mlength_v_max;
    transform_gain_v = sqrt(transform_gain_v/block.mlength_v);
    block.TRANSFORM_V(mTransformGain_v/transform_gain_v, TransformCoefficients(block.mlength_v));
    double transform_gain_s = 1.0*mlength_s_max;
    transform_gain_s = sqrt(transform_gain_s/block.mlength_s);
    block.TRANSFORM_S(mTransformGain_s/transform_gain_s, TransformCoefficients(block.mlength_s));
    double transform_gain_t = 1.0*mlength_t_max;
    transform_gain_t = sqrt(transform_gain_t/block.mlength_t);
    block.TRANSFORM_T(mTransformGain_t/transform_gain_t, TransformCoefficients(block.mlength_t));   
   
}
