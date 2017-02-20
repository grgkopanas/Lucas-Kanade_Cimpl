#include "include/lucas_kanade.h"
#include "include/matrix.h"

/*
* This function returns to out(float Mat) the diffrence between the pixels
* of the two images(t0, t1) if we dislocate t1 by (off_x, off_y).
* The vector dx and dy can be float. Bilinear Interpolations is used
* to access t1.
* The dislocation vector can throw the t1 accessing out of bounds,
* the we clamp on the edge. Assuming the two images have 
* the same dimensions. Assuming t0 and t1 have 1 channel (BW) 
*/
void imageLocationDiffrence(const Mat *t0, const Mat *t1,
                             Mat *out, double off_y, double off_x) {
    assert( t0->rows == t1->rows );
    assert( t0->cols == t1->cols ); 
    for (int i = 0; i < t0->rows; i++) {
        for (int j = 0; j < t0->cols; j++) {
            float i1,j1;
            i1 = std::max(0.0, i + off_y);
            i1 = std::min(t1->rows - 1.0, i + off_y);
            j1 = std::max(0.0, j + off_x);
            j1 = std::min(t1->cols - 1.0, j + off_x);
            float temp = t0->at<uchar>(i,j) - bilinearInterp(t1, i1, j1);
            out->at<float>(i,j) = temp;
        }
    }
}

/*
* This function returns to out the SpatialGradientmatrix defined as follows:
* The sum over a 2d window with dimensions OMEGAxOMEGA of the values:
* [ Idx^2        , Idx * Idy]
* [ Idx * Idxy   , Idy^2    ]
* Where Idx and Idy are the gradients over the x and y direction of an Image
* The output Matix is defined over 
* [OMEGA, Idx_width - OMEGA], [OMEGA, Idx_height - OMEGA]
*/
void spatialGradientMatrix(const Mat *Idx, const Mat *Idy, Mat *out) {

    assert (Idx->rows == Idy->rows);
    assert (Idx->cols == Idy->cols);
    assert (out->cols == Idx->cols - 2*OMEGA);
    assert (out->rows == Idx->rows - 2*OMEGA);
    assert (out->channels() == 4);

    for (int i=0; i<out->rows; i++) {
        for (int j=0; j<out->cols; j++) {
            float Sdx=0.0;
            float Sdxdy=0.0;
            float Sdy=0.0;
            for (int w_y = -OMEGA; w_y <= OMEGA; w_y++) {
                for (int w_x = -OMEGA; w_x <= OMEGA; w_x++) {
                    int i_d = (i + OMEGA) + w_y;
                    int j_d = (j + OMEGA) + w_x;
                    assert (i_d < Idx->rows);
                    assert (j_d < Idx->cols);
                    float dx = Idx->at<float>(i_d, j_d);
                    float dy =  Idy->at<float>(i_d, j_d);
                    Sdx += dx*dx;
                    Sdxdy += dx*dy;
                    Sdy += dy*dy;
                }
            }
            out->at<Vec4f>(i,j)[0] = Sdx;
            out->at<Vec4f>(i,j)[1] = Sdxdy; 
            out->at<Vec4f>(i,j)[2] = Sdxdy;
            out->at<Vec4f>(i,j)[3] = Sdy;
        }
    }
}

/*
* This function returns to out the imageMismatch Vector defined as follows:
* The sum over a 2d windows with dimensions OMEGAxOMEGA of the values:
* [delta_I*Idx]
* [delta_I*Idy]
* Where Idx is the imageLocationDiffrence Matrix between two images with a 
* dislocation vector attached to the second Image, and Idx, Idy are the 
* gradients over the x and y direction of an Image.
*/
void imageMismatchVector(const Mat *delta_I, const Mat *Idx, const Mat *Idy, Mat *out) {
    assert ( delta_I->rows == Idx->rows && delta_I->rows==Idy->rows );
    assert ( delta_I->cols == Idx->cols && delta_I->cols==Idy->cols );

    assert ( out->rows == delta_I->rows - 2*OMEGA);
    assert ( out->cols == delta_I->cols - 2*OMEGA);
    assert ( out->channels() == 2 );

    for (int i=0; i<out->rows; i++) {
        for (int j=0; j<out->cols; j++) {
            float Sx=0.0;
            float Sy=0.0;
            for (int w_y = -OMEGA; w_y <= OMEGA; w_y++) {
                for (int w_x = -OMEGA; w_x <= OMEGA; w_x++) {
                    int i_d = (i + OMEGA) + w_y;
                    int j_d = (j + OMEGA) + w_x;
                    assert (i_d < delta_I->rows);
                    assert (j_d < delta_I->cols);
                    float delta = delta_I->at<float>(i_d, j_d);
                    float dx = Idx->at<float>(i_d, j_d);
                    float dy = Idy->at<float>(i_d, j_d);
                    Sx += dx*delta;
                    Sy += dy*delta;
                }
            }
            out->at<Vec2f>(i,j)[0] = Sx;
            out->at<Vec2f>(i,j)[1] = Sy; 
        }
    }
    
}
