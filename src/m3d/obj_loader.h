#ifndef __M3D_OBJ_LOADER_H__
#define __M3D_OBJ_LOADER_H__

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "hash_table.h"
    #include "geometry.h"
    #include "mesh.h"
    #include "util.h"

    m3d_Mesh *m3d_ObjLoad(const char *filename);

#endif // __M3D_OBJ_LOADER_H__
