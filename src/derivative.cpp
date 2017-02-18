#include "include/derivative.h"
#include "include/conv2d.h"

void derivativeFitler(const Mat *img, Mat *deriv, int direction) {
    const float dx_filter[3] = DERIV_FILTER;
    Mat kernel;
    if (direction == DX) {
       kernel = Mat(1, 3, CV_32F, const_cast<float *>(dx_filter));
    }
    else if (direction == DY) {
        kernel = Mat(3, 1, CV_32F, const_cast<float *>(dx_filter));
    }

    for (int i = 1; i < img->rows-1; i++) {
        for (int j = 1; j < img->cols-1; j++) {
            deriv->at<uchar>(i,j) = uchar(conv2d(i, j, img, &kernel)/2.0 + 128 + 0.5); 
        }
    }

    for (int j = 1; j < deriv->cols-1; j++) {
        deriv->at<uchar>(0,j) = deriv->at<uchar>(1,j);
        deriv->at<uchar>(deriv->rows-1, j) = deriv->at<uchar>(deriv->rows-2, j);
    }
    for (int j = 1; j < deriv->rows-1; j++) {
        deriv->at<uchar>(j, 0) = deriv->at<uchar>(j, 1);
        deriv->at<uchar>(j, deriv->cols-1) = deriv->at<uchar>(j, deriv->cols-2);
    }

    deriv->at<uchar>(0, 0) = deriv->at<uchar>(1, 1);
    deriv->at<uchar>(deriv->rows-1, 0) = deriv->at<uchar>(deriv->rows-2, 1);
    deriv->at<uchar>(0, deriv->cols-1) = deriv->at<uchar>(1, deriv->cols-2);
    deriv->at<uchar>(deriv->rows-1, deriv->cols-1) = deriv->at<uchar>(deriv->rows-2, deriv->cols-2); 
}