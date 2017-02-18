#include "include/laplacian_pyr.h"
#include "include/conv2d.h"

void filterLaplacianPyramid(Mat *lapl, const Mat *img, const Mat *kernel) {
    
    assert(kernel->rows == kernel->cols && "Non-Square kernels are not support for Laplacian yet.");

    int edges = kernel->rows/2;
    for (int i = edges; i < lapl->rows-edges; i++) {
        for (int j = edges; j < lapl->cols-edges; j++) {
            lapl->at<uchar>(i,j) = (conv2d(i*2, j*2, img, kernel) + 0.5);
        }
    }
    // TODO: Optimize this with texture memory
    /* Filling the Horizontal edges */
    for (int i = 0; i < edges; i++) {
        for (int j = 1; j < lapl->cols-1; j++) {
            lapl->at<uchar>(i,j) = lapl->at<uchar>(edges,j);
            lapl->at<uchar>(lapl->rows-(1+i), j) = lapl->at<uchar>(lapl->rows-(1+edges), j);
        }
    }
    /* Filling the Vertical edges */
    for (int i = 0; i < edges; i++) {
        for (int j = 1; j < lapl->rows-1; j++) {
            lapl->at<uchar>(j, i) = lapl->at<uchar>(j, edges);
            lapl->at<uchar>(j, lapl->cols-(1+i)) = lapl->at<uchar>(j, lapl->cols-(1+edges));
        }
    }
    /* Filling the four corners */
    for (int i=0; i<edges; i++) {
        lapl->at<uchar>(i, i) = lapl->at<uchar>(edges, edges);
        lapl->at<uchar>(lapl->rows-(1+i), i) = lapl->at<uchar>(lapl->rows-(1+edges), edges);
        lapl->at<uchar>(i, lapl->cols-(1+i)) = lapl->at<uchar>(edges, lapl->cols-(1+edges));
        lapl->at<uchar>(lapl->rows-(1+i), lapl->cols-(1+i)) = lapl->at<uchar>(lapl->rows-(1+edges), lapl->cols-(1+edges)); 
    }
}


void LaplacianPyramid(const Mat *img, vector<Mat> *lapl, const int levels) {
    const float aa_filter[9] = AA_KERNEL;

    const Mat kernel(3,3, CV_32F, const_cast<float *>(aa_filter));
    lapl->at(0) = img[0];
    for (int lev=1; lev<levels; lev++) {
        const Mat *prev_lvl = &lapl->at(lev-1);
        lapl->at(lev) = Mat(prev_lvl->rows/2, prev_lvl->cols/2, CV_8UC1);
        Mat *curr_lvl = &lapl->at(lev);
        filterLaplacianPyramid(curr_lvl, prev_lvl, &kernel);
    }
}