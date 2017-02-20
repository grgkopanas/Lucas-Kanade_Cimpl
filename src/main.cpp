#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>   
#include <opencv2/imgproc/imgproc.hpp> 

#include "include/laplacian_pyr.h"
#include "include/conv2d.h"
#include "include/derivative.h"
#include "include/matrix.h"
#include "include/lucas_kanade.h"

using namespace std;
using namespace cv;

#define B 0
#define G 1
#define R 2

#define LAPLACIAN_LEVELS 4

int main(int argc, char* argv[]) 
{        
    Mat img_c = imread("/work/personal/cv/Lucas-Kanade_Cimpl/eval-data/Army/frame10.png");  
    Mat img_g;
    cvtColor(img_c, img_g, CV_BGR2GRAY);

    Mat img_c1 = imread("/work/personal/cv/Lucas-Kanade_Cimpl/eval-data/Army/frame11.png");  
    Mat img_g1;
    cvtColor(img_c1, img_g1, CV_BGR2GRAY);

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

    Mat dx(img_g.rows, img_g.cols, CV_32F);
    derivativeFitler(&img_g, &dx, CENTRAL, DX);

    Mat dy(img_g.rows, img_g.cols, CV_32F);
    derivativeFitler(&img_g, &dy, CENTRAL, DY);

    Mat SGM(img_g.rows - 2*OMEGA, img_g.cols - 2*OMEGA, CV_32FC4);
    spatialGradientMatrix(&dx, &dy, &SGM);

    Mat Id(img_g.rows, img_g.cols, CV_32F);
    imageLocationDiffrence(&img_g, &img_g, &Id, 0.5, 0.5);
    Mat Id1(img_g.rows, img_g.cols, CV_32F);
    imageLocationDiffrence(&img_g, &img_g, &Id1, 0.1, 0.1);

    namedWindow("Gaus0");
    imshow("Gaus0", lapl[0]);
    namedWindow("Gaus1");
    imshow("Gaus1", (dx+128)/255.0);
    namedWindow("Gaus4");
    imshow("Gaus4", (dy+128)/255.0);
    namedWindow("Gaus2");
    imshow("Gaus2", lapl[2]);
    namedWindow("Gaus3");
    imshow("Gaus3", lapl[3]);
    namedWindow("Id");
    imshow("Id", Id/255.0 + 1);
    namedWindow("Id1");
    imshow("Id1", Id1/255.0 + 1);
    waitKey(0);

    return 0;
}
