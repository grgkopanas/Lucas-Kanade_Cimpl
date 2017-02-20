#include "include/matrix.h"

float bilinearInterp(const Mat *img, const float y, const float x) {

    assert( y < img->rows );
    assert( x < img->cols );

    int x_0 = (int) x;
    int y_0 = (int) y;

    float a_x = x - x_0;
    float a_y = y - y_0;

    float x_next = min(x_0 + 1, img->cols);
    float y_next = min(y_0 + 1, img->rows);

    float t1 = (1 - a_x) * (1 - a_y) * img->at<uchar>(y_0, x_0);
    float t2 = a_x * (1 - a_y) * img->at<uchar>(y_0, x_next);
    float t3 = (1 - a_x) * a_y * img->at<uchar>(y_next, x_0);
    float t4 = a_x * a_y * img->at<uchar>(y_next, x_next);

    return t1 + t2 + t3 + t4;
}

float absoluteSum(const Mat *kernel) {
    float sum=0.0;

    for (int i = 0; i < kernel->rows; i++) {
        for (int j = 0; j < kernel->cols; j++) {
                sum += abs(kernel->at<float>(i,j));
        }
    }

    return sum;
}

void invert2by2Mat(Vec4f *m, Vec4f *m_inv ) {
    float a = m->val[0];
    float b = m->val[1];
    float c = m->val[2];
    float d = m->val[3];

    float det = 1/(a*d - b*c); 
    m_inv->val[0] =  det*d;
    m_inv->val[1] = -det*b;
    m_inv->val[2] = -det*c;
    m_inv->val[3] =  det*a;
}
