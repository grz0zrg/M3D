#ifndef __M3D_COLOR_H__
#define __M3D_COLOR_H__

    typedef struct {
        float r, g, b, unused;
    } m3d_Color3f;

    void m3d_SetColor3f(m3d_Color3f *color, float r, float g, float b);

#endif // __M3D_COLOR_H__
