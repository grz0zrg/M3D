#ifndef __M3D_H__
#define __M3D_H__

    #include <stdlib.h>
    #include <stdio.h>

    #include "define.h"
    #include "cross_gl.h"
    #ifdef GP2X_WIZ
		#include <sdl.h>
    #endif

    #include "benchmark.h"
    #include "camera.h"
    #include "image.h"
    #include "mesh.h"
    #include "texture.h"
    #include "font.h"
    #include "util.h"
    #include "render.h"

    void m3d_Initialize(int screen_width, int screen_height);
    void m3d_Free();

    void m3d_SwapBuffers();
    void m3d_SetViewport(int x, int y, int w, int h);
    void m3d_UseCamera(m3d_Camera *camera);

    inline int m3d_getScreenWidth();
    inline int m3d_getScreenHeight();

    m3d_Texture *m3d_GetTexture(const char *filename);
    m3d_Image   *m3d_GetImage  (const char *filename);
    m3d_Mesh    *m3d_GetMesh   (const char *filename);
    m3d_Font    *m3d_GetFont   (const char *filename);
    void m3d_DropTexture       (const char *filename);
    void m3d_DropImage         (const char *filename);
    void m3d_DropMesh          (const char *filename);
    void m3d_DropFont          (const char *filename);

    void m3d_ClearColor(float r, float g, float b, float a);
    void m3d_SetFlatShading();
    void m3d_SetSmoothShading();
    void m3d_SetNicest(GLenum name);
    void m3d_SetFastest(GLenum name);
    void m3d_Enable(GLenum cap);
    void m3d_Disable(GLenum cap);
    void m3d_UseFrontFaceCW();
    void m3d_UseFrontFaceCCW();
    void m3d_CullFrontFace();
    void m3d_CullBackFace();
    void m3d_CullFace();

    double m3d_GetTime();

    int m3d_IsRunning();

#endif // __M3D_H__
