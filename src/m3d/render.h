#ifndef __M3D_RENDER_H__
#define __M3D_RENDER_H__

    #include <stdlib.h>
    #include <stdio.h>
    #include <stdarg.h>

    #include "cross_gl.h"
    #include "mesh.h"
    #include "font.h"

    void m3d_RenderMesh(m3d_Mesh *mesh);
    void m3d_RenderText(m3d_Font *font, int xp, int yp, char *string, ...);
    void m3d_RenderQuad();

#endif // __M3D_RENDER_H__
