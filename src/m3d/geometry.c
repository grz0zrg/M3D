#include "geometry.h"

#include "define.h"

m3d_Geometry *m3d_GeomInit() {
	m3d_Geometry *geom = (m3d_Geometry *)malloc(sizeof(m3d_Geometry));

	if (!geom) return NULL;

    memset(geom, 0, sizeof(m3d_Geometry));

	geom->prim_type = M3D_GEOMETRY_TRIANGLE;

	return geom;
}

void m3d_GeomFree(m3d_Geometry *geom) {
    if (!geom) return;

    m3d_VboFree(geom->vbo);

	free(geom->indice);
	free(geom->vertice);
	free(geom->texcoord);
	free(geom->normal);
	free(geom->color);
	free(geom);
}

void m3d_GeomMerge(m3d_Geometry *dst, m3d_Geometry *src) {
    if (!dst || !src) return;

    unsigned int i = 0, data_length = 0;
    if (dst->indice && src->indice) {
        data_length = (dst->indice_n+src->indice_n);
        M3D_INDICE_TYPE *merged = NULL;
        merged = (M3D_INDICE_TYPE *)realloc(dst->indice,
                                            sizeof(M3D_INDICE_TYPE) *
                                            data_length);

        if (merged) {
            dst->indice = merged;
            for (i = dst->indice_n; i < data_length; i++) {
                dst->indice[i] = src->indice[i-dst->indice_n]+(dst->vertice_n);
            }
            dst->indice_n = data_length;
        }
    }

    if (dst->vertice && src->vertice) {
        data_length = (dst->vertice_n+src->vertice_n) * 3;
        float *merged = NULL;
        merged = (float *)realloc(dst->vertice,
                                            sizeof(float) *
                                            data_length);
        if (merged) {
            dst->vertice = merged;
            unsigned int v_real_n = dst->vertice_n * 3;
            for (i = v_real_n; i < data_length; i++) {
                dst->vertice[i] = src->vertice[i-v_real_n];
            }

            dst->vertice_n = (dst->vertice_n+src->vertice_n);
        }
    }

    if (dst->texcoord && src->texcoord) {
        data_length = (dst->texcoord_n+src->texcoord_n) * 2;
        float *merged = NULL;
        merged = (float *)realloc(dst->texcoord,
                                            sizeof(float) *
                                            data_length);
        if (merged) {
            dst->texcoord = merged;
            unsigned int t_real_n = dst->texcoord_n * 2;
            for (i = t_real_n; i < data_length; i++) {
                dst->texcoord[i] = src->texcoord[i-t_real_n];
            }
            dst->texcoord_n = (dst->texcoord_n+src->texcoord_n);
        }
    }

    if (dst->normal && src->normal) {
        data_length = (dst->normal_n+src->normal_n) * 3;
        float *merged = NULL;
        merged = (float *)realloc(dst->normal,
                                            sizeof(float) *
                                            data_length);
        if (merged) {
            dst->normal = merged;
            unsigned int n_real_n = dst->normal_n * 3;
            for (i = n_real_n; i < data_length; i++) {
                dst->normal[i] = src->normal[i-n_real_n];
            }
            dst->normal_n = (dst->normal_n+src->normal_n);
        }
    }

    if (dst->color && src->color) {
        data_length = (dst->color_n+src->color_n) * 4;
        unsigned char *merged = NULL;
        merged = (unsigned char *)realloc(dst->color,
                                            sizeof(unsigned char) *
                                            data_length);
        if (merged) {
            dst->color = merged;
            unsigned int c_real_n = dst->color_n * 4;
            for (i = c_real_n; i < data_length; i++) {
                dst->color[i] = src->color[i-c_real_n];
            }
            dst->color_n = (dst->color_n+src->color_n);
        }
    }
}

m3d_Geometry *m3d_GeomClone(m3d_Geometry *geom) {
    if (!geom) return NULL;

    m3d_Geometry *new_geom = m3d_GeomInit();
    if (!new_geom) return NULL;

    m3d_GeomAllocIndice(new_geom, geom->indice_n);
    m3d_GeomAllocVertice(new_geom, geom->vertice_n);
    m3d_GeomAllocNormal(new_geom, geom->normal_n);
    m3d_GeomAllocTexcoord(new_geom, geom->texcoord_n);
    m3d_GeomAllocColor(new_geom, geom->color_n);

    unsigned int i = 0;
    for (i = 0; i < geom->indice_n; i++) {
        new_geom->indice[i] = geom->indice[i];
    }

    for (i = 0; i < geom->vertice_n*3; i++) {
        new_geom->vertice[i] = geom->vertice[i];
    }

    for (i = 0; i < geom->normal_n*3; i++) {
        new_geom->normal[i] = geom->normal[i];
    }

    for (i = 0; i < geom->texcoord_n*2; i++) {
        new_geom->texcoord[i] = geom->texcoord[i];
    }

    for (i = 0; i < geom->color_n*4; i++) {
        new_geom->color[i] = geom->color[i];
    }

    return new_geom;
}

void m3d_GeomSetupVboExt(m3d_Geometry *geom,
                         GLenum iusage,
                         GLenum vusage,
                         GLenum tusage,
                         GLenum nusage,
                         GLenum cusage) {
    if (!geom) return;

    geom->vbo = m3d_VboInit();

    int error = 0;
    if (geom->indice_n > 0 && !geom->indice) {
        error = m3d_VboBuild(geom->vbo, M3D_INDICE_SLOT, geom->indice_n,
                             geom->indice,
                             GL_ELEMENT_ARRAY_BUFFER, iusage);
    }

    if (geom->vertice_n > 0 && !geom->vertice) {
        error = m3d_VboBuild(geom->vbo, M3D_VERTICE_SLOT, geom->vertice_n * 3,
                             geom->vertice,
                             GL_ARRAY_BUFFER, vusage);
    }

    if (geom->texcoord_n > 0 && !geom->texcoord) {
        error = m3d_VboBuild(geom->vbo, M3D_TEXCOORD_SLOT, geom->texcoord_n * 2,
                             geom->texcoord,
                             GL_ARRAY_BUFFER, tusage);
    }

    if (geom->normal_n > 0 && !geom->normal) {
        error = m3d_VboBuild(geom->vbo, M3D_NORMAL_SLOT, geom->normal_n * 3,
                             geom->normal,
                             GL_ARRAY_BUFFER, nusage);
    }

    if (geom->color_n > 0 && !geom->color) {
        error = m3d_VboBuild(geom->vbo, M3D_COLOR_SLOT, geom->color_n,
                             geom->color,
                             GL_ARRAY_BUFFER, cusage);
    }

    if (error) {
        m3d_VboFree(geom->vbo);
        geom->vbo = NULL;
    }
}

void m3d_GeomSetupVbo(m3d_Geometry *geom) {
    if (!geom) return;

    geom->vbo = m3d_VboInit();

    int error = 0;
    if (geom->indice_n > 0 && !geom->indice) {
        error = m3d_VboBuild(geom->vbo, M3D_INDICE_SLOT, geom->indice_n,
                             geom->indice,
                             GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    }

    if (geom->vertice_n > 0 && !geom->vertice) {
        error = m3d_VboBuild(geom->vbo, M3D_VERTICE_SLOT, geom->vertice_n * 3,
                             geom->vertice,
                             GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    }

    if (geom->texcoord_n > 0 && !geom->texcoord) {
        error = m3d_VboBuild(geom->vbo, M3D_TEXCOORD_SLOT, geom->texcoord_n * 2,
                             geom->texcoord,
                             GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    }

    if (geom->normal_n > 0 && !geom->normal) {
        error = m3d_VboBuild(geom->vbo, M3D_NORMAL_SLOT, geom->normal_n * 3,
                             geom->normal,
                             GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    }

    if (geom->color_n > 0 && !geom->color) {
        error = m3d_VboBuild(geom->vbo, M3D_COLOR_SLOT, geom->color_n,
                             geom->color,
                             GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    }

    if (error) {
        m3d_VboFree(geom->vbo);
        geom->vbo = NULL;
    }
}

void m3d_GeomSetVertice(m3d_Geometry *geom, unsigned int index,
                        float v1, float v2, float v3) {
    unsigned int i = index * 3;

    geom->vertice[i]   = v1;
    geom->vertice[i+1] = v2;
    geom->vertice[i+2] = v3;
}

void m3d_GeomSetTexcoord(m3d_Geometry *geom, unsigned int index,
                         float u, float v) {
    unsigned int i = index<<1;

    geom->texcoord[i]   = u;
    geom->texcoord[i+1] = v;
}

void m3d_GeomAllocVertice(m3d_Geometry *geom, unsigned int size) {
    if (geom->vertice) free(geom->vertice);

    geom->vertice = (float *)malloc(sizeof(float) * 3 * size);
    geom->vertice_n = size;
}

void m3d_GeomAllocTexcoord(m3d_Geometry *geom, unsigned int size) {
    if (geom->texcoord) free(geom->texcoord);

    geom->texcoord = (float *)malloc(sizeof(float) * 2 * size);
    geom->texcoord_n = size;
}

void m3d_GeomAllocNormal(m3d_Geometry *geom, unsigned int size) {
    if (geom->normal) free(geom->normal);

    geom->normal = (float *)malloc(sizeof(float) * 3 * size);
    geom->normal_n = size;
}

void m3d_GeomAllocColor(m3d_Geometry *geom, unsigned int size) {
    if (geom->color) free(geom->color);

    geom->color = (unsigned char *)malloc(sizeof(unsigned char) * 4 * size);
    geom->color_n = size;
}

void m3d_GeomAllocIndice(m3d_Geometry *geom, unsigned int size) {
    if (geom->indice) free(geom->indice);

    geom->indice = (M3D_INDICE_TYPE *)malloc(sizeof(M3D_INDICE_TYPE) * size);
    geom->indice_n = size;
}

int m3d_GeomAddVertice(m3d_Geometry *geom, float v1, float v2, float v3) {
	if (!geom) return 1;

    float *vertice = NULL;
	if (geom->vertice == NULL) {
		vertice = (float *)malloc(sizeof(float) * 3);
	} else {
		vertice = (float *)realloc(geom->vertice,
                                sizeof(float) * 3 * (geom->vertice_n+1));
	}

	if (!vertice) {
		return 1;
	}

	geom->vertice = vertice;

	unsigned int start_index     = geom->vertice_n * 3;
	geom->vertice[start_index]   = v1;
	geom->vertice[start_index+1] = v2;
	geom->vertice[start_index+2] = v3;

	geom->vertice_n++;

	return 0;
}

int m3d_GeomAddNormal(m3d_Geometry *geom, float n1, float n2, float n3) {
	if (!geom) return 1;

    float *normal = NULL;
	if (geom->normal == NULL) {
		normal = (float *)malloc(sizeof(float) * 3);
	} else {
		normal = (float *)realloc(geom->normal,
                                sizeof(float) * 3 * (geom->normal_n+1));
	}

	if (!normal) {
		return 1;
	}

	geom->normal = normal;

	unsigned int start_index    = geom->normal_n * 3;
	geom->normal[start_index]   = n1;
	geom->normal[start_index+1] = n2;
	geom->normal[start_index+2] = n3;

	geom->normal_n++;

	return 0;
}

int m3d_GeomAddTexcoord(m3d_Geometry *geom, float u, float v) {
	if (!geom) return 1;

    float *texcoord = NULL;
	if (geom->texcoord == NULL) {
		texcoord = (float *)malloc(sizeof(float) * 2);
	} else {
		texcoord = (float *)realloc(geom->texcoord,
									sizeof(float) * 2 * (geom->texcoord_n+1));
	}

	if (!texcoord) {
		return 1;
	}

	geom->texcoord = texcoord;

	unsigned int start_index    = geom->texcoord_n * 2;
	geom->texcoord[start_index]   = u;
	geom->texcoord[start_index+1] = v;

	geom->texcoord_n++;

	return 0;
}

int m3d_GeomAddColor(m3d_Geometry *geom, unsigned char r,
                                     unsigned char g,
                                     unsigned char b,
                                     unsigned char a) {
	if (!geom) return 1;

    unsigned char *color = NULL;
	if (geom->color == NULL) {
		color = (unsigned char *)malloc(sizeof(unsigned char) * 4);
	} else {
		color = (unsigned char *)realloc(geom->color,
                                sizeof(unsigned char) * 4 * (geom->color_n+1));
	}

	if (!color) {
		return 1;
	}

	geom->color = color;

	unsigned int start_index   = geom->color_n * 4;
	geom->color[start_index]   = r;
	geom->color[start_index+1] = g;
	geom->color[start_index+2] = b;
	geom->color[start_index+3] = a;

	geom->color_n++;

	return 0;
}

int m3d_GeomAddIndice(m3d_Geometry *geom, M3D_INDICE_TYPE ind) {
	if (!geom) return 1;

    M3D_INDICE_TYPE *indice = NULL;
	if (geom->indice == NULL) {
		indice = (M3D_INDICE_TYPE *)malloc(sizeof(M3D_INDICE_TYPE));
	} else {
		indice = (M3D_INDICE_TYPE *)realloc(geom->indice,
                                sizeof(M3D_INDICE_TYPE) * (geom->indice_n+1));
	}

	if (!indice) {
		return 1;
	}

	geom->indice = indice;

	geom->indice[geom->indice_n] = ind;

	geom->indice_n++;

	return 0;
}

void m3d_GeomDebug(m3d_Geometry *geom) {
    if (!geom) return;

    if (!geom->indice) {
        unsigned int i = 0;
        for (i = 0; i < geom->vertice_n*3; i+=3) {
            printf("Vertice:%f %f %f\n\n",
                  geom->vertice[i], geom->vertice[i+1],
                  geom->vertice[i+2]);
        }

        for (i = 0; i < geom->normal_n*3; i+=3) {
            printf("Normal:%f %f %f\n\n",
                  geom->normal[i], geom->normal[i+1],
                  geom->normal[i+2]);
        }

        for (i = 0; i < geom->texcoord_n*2; i+=2) {
            printf("UV:%f %f\n\n",
                  geom->texcoord[i], geom->texcoord[i+1]);
        }
    } else {
        unsigned int i = 0;
        for (i = 0; i < geom->indice_n; i++) {
            unsigned int indice = geom->indice[i];
            printf("Indice %i:\nVertice:%f %f %f\n"
                   "UV:     %f %f\nNormal: %f %f %f\n\n", indice,
                  geom->vertice[(indice*3)], geom->vertice[(indice*3)+1],
                  geom->vertice[(indice*3)+2],
                  geom->texcoord[(indice*2)], geom->texcoord[(indice*2)+1],
                  geom->normal[(indice*3)], geom->normal[(indice*3)+1],
                  geom->normal[(indice*3)+2]);
        }
    }
}
