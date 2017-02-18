#ifndef DERIVATIVE_PYR_H_INCLUDED
#define DERIVATIVE_PYR_H_INCLUDED

#include <opencv2/core/core.hpp>
using namespace cv;

#define DERIV_FILTER {-1.0, 0.0, 1.0}

#define DX 0
#define DY 1

void derivativeFitler(const Mat *img, Mat *deriv, int direction);

#endif