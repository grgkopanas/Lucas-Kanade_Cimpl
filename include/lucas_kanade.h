#ifndef LUCAS_KANADE_H_INCLUDED
#define LUCAS_KANADE_H_INCLUDED

#include <opencv2/core/core.hpp>
using namespace cv;

void imageLocationDiffrence(const Mat *t0, const Mat *t1,
                             Mat *out, double dy, double dx);

#endif