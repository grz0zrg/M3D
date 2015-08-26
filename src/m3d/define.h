#ifndef __M3D_DEFINE_H__
#define __M3D_DEFINE_H__

    #include "cross_gl.h"

    // textures param pname
    #define M3D_TEX_MIN_FILTER GL_TEXTURE_MIN_FILTER
    #define M3D_TEX_MAG_FILTER GL_TEXTURE_MAG_FILTER
    #define M3D_TEX_WRAP_S GL_TEXTURE_WRAP_S
    #define M3D_TEX_WRAP_T GL_TEXTURE_WRAP_T

    // textures param
    #define M3D_NEAREST GL_NEAREST
    #define M3D_LINEAR GL_LINEAR
    #define M3D_NEAREST_MIPMAP_NEAREST GL_NEAREST_MIPMAP_NEAREST
    #define M3D_LINEAR_MIPMAP_NEAREST GL_LINEAR_MIPMAP_NEAREST
    #define M3D_NEAREST_MIPMAP_LINEAR GL_NEAREST_MIPMAP_LINEAR
    #define M3D_LINEAR_MIPMAP_LINEAR GL_LINEAR_MIPMAP_LINEAR
    #define M3D_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE
    #define M3D_REPEAT GL_REPEAT

    #define M3D_PERSPECTIVE_CORRECTION GL_PERSPECTIVE_CORRECTION_HINT
    #define M3D_LINE_SMOOTH GL_LINE_SMOOTH_HINT
    #define M3D_POINT_SMOOTH GL_POINT_SMOOTH_HINT
    #define M3D_GENERATE_MIPMAP_HINT GL_GENERATE_MIPMAP_HINT
    #define M3D_FOG GL_FOG_HINT

    #define M3D_FALSE 0
    #define M3D_TRUE  1

    #define M3D_RGB GL_RGB
    #define M3D_RGBA GL_RGBA

    // render flags
    #define M3D_RENDER_DEFAULT 0

    // material types
    #define M3D_MT_SOLID 0

    // type of geometry indices
    #define M3D_INDICE_TYPE unsigned short
    #define M3D_GL_INDICE_TYPE GL_UNSIGNED_SHORT

    #define M3D_MAX_VBO_SLOT 8

    #define M3D_INDICE_SLOT   0
    #define M3D_VERTICE_SLOT  1
    #define M3D_TEXCOORD_SLOT 2
    #define M3D_NORMAL_SLOT   3
    #define M3D_COLOR_SLOT    4

    enum GP2X_WIZ_BMAP
    {
        VK_UP , // 0
        VK_UP_LEFT , // 1
        VK_LEFT , // 2
        VK_DOWN_LEFT , // 3
        VK_DOWN , // 4
        VK_DOWN_RIGHT , // 5
        VK_RIGHT , // 6
        VK_UP_RIGHT , // 7
        VK_START , // 8
        VK_SELECT , // 9
        VK_FL , // 10
        VK_FR , // 11
        VK_FA , // 12
        VK_FB , // 13
        VK_FX , // 14
        VK_FY , // 15
        VK_VOL_UP , // 16
        VK_VOL_DOWN , // 17
        VK_TAT // 18
    };

#endif // __M3D_DEFINE_H__
