#ifndef __M3D_GEOMETRY_H__
#define __M3D_GEOMETRY_H__

    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>

    #include "cross_gl.h"
    #include "vbo.h"
    #include "define.h"

    #define M3D_GEOMETRY_POINT    1
    #define M3D_GEOMETRY_LINE     2
    #define M3D_GEOMETRY_TRIANGLE 4

    typedef struct {
        M3D_INDICE_TYPE *indice;

        float *vertice;
        float *texcoord;
        float *normal;

        unsigned char *color;

        unsigned int indice_n;
        unsigned int vertice_n;
        unsigned int texcoord_n;
        unsigned int normal_n;
        unsigned int color_n;

        m3d_Vbo *vbo;

        int prim_type;
    } m3d_Geometry;

    m3d_Geometry *m3d_GeomInit();
    void m3d_GeomAllocVertice(m3d_Geometry *geom, unsigned int size);
    void m3d_GeomAllocTexcoord(m3d_Geometry *geom, unsigned int size);
    void m3d_GeomAllocNormal(m3d_Geometry *geom, unsigned int size);
    void m3d_GeomAllocColor(m3d_Geometry *geom, unsigned int size);
    void m3d_GeomAllocIndice(m3d_Geometry *geom, unsigned int size);
    void m3d_GeomSetVertice(m3d_Geometry *geom, unsigned int index,
                            float v1, float v2, float v3);
    void m3d_GeomSetTexcoord(m3d_Geometry *geom, unsigned int index,
                             float u, float v);
    int  m3d_GeomAddVertice(m3d_Geometry *geom, float v1, float v2, float v3);
    int  m3d_GeomAddNormal(m3d_Geometry *geom, float n1, float n2, float n3);
    int  m3d_GeomAddTexcoord(m3d_Geometry *geom, float u, float v);
    int  m3d_GeomAddColor(m3d_Geometry *geom, unsigned char r, unsigned char g,
                                     unsigned char b, unsigned char a);
    int  m3d_GeomAddIndice(m3d_Geometry *geom, unsigned short ind);
    void m3d_GeomMerge(m3d_Geometry *dst, m3d_Geometry *src);
    m3d_Geometry *m3d_GeomClone(m3d_Geometry *geom);
    void m3d_GeomSetupVbo(m3d_Geometry *geom);
    void m3d_GeomSetupVboExt(m3d_Geometry *geom,
                         GLenum iusage,
                         GLenum vusage,
                         GLenum tusage,
                         GLenum nusage,
                         GLenum cusage);
    void m3d_GeomDebug(m3d_Geometry *geom);
    void m3d_GeomFree(m3d_Geometry *geom);

#endif // __M3D_GEOMETRY_H__
