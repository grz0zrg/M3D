#ifndef __M3D_MATERIAL_H__
#define __M3D_MATERIAL_H__

    #include <stdlib.h>
    #include <string.h>

    #include "color.h"
    #include "texture.h"
    #include "define.h"

    typedef struct {
        char *name;

        m3d_Color3f ambient_color;
        m3d_Color3f diffuse_color;
        m3d_Color3f specular_color;

        float opacity;

        float ior;
        float specular_hardness;

        int illum_mode; // 0 ignore lighting, 1 no specular, 2 lighting

        unsigned int type;
        unsigned int flags;

        m3d_Texture *diffuse_texture;
    } m3d_Material;

    m3d_Material *m3d_MaterialInit();
    void m3d_MaterialFree(m3d_Material *material);

#endif // __M3D_MATERIAL_H__
