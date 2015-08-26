#include "vbo.h"

m3d_Vbo *m3d_VboInit() {
	m3d_Vbo *vbo = (m3d_Vbo *)malloc(sizeof(m3d_Vbo));

	if (!vbo) return NULL;

    memset(vbo, 0, sizeof(m3d_Vbo));

	return vbo;
}

void m3d_VboDelete(m3d_Vbo *vbo, unsigned int slot) {
    if (!vbo) return;

    glDeleteBuffers(1, &vbo->data[slot].id);
}

void m3d_VboUpdate(m3d_Vbo *vbo, unsigned int slot,
                   GLsizeiptr size, const GLvoid *data) {
    glBindBuffer(vbo->data[slot].target, vbo->data[slot].id);
    glBufferData(vbo->data[slot].target, size, data, vbo->data[slot].usage);
}

void m3d_VboPartialUpdate(m3d_Vbo *vbo, unsigned int slot, GLintptr offset,
                          GLsizeiptr size, const GLvoid *data) {
    glBindBuffer(vbo->data[slot].target, vbo->data[slot].id);
    glBufferSubData(vbo->data[slot].target, offset, size, data);
}

int m3d_VboBuild(m3d_Vbo *vbo, unsigned int slot,
                               GLsizeiptr size, const GLvoid *data,
                               GLenum target,
                               GLenum usage) {
    if (!vbo) return 1;

    if (size <= 0 || slot >= M3D_MAX_VBO_SLOT) {
        printf("m3d_VboBuild failed, size = 0 or slot >= M3D_MAX_VBO_SLOT\n");
        return 1;
    }

    if (vbo->data[slot].id) {
        m3d_VboDelete(vbo, slot);
    }

    GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(target, buffer_id);
    glBufferData(target, size, data, usage);

    vbo->data[slot].usage  = usage;
    vbo->data[slot].target = target;
    vbo->data[slot].id     = buffer_id;

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        m3d_VboDelete(vbo, slot);
        printf("m3d_VboBuild failed, GL error code: %i\n", err);
        return 1;
    }

    glBindBuffer(target, 0);

    return 0;
}

void m3d_VboFree(m3d_Vbo *vbo) {
    if (!vbo) return;

#ifndef DEBUG // crash problem related to glDeleteBuffers, may just be the driver
    int i = 0;
    for (i = 0; i < M3D_MAX_VBO_SLOT; i++) {
        glDeleteBuffers(1, &vbo->data[i].id);
    }
#endif // DEBUG

	free(vbo);
}
