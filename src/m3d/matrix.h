#ifndef __M3D_MATRIX_H__
#define __M3D_MATRIX_H__

    #include <math.h>

    typedef struct {
        float m[16];
    } m3d_Matrix;

    typedef struct {
        int m[16];
    } m3d_MatrixFixed;

    void m3d_MatrixMakeFrustrum(m3d_Matrix *matrix,
                            float left,   float right,
                            float bottom, float top,
                            float near,   float far);

    void m3d_MatrixMakeOrtho(m3d_Matrix *matrix,
                                float left,   float right,
                                float bottom, float top,
                                float near,   float far);

    inline void m3d_MatrixMakeIdentity(m3d_Matrix *matrix);
    inline void m3d_MatrixTranslate(m3d_Matrix *matrix,
                                    float x, float y, float z);
    inline void m3d_MatrixRotate(m3d_Matrix *matrix, float x, float y, float z);
    inline void m3d_MatrixScale(m3d_Matrix *matrix,
                                float sx, float sy, float sz);

#endif // __M3D_MATRIX_H__
