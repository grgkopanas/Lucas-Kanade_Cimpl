#ifndef CONV2D_H_INCLUDED
#define CONV2D_H_INCLUDED

#include <opencv2/core/core.hpp>
using namespace cv;

float conv2d(const int y, const int x, const Mat *img, const Mat *kernel);

#endif