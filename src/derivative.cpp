#include "include/derivative.h"
#include "include/conv2d.h"
#include "include/matrix.h"

void prepareCentralKernel(Mat *kernel, int direction) {
    int dim_x;
    int dim_y;

    const float filter[3] = DERIV_FILTER;
    if (direction == DX) {
        dim_y = 1;
        dim_x = 3;
    }
    else {
        dim_y = 3;
        dim_x = 1;
    }
    *kernel = Mat(dim_y, dim_x, CV_32F, const_cast<float *>(filter));
}

void prepareScharrKernel(Mat *kernel, int direction) {
    int dim_x = 3;
    int dim_y = 3;

    if (direction == DX) {
        const float filter[9] = SCHARR_FILTER_DX;
        *kernel = Mat(dim_y, dim_x, CV_32F, const_cast<float *>(filter));
    }
    else if (direction == DY) {
        const float filter[9] = SCHARR_FILTER_DY;
        *kernel = Mat(dim_y, dim_x, CV_32F, const_cast<float *>(filter));
    }
}

void derivativeFitler(const Mat *img, Mat *deriv, int type, int direction) {
    Mat kernel;
    
    switch (type) {
    case SCHARR:
        prepareScharrKernel(&kernel, direction);
        break;
    case CENTRAL:
        prepareCentralKernel(&kernel, direction);
        break;
    }
    float norm_factor = absoluteSum(&kernel);

    for (int i = 1; i < img->rows-1; i++) {
        for (int j = 1; j < img->cols-1; j++) {
            deriv->at<float>(i,j) = conv2d(i, j, img, &kernel)/norm_factor; 
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