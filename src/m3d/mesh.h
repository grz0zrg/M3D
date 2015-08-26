#ifndef __M3D_MESH_H__
#define __M3D_MESH_H__

    #include <stdlib.h>
    #include <string.h>

    #include "material.h"
    #include "geometry.h"
    #include "matrix.h"

    typedef struct {
        char *name;

        m3d_Geometry *geometry;
        m3d_Material *material;

        m3d_Matrix model_matrix;

        float x, y, z;
        float sx, sy, sz;
        float rx, ry, rz;
    } m3d_Mesh;

    m3d_Mesh *m3d_MeshInit();

    void m3d_MeshSetMaterial(m3d_Mesh *mesh, m3d_Material *material);
    void m3d_MeshSetGeometry(m3d_Mesh *mesh, m3d_Geometry *geom);
    void m3d_MeshSetName(m3d_Mesh *mesh, const char *name);
    void m3d_MeshMerge(m3d_Mesh *dst, m3d_Mesh *src);
    m3d_Mesh *m3d_MeshClone(m3d_Mesh *mesh);
    void m3d_MeshUpdateMatrix(m3d_Mesh *mesh);
    void m3d_MeshFree(m3d_Mesh *mesh);

#endif // __M3D_MESH_H__
