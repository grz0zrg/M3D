#include "mesh.h"

#include "define.h"

m3d_Mesh *m3d_MeshInit() {
	m3d_Mesh *mesh = (m3d_Mesh *)malloc(sizeof(m3d_Mesh));

	if (!mesh) return NULL;

    memset(mesh, 0, sizeof(m3d_Mesh));

    m3d_MatrixMakeIdentity(&mesh->model_matrix);

    mesh->sz = 1.0f;
    mesh->sx = mesh->sy = -1.0f;

	return mesh;
}

void m3d_MeshFree(m3d_Mesh *mesh) {
    if (!mesh) return;

	free(mesh->name);
	free(mesh);
}

void m3d_MeshSetName(m3d_Mesh *mesh, const char *name) {
    if (!mesh) return;
    if (!name) return;

    size_t name_length = strlen(name);

    mesh->name = (char *)malloc(sizeof(char) * (name_length+1));
    if (mesh->name) {
        memcpy(mesh->name, name, name_length+1);
    }
}

void m3d_MeshSetMaterial(m3d_Mesh *mesh, m3d_Material *material) {
    if (!mesh || !material) return;

    mesh->material = material;
}

void m3d_MeshSetGeometry(m3d_Mesh *mesh, m3d_Geometry *geom) {
    if (!mesh || !geom) return;

    mesh->geometry = geom;
}

void m3d_MeshMerge(m3d_Mesh *dst, m3d_Mesh *src) {
    if (!dst || !src) return;
    if (!dst->geometry) return;
    if (!dst->geometry->vertice_n) return;

    unsigned int dst_prev_v_n = dst->geometry->vertice_n * 3;
    m3d_GeomMerge(dst->geometry, src->geometry);

    m3d_MeshUpdateMatrix(src);

    unsigned int i = 0;
    for (i = dst_prev_v_n; i < dst->geometry->vertice_n * 3; i+=3) {
        float v1 = dst->geometry->vertice[i];
        float v2 = dst->geometry->vertice[i+1];
        float v3 = dst->geometry->vertice[i+2];

        dst->geometry->vertice[i]   =
            v1 * src->model_matrix.m[0] +
            v2 * src->model_matrix.m[4] +
            v3 * src->model_matrix.m[8] +
            -1 * src->model_matrix.m[12];

        dst->geometry->vertice[i+1] =
            v1 * src->model_matrix.m[1] +
            v2 * src->model_matrix.m[5] +
            v3 * src->model_matrix.m[9] +
            -1 * src->model_matrix.m[13];

        dst->geometry->vertice[i+2] =
            v1 * src->model_matrix.m[2] +
            v2 * src->model_matrix.m[6] +
            v3 * src->model_matrix.m[10]+
            -1 * src->model_matrix.m[14];

/*        dst->geometry->vertice[i]   -= src->model_matrix.m[12];
        dst->geometry->vertice[i+1] -= src->model_matrix.m[13];
        dst->geometry->vertice[i+2] -= src->model_matrix.m[14];
*/
    }
}

m3d_Mesh *m3d_MeshClone(m3d_Mesh *mesh) {
    if (!mesh) return NULL;

    m3d_Mesh *new_mesh = m3d_MeshInit();
    if (!new_mesh) return NULL;

    new_mesh->model_matrix = mesh->model_matrix;
    new_mesh->geometry = mesh->geometry;
    new_mesh->material = mesh->material;
    new_mesh->x  = mesh->x;  new_mesh->y  = mesh->y;  new_mesh->z  = mesh->z;
    new_mesh->sx = mesh->sx; new_mesh->sy = mesh->sy; new_mesh->sz = mesh->sz;
    new_mesh->rx = mesh->rx; new_mesh->ry = mesh->ry; new_mesh->rz = mesh->rz;

    m3d_MeshSetName(new_mesh, mesh->name);

    return new_mesh;
}

void m3d_MeshUpdateMatrix(m3d_Mesh *mesh) {
    m3d_MatrixMakeIdentity(&mesh->model_matrix);
    m3d_MatrixTranslate(&mesh->model_matrix, mesh->x, mesh->y, mesh->z);
    m3d_MatrixRotate(&mesh->model_matrix, mesh->rx, mesh->ry, mesh->rz);
    m3d_MatrixScale(&mesh->model_matrix, mesh->sx, mesh->sy, mesh->sz);
}
