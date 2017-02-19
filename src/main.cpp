#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>   
#include <opencv2/imgproc/imgproc.hpp> 

#include "include/laplacian_pyr.h"
#include "include/conv2d.h"
#include "include/derivative.h"

using namespace std;
using namespace cv;

#define B 0
#define G 1
#define R 2

#define LAPLACIAN_LEVELS 4

int main(int argc, char* argv[]) 
{        
    Mat img_c = imread("/home/gkopanas/cv/lucas-kanade/camel.jpg");  
    Mat img_g;
    cvtColor(img_c, img_g, CV_BGR2GRAY);

    vector<Mat> lapl(LAPLACIAN_LEVELS);
    LaplacianPyramid(&img_g, &lapl, LAPLACIAN_LEVELS);

    /*
    Mat out(img.rows, img.cols, CV_8UC3);
    for (int i=0; i <img.rows; i++) {
        for (int j=0; j<img.cols; j++) {
            out.data[i*out.step[0] + j*3 + R] = img.at<Vec3b>(i,j)[R];
            out.data[i*out.step[0] + j*3 + G] = img.at<Vec3b>(i,j)[G];
            out.data[i*out.step[0] + j*3 + B] = img.at<Vec3b>(i,j)[B];
        }
    }
    */

    Mat dx(img_g.rows, img_g.cols, CV_8UC1);
    derivativeFitler(&img_g, &dx, SCHARR, DX);

    Mat dy(img_g.rows, img_g.cols, CV_8UC1);
    derivativeFitler(&img_g, &dy, SCHARR, DY);

    namedWindow("Gaus0");
    imshow("Gaus0", lapl[0]);
    namedWindow("Gaus1");
    imshow("Gaus1", dx);
    namedWindow("Gaus4");
    imshow("Gaus4", dy);
    namedWindow("Gaus2");
    imshow("Gaus2", lapl[2]);
    namedWindow("Gaus3");
    imshow("Gaus3", lapl[3]);
    
    waitKey(0);

    return 0;
}
