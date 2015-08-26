#ifndef __M3D_VBO_H__
#define __M3D_VBO_H__

    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>

    #include "define.h"
    #include "cross_gl.h"

    typedef struct {
        GLuint id;
        GLenum target;
        GLenum usage;
    } m3d_VboData;

    typedef struct {
        m3d_VboData data[M3D_MAX_VBO_SLOT];
    } m3d_Vbo;

    m3d_Vbo *m3d_VboInit();
    int m3d_VboBuild(m3d_Vbo *vbo, unsigned int slot,
                                   GLsizeiptr size, const GLvoid *data,
                                   GLenum target,
                                   GLenum usage);
    void m3d_VboUpdate(m3d_Vbo *vbo, unsigned int slot,
                   GLsizeiptr size, const GLvoid *data);
    void m3d_VboPartialUpdate(m3d_Vbo *vbo, unsigned int slot, GLintptr offset,
                          GLsizeiptr size, const GLvoid *data);
    void m3d_VboDelete(m3d_Vbo *vbo, unsigned int slot);
    void m3d_VboFree(m3d_Vbo *vbo);

#endif // __M3D_VBO_H__
