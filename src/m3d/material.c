#include "material.h"

m3d_Material *m3d_MaterialInit() {
	m3d_Material *material = (m3d_Material *)malloc(sizeof(m3d_Material));

	if (!material) return NULL;

	memset(material, 0, sizeof(m3d_Material));

	return material;
}

void m3d_MaterialFree(m3d_Material *material) {
    if (!material) return;

    free(material->name);
	free(material);
}
