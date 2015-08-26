#ifndef __M3D_CAMERA_H__
#define __M3D_CAMERA_H__

    #include <stdlib.h>
    #include <string.h>

    #include "matrix.h"
    #include "util.h"

    #define m3d_CameraType int
    #define M3D_CAMERA_ORTHOGRAPHIC 0
    #define M3D_CAMERA_PERSPECTIVE  1

    typedef struct {
        m3d_Matrix projection_matrix;

        m3d_CameraType type;
        float fov, aspect, near, far;
    } m3d_Camera;

    // if ORTHOGRAPHIC : type, left, right, top, bottom, near, far
    m3d_Camera *m3d_CameraInit(m3d_CameraType type, float fov, float aspect,
                           float near, float far);
    void m3d_CameraFree(m3d_Camera *camera);

#endif // __M3D_CAMERA_H__
