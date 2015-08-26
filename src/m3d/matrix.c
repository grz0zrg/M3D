#include "matrix.h"

void m3d_MatrixMakeFrustrum(m3d_Matrix *matrix,
                            float left,   float right,
                            float bottom, float top,
                            float near,   float far) {
    if (!matrix) return;

    float *m = matrix->m;

    float x = 2 * near / ( right - left );
    float y = 2 * near / ( top - bottom );

    float a = (right + left) / (right - left);
    float b = (top + bottom) / (top - bottom);
    float c = -(far + near) / (far - near);
    float d = -2 * far * near / (far - near);

    m[0] = x; m[4] = 0; m[8 ] = a;  m[12] = 0;
    m[1] = 0; m[5] = y; m[9 ] = b;  m[13] = 0;
    m[2] = 0; m[6] = 0; m[10] = c;  m[14] = d;
    m[3] = 0; m[7] = 0; m[11] = -1; m[15] = 0;
}

void m3d_MatrixMakeOrtho(m3d_Matrix *matrix,
                            float left,   float right,
                            float bottom, float top,
                            float near,   float far) {
    if (!matrix) return;

    float *m = matrix->m;

    float w = right - left;
    float h = top - bottom;
    float p = far - near;

    float x = (right + left) / w;
    float y = (top + bottom) / h;
    float z = (far + near) / p;

    m[0] = 2 / w; m[4] = 0;     m[8 ] = 0;    m[12] = -x;
    m[1] = 0;     m[5] = 2 / h; m[9 ] = 0;    m[13] = -y;
    m[2] = 0;     m[6] = 0;     m[10] = -2/p; m[14] = -z;
    m[3] = 0;     m[7] = 0;     m[11] = 0;    m[15] = 1;
}

inline void m3d_MatrixMakeIdentity(m3d_Matrix *matrix) {
    float *m = matrix->m;

    m[0] = 1; m[4] = 0; m[8 ] = 0; m[12] = 0;
    m[1] = 0; m[5] = 1; m[9 ] = 0; m[13] = 0;
    m[2] = 0; m[6] = 0; m[10] = 1; m[14] = 0;
    m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1;
}

inline void m3d_MatrixTranslate(m3d_Matrix *matrix, float x, float y, float z) {
    matrix->m[12] = x;
    matrix->m[13] = y;
    matrix->m[14] = z;
}

inline void m3d_MatrixRotate(m3d_Matrix *matrix, float x, float y, float z) {
    float a = cosf(x), b = sinf(x);
    float c = cosf(y), d = sinf(y);
    float e = cosf(z), f = sinf(z);

    float ac = a * c, ad = a * d, bc = b * c, bd = b * d;

    float *m = matrix->m;

    m[0] = c * e;
    m[4] = bd - ac * f;
    m[8] = bc * f + ad;

    m[1] = f;
    m[5] = a * e;
    m[9] = - b * e;

    m[2] = - d * e;
    m[6] = ad * f + bc;
    m[10] = ac - bd * f;

    m[3] = 0;
    m[7] = 0;
    m[11] = 0;
/*
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;*/
}

inline void m3d_MatrixScale(m3d_Matrix *matrix, float sx, float sy, float sz) {
    float *m = matrix->m;
    m[0] *= sx; m[4] *= sy; m[8 ] *= sz;
    m[1] *= sx; m[5] *= sy; m[9 ] *= sz;
    m[2] *= sx; m[6] *= sy; m[10] *= sz;
    m[3] *= sx; m[7] *= sy; m[11] *= sz;
}
