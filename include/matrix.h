#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <opencv2/core/core.hpp>
using namespace cv;

float absoluteSum(const Mat *kernel);
float bilinearInterp(const Mat *img, const float y, const float x);

#endif