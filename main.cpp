#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>    

using namespace std;
using namespace cv;

#define B 0
#define G 1
#define R 2

#define LAPLACIAN_LEVELS 4

const float aa_filter[9] {1.0/16.0, 1.0/8.0, 1.0/16.0,
                          1.0/8.0,  1.0/4.0, 1.0/8.0,
                          1.0/16.0, 1.0/8.0, 1.0/16.0};

Vec3b conv2d (int y, int x, Mat *img, Mat kernel) {
    //Assuming kernel is normalized
    //assert kernel3f
    //assert values are not oor

    Vec3f val(0.0, 0.0, 0.0);
    int off_y = kernel.rows/2;
    int off_x = kernel.cols/2;
    for (int i = -off_y; i <= off_y; i++ ) {
        for (int j = -off_x; j <= off_x; j++) {
            val += img->at<Vec3b>(y+i,x+j)*kernel.at<float>(i+off_y, j+off_x);
        }
    }
    //No normalization, what happens with values over this is normilized kernel
    return Vec3b(val);
}

void generateLaplacianLevel(Mat *lapl, Mat *img) {
    Mat kernel(3,3, CV_32F, const_cast<float *>(aa_filter));
    //assuming 3x3 kernel
    for (int i = 1; i < lapl->rows-1; i++) {
        for (int j=1; j < lapl->cols-1; j++) {
            lapl->at<Vec3b>(i,j) = conv2d(i*2, j*2, img, kernel);
        }
    }

    for (int j = 1; j < lapl->cols-1; j++) {
        lapl->at<Vec3b>(0,j) = lapl->at<Vec3b>(1,j);
        lapl->at<Vec3b>(lapl->rows-1, j) = lapl->at<Vec3b>(lapl->rows-2, j);
    }
    for (int i = 1; i < lapl->rows-1; i++) {
        lapl->at<Vec3b>(i,0) = lapl->at<Vec3b>(i,1);
        lapl->at<Vec3b>(i, lapl->cols-1) = lapl->at<Vec3b>(i, lapl->cols-2);
    }
    lapl->at<Vec3b>(0, 0) = lapl->at<Vec3b>(1, 1);
    lapl->at<Vec3b>(lapl->rows-1, 0) = lapl->at<Vec3b>(lapl->rows-2, 1);
    lapl->at<Vec3b>(0, lapl->cols-1) = lapl->at<Vec3b>(1, lapl->cols-2);
    lapl->at<Vec3b>(lapl->rows-1, lapl->cols-1) = lapl->at<Vec3b>(lapl->rows-2, lapl->cols-2);
    
}

void LaplacianPyramid(const Mat img, vector<Mat> *lapl, int levels) {
    lapl->at(0) = img;
    for (int lev=1; lev<levels; lev++) {
        Mat *prev_lvl = &lapl->at(lev-1);
        lapl->at(lev) = Mat(prev_lvl->rows/2, prev_lvl->cols/2, CV_8UC3);
        Mat *curr_lvl = &lapl->at(lev);
        generateLaplacianLevel(curr_lvl, prev_lvl);
    }
}

int main(int argc, char* argv[]) 
{        
    const Mat img= imread("/home/gkopanas/cv/lucas-kanade/camel.jpg");  

    vector<Mat> lapl(LAPLACIAN_LEVELS);
    LaplacianPyramid(img, &lapl, LAPLACIAN_LEVELS);

    namedWindow("Gaus0");
    imshow("Gaus0", lapl[0]);
    namedWindow("Gaus1");
    imshow("Gaus1", lapl[1]);
    namedWindow("Gaus2");
    imshow("Gaus2", lapl[2]);
    namedWindow("Gaus3");
    imshow("Gaus3", lapl[3]);
    waitKey(0);

    return 0;
}