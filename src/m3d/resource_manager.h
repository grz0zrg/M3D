#ifndef __M3D_RESOURCE_MANAGER_H__
#define __M3D_RESOURCE_MANAGER_H__

    #include <stdlib.h>

    #include "hash_table.h"
    #include "texture.h"
    #include "obj_loader.h"
    #include "tga_loader.h"
    #include "png_loader.h"
    #include "font.h"

    int m3d_ResourceManagerInit();
    m3d_Font    *m3d_ResourceManagerGetFont   (const char *filename);
    m3d_Image   *m3d_ResourceManagerGetImage  (const char *filename);
    m3d_Mesh    *m3d_ResourceManagerGetMesh   (const char *filename);
    m3d_Texture *m3d_ResourceManagerGetTexture(const char *filename);

    void m3d_ResourceManagerDropImage  (const char *filename);
    void m3d_ResourceManagerDropFont   (const char *filename);
    void m3d_ResourceManagerDropMesh   (const char *filename);
    void m3d_ResourceManagerDropTexture(const char *filename);
    void m3d_ResourceManagerFree();

#endif // __M3D_RESOURCE_MANAGER_H__
