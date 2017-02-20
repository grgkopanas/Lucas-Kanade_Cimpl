#ifndef LUCAS_KANADE_H_INCLUDED
#define LUCAS_KANADE_H_INCLUDED

#include <opencv2/core/core.hpp>
using namespace cv;

#define OMEGA 6

void imageLocationDiffrence(const Mat *t0, const Mat *t1,
                             Mat *out, double off_y, double off_x);
void spatialGradientMatrix(const Mat *Idx, const Mat *Idy, Mat *out);
void imageMismatchVector(const Mat *delta_I, const Mat *Idx,
                             const Mat *Idy, const Mat *out);

#endif