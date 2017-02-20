#include "include/lucas_kanade.h"
#include "include/matrix.h"

/*
* This function returns to out(float Mat) the diffrence between the pixels
* of the two images(t0, t1) if we dislocate t1 by (dx,dy).
* The vector dx and dy can be float. Bilinear Interpolations is used
* to access t1.
* The dislocation vector can throw the t1 accessing out of bounds,
* the we clamp on the edge. Assuming the two images have 
* the same dimensions. Assuming t0 and t1 have 1 channel (BW) 
*/
void imageLocationDiffrence(const Mat *t0, const Mat *t1,
                             Mat *out, double dy, double dx) {
    assert( t0->rows == t1->rows );
    assert( t0->cols == t1->cols ); 
    for (int i = 0; i < t0->rows; i++) {
        for (int j = 0; j < t0->cols; j++) {
            float i1,j1;
            i1 = std::max(0.0, i + dy);
            i1 = std::min(t1->rows - 1.0, i+dy);
            j1 = std::max(0.0, j + dx);
            j1 = std::min(t1->cols - 1.0, j+dx);
            float temp = t0->at<uchar>(i,j) - bilinearInterp(t1, i1, j1);
            out->at<float>(i,j) = temp;
        }
    }
}