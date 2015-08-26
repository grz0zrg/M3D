#include "camera.h"

m3d_Camera *m3d_CameraInit(m3d_CameraType type, float fov, float aspect,
                           float near, float far) {
	m3d_Camera *camera = (m3d_Camera *)malloc(sizeof(m3d_Camera));

	if (!camera) return NULL;

	memset(camera, 0, sizeof(m3d_Camera));

	camera->type   = type;
	camera->fov    = fov;
	camera->aspect = aspect;
	camera->near   = near;
	camera->far    = far;

    if (camera->type == M3D_CAMERA_PERSPECTIVE) {
        float ymax = near * tanf(m3d_UtilDeg2Rad(fov * 0.5f));
        float ymin = -ymax;
        float xmin = ymin * aspect;
        float xmax = ymax * aspect;

        m3d_MatrixMakeFrustrum(&camera->projection_matrix,
                               xmin, xmax, ymin, ymax, near, far);
    } else { // ortho
        float left   = fov;
        float right  = aspect;
        float top    = near;
        float bottom = far;
        m3d_MatrixMakeOrtho(&camera->projection_matrix,
                            left, right, top, bottom, near, far);
    }

	return camera;
}

void m3d_CameraFree(m3d_Camera *camera) {
	free(camera);
}
