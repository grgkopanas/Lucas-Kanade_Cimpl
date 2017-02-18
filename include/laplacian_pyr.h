#ifndef LAPLACIAN_PYR_H_INCLUDED
#define LAPLACIAN_PYR_H_INCLUDED

#include <opencv2/core/core.hpp>
using namespace cv;

#define AA_KERNEL {1.0/16.0, 1.0/8.0, 1.0/16.0, \
                   1.0/8.0,  1.0/4.0, 1.0/8.0, \
                   1.0/16.0, 1.0/8.0, 1.0/16.0}

void filterLaplacianPyramid(Mat *lapl, const Mat *img, const Mat *kernel);
void LaplacianPyramid(const Mat *img, vector<Mat> *lapl, const int levels);

#endif