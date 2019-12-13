#include "Matrix.h"
#include "Block4D.h"
#include <math.h>
#include <stdlib.h>

#ifndef MULTISCALETRANSFORM_H
#define MULTISCALETRANSFORM_H

class MultiscaleTransform {
public:
    int mlength_t_max, mlength_s_max;               /*!< maximum transform size at directions t, s */
    int mlength_v_max, mlength_u_max;               /*!< maximum transform size at directions v, u */
    int mlength_t_min, mlength_s_min;               /*!< minimum transform size at directions t, s */
    int mlength_v_min, mlength_u_min;               /*!< minimum transform size at directions v, u */
    int mPartitionDepth_t, mPartitionDepth_s;       /*!< maximum depth of the partition tree at directions t, s */
    int mPartitionDepth_v, mPartitionDepth_u;       /*!< maximum depth of the partition tree at directions v, u */
    char *mPartitionCode;                           /*!< partition code string */
    Matrix *mTransform;                             /*!< array of transform matrices for every possible transform size  */
    int mNumberOfScales;                            /*!< size of the array of transform matrices  */
    double mTransformGain_t, mTransformGain_s;      /*!< gain of the transform at directions t, s */ 
    double mTransformGain_v, mTransformGain_u;      /*!< gain of the transform at directions v, u */ 
    MultiscaleTransform(void);
    ~MultiscaleTransform(void);
    void SetDimension(int max_t, int max_s, int max_v, int max_u, int min_t = 1, int min_s = 1, int min_v = 4, int min_u = 4);
    void LoadDCT(void);                             
    void LoadIDCT(void);                            /*!< loads the IDCT coefficients to all matrices of the mTransform array*/
    void MultiscaleTransformBlock4D(Block4D &originalBlock, Block4D &transformedBlock);
    void AddSublengthsToList(int max_length, int min_length, int *list, int &list_size);
    double* TransformCoefficients(int length);
    void Transform4D(Block4D &block);
    void InverseTransform4D(Block4D &block);
};

#endif /* MULTISCALETRANSFORM_H */

