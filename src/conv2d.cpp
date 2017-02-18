#include "include/conv2d.h"

float conv2d(const int y, const int x, const Mat *img, const Mat *kernel) {
    //Assuming kernel is normalized
    //assert kernel3f
    //assert values are not oor

    float val = 0.0;
    int off_y = kernel->rows/2;
    int off_x = kernel->cols/2;
    for (int i = -off_y; i <= off_y; i++ ) {
        for (int j = -off_x; j <= off_x; j++) {
            val += img->at<uchar>(y+i, x+j)*kernel->at<float>(i+off_y, j+off_x);
        }
    }
    //No normalization, what happens with values over this is normilized kernel
    return val;
}