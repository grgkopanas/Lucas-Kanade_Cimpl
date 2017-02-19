#ifndef DERIVATIVE_PYR_H_INCLUDED
#define DERIVATIVE_PYR_H_INCLUDED

#include <opencv2/core/core.hpp>
using namespace cv;

#define DX 0
#define DY 1
#define CENTRAL 0
#define SCHARR 1

#define DERIV_FILTER {-1.0, 0.0, 1.0}

#define SCHARR_FILTER_DX {-3.0, -10.0, -3.0,\
                           0.0,   0.0,  0.0,\
                           3.0,  10.0,  3.0}

#define SCHARR_FILTER_DY {-3.0, 0.0, 3.0,\
                         -10.0, 0.0, 10.0,\
                          -3.0, 0.0, 3.0}
            
void prepareScharrKernel(Mat *kernel, int direction);
void prepareCentralKernel(Mat *kernel, int direction);
void derivativeFitler(const Mat *img, Mat *deriv, int type, int direction);

#endif