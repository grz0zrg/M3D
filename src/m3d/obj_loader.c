#include "obj_loader.h"

#include "resource_manager.h"

#define M3D_OBJ_MAX_LINE_LENGTH 256

typedef struct {
    m3d_Material *material;

    int used;
} m3d_ObjMtl;

typedef struct {
    m3d_ObjMtl **materials;

    unsigned int materials_n;
} m3d_ObjMtlib;

m3d_ObjMtlib *m3d_ObjMtlibInit() {
	m3d_ObjMtlib *mtlib = (m3d_ObjMtlib *)malloc(sizeof(m3d_ObjMtlib));

	if (!mtlib) return NULL;

	memset(mtlib, 0, sizeof(m3d_ObjMtlib));

	return mtlib;
}

m3d_Material *m3d_ObjMtlibAddMaterial(m3d_ObjMtlib *mtlib, char *name) {
    if (!mtlib) return NULL;

    m3d_Material *material = m3d_MaterialInit();
    if (!material) return NULL;

    material->name = name;

    m3d_ObjMtl *objmtl = (m3d_ObjMtl *)malloc(sizeof(m3d_ObjMtl));
    if (!objmtl) {
        m3d_MaterialFree(material);
        return NULL;
    }

    m3d_ObjMtl **mtls_tmp = NULL;
    if (!mtlib->materials) {
        mtls_tmp = (m3d_ObjMtl **)malloc(sizeof(m3d_ObjMtl *));
    } else {
        mtls_tmp = (m3d_ObjMtl **)realloc(mtlib->materials,
                            sizeof(m3d_ObjMtl *) * (mtlib->materials_n+1));
    }

    if (!mtls_tmp) {
        m3d_MaterialFree(material);
        free(objmtl);
        return NULL;
    }

    mtlib->materials = mtls_tmp;

    mtlib->materials[mtlib->materials_n] = objmtl;

    objmtl->material = material;
    objmtl->used = 0;

    mtlib->materials_n++;

    return material;
}

void m3d_ObjMtlibSetUsed(m3d_ObjMtlib *mtlib, const char *name) {
    if (!mtlib || !name) return;

    size_t name_len = strlen(name);

    unsigned int i = 0;
    for (i = 0; i < mtlib->materials_n; i++) {
        m3d_ObjMtl *objmtl = mtlib->materials[i];

        if (!objmtl) continue;

        m3d_Material *tmp_mat = objmtl->material;
        const char *material_name = tmp_mat->name;

        if (material_name) {
            if (strlen(material_name) != name_len) {
                continue;
            }

            if (strncmp(name, material_name, name_len) == 0) {
                objmtl->used = 1;
                break;
            }
        }
    }
}

m3d_Material *m3d_ObjMtlibGetMaterial(m3d_ObjMtlib *mtlib, const char *name) {
    if (!name) return NULL;

    m3d_Material *material = NULL;

    size_t name_len = strlen(name);

    unsigned int i = 0;
    for (i = 0; i < mtlib->materials_n; i++) {
        m3d_ObjMtl *objmtl = mtlib->materials[i];

        if (!objmtl) continue;

        m3d_Material * tmp_mat = objmtl->material;
        const char *material_name = tmp_mat->name;

        if (material_name) {
            if (strlen(material_name) != name_len) {
                continue;
            }

            if (strncmp(name, material_name, name_len) == 0) {
                material = tmp_mat;
                break;
            }
        }
    }

    return material;
}

void m3d_ObjMtlibFree(m3d_ObjMtlib *mtlib) {
    if (!mtlib) return;

    unsigned int i = 0;
    for (i = 0; i < mtlib->materials_n; i++) {
        m3d_ObjMtl *objmtl = mtlib->materials[i];
        if (!objmtl) continue;

        // only free unused materials
        if (!objmtl->used) {
            m3d_MaterialFree(objmtl->material);
        }

        free(objmtl);
    }
    free(mtlib->materials);
    free(mtlib);
}

char *m3d_ObjGetLine(FILE *f) {
    if (!f) {
        return NULL;
    }

    char *line = (char *)malloc(sizeof(char) * M3D_OBJ_MAX_LINE_LENGTH);
    if (!line) {
        return NULL;
    }

    int c = fgetc(f);
    while (c == ' ') { c = fgetc(f); }; // skip whitespaces

    if (c == EOF) {
        free(line);
        return NULL;
    }

    memset(line, 0, sizeof(char) * M3D_OBJ_MAX_LINE_LENGTH);

    int i = 0; // store true line length
    while (c != '\n' && c != EOF) {
        if (i < (M3D_OBJ_MAX_LINE_LENGTH-1)) {
            if (c == '#') { // skip .obj comments
                while (c != '\n' && c != EOF) { c = fgetc(f); };
                break;
            }

            line[i++] = c;
        }

        c = fgetc(f);
    }

    return line;
}

char *m3d_ObjGetTag(const char *line) {
    if (!line) return NULL;

    int tag_end = strcspn(line, " ");
    char *tag = (char *)malloc(sizeof(char) * (tag_end+1));
    if (!tag) return NULL;

    strncpy(tag, line, tag_end);
    tag[tag_end] = '\0';

    m3d_UtilToLowercase(tag);

    return tag;
}

char *m3d_ObjGetData(char *line) {
    if (!line) return NULL;

    int data_start = strcspn(line, " ")+1;
    size_t data_length = strlen(line)-data_start;

    char *data_start_p = line+data_start;

    // skip whitespace
    while (*data_start_p == ' ') { data_start_p++; data_length--; };

    if (data_length <= 0) return NULL;

    char *data = (char *)malloc(sizeof(char) * (data_length+1));
    if (!data) return NULL;

    memcpy(data, data_start_p, data_length+1);

    return data;
}

m3d_ObjMtlib *m3d_ObjLoadMtlib(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("failed to open .mtl \"%s\"", filename);
        return NULL;
    }

    m3d_ObjMtlib *mtlib = m3d_ObjMtlibInit();
    if (!mtlib) {
        return NULL;
    }

    m3d_Material *curr_material = NULL;

	// parse the .mtl
    char *line = NULL;
    while ((line = m3d_ObjGetLine(f))) {
        unsigned int line_length = strlen(line);

		if (!line_length) {
            free(line);
            continue;
		}

        char *tag = m3d_ObjGetTag(line);
        if (!tag) {
            free(line);
            continue;
        }

        char *data = m3d_ObjGetData(line);
        if (!data) {
            free(line);
            free(tag);
            continue;
        }

        char *data_p = data;

        int data_length = strlen(data);

        if (strncmp(tag, "newmtl", 6) == 0) {
            char *name = (char *)malloc(sizeof(char) * (data_length+1));
            if (name) {
                memcpy(name, data, data_length+1);

                curr_material = m3d_ObjMtlibAddMaterial(mtlib, name);
                if (!curr_material) {
                    free(name);
                    printf("%s newmtl: failed to add material to the mtlib",
                           filename);
                }
            }
        } else if (strncmp(tag, "map_kd", 6) == 0) {
            char *path = m3d_UtilGetPath(filename);
            if (path) {
                size_t path_length = strlen(path);

                char *texture_filename = (char *)malloc(sizeof(char) *
                                                (data_length+path_length+1));
                if (texture_filename) {
                    memcpy(texture_filename, path, path_length);
                    memcpy(texture_filename+path_length, data, data_length+1);

                    if (curr_material) {
                        m3d_Texture *diffuse_texture = NULL;
                        diffuse_texture =
                                m3d_ResourceManagerGetTexture(texture_filename);
                        curr_material->diffuse_texture = diffuse_texture;
                    }

                    free(texture_filename);
                } else {
                    printf("\"%s\" ignored (alloc. error)\n", tag);
                }

                free(path);
            } else {
                printf("\"%s\" ignored (path. error)\n", tag);
            }
        } else if (strncmp(tag, "ns" , 2) == 0) {
            if (curr_material) {
                int specular_hardness = m3d_UtilParseInt(&data_p);

                curr_material->specular_hardness = specular_hardness;
            }
        } else if (strncmp(tag, "ka" , 2) == 0) {
            if (curr_material) {
                float r = m3d_UtilParseFloat(&data_p);
                float g = m3d_UtilParseFloat(&data_p);
                float b = m3d_UtilParseFloat(&data_p);

                m3d_SetColor3f(&curr_material->ambient_color, r, g, b);
            }
        } else if (strncmp(tag, "kd" , 2) == 0) {
            if (curr_material) {
                float r = m3d_UtilParseFloat(&data_p);
                float g = m3d_UtilParseFloat(&data_p);
                float b = m3d_UtilParseFloat(&data_p);

                m3d_SetColor3f(&curr_material->diffuse_color, r, g, b);
            }
        } else if (strncmp(tag, "ks" , 2) == 0) {
            if (curr_material) {
                float r = m3d_UtilParseFloat(&data_p);
                float g = m3d_UtilParseFloat(&data_p);
                float b = m3d_UtilParseFloat(&data_p);

                m3d_SetColor3f(&curr_material->specular_color, r, g, b);
            }
        } else if (strncmp(tag, "ni" , 2) == 0) {
            if (curr_material) {
                float ior = m3d_UtilParseFloat(&data_p);

                curr_material->ior = ior;
            }
        } else if (strncmp(tag, "d" , 1) == 0) {
            if (curr_material) {
                float opacity = m3d_UtilParseFloat(&data_p);

                curr_material->opacity = opacity;
            }
        } else if (strncmp(tag, "illum", 5) == 0) {
            if (curr_material) {
                int illum_mode = m3d_UtilParseInt(&data_p);

                curr_material->illum_mode = illum_mode;
            }
        } else {
            printf("unknow .mtl tag: '%s'\n", tag);
        }

        free(line);
        free(tag);
        free(data);
    }

    fclose(f);

    return mtlib;
}

m3d_Mesh *m3d_ObjLoad(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("failed to open .obj \"%s\"", filename);
        return NULL;
    }

	m3d_Mesh *mesh = m3d_MeshInit();
	if (!mesh) {
        printf("failed to open .obj \"%s\" (mesh. init. failed)", filename);
        return NULL;
	}

	m3d_Geometry *obj_geom = m3d_GeomInit(); // store temporary obj data
	if (!obj_geom) {
        printf("failed to open .obj \"%s\" (geom. init. failed)", filename);
        m3d_MeshFree(mesh);
        fclose(f);
        return NULL;
	}

	m3d_Geometry *m3d_geom = m3d_GeomInit(); // store final geometry
	if (!m3d_geom) {
        printf("failed to open .obj \"%s\" (geom. init. failed)", filename);
        m3d_GeomFree(obj_geom);
        m3d_MeshFree(mesh);
        fclose(f);
        return NULL;
	}

    // will store the associated .mtl file data
	m3d_ObjMtlib *mtlib = NULL;

	/* will be used to unify indices to optimize the final geometry with
       zero geometry duplicata, with a hash table it is simple but it may
       require huge amount of memory...
	*/
	m3d_HashTable *indices_hash_table = m3d_HashTableInit();

	// .obj can store many meshs
	int mesh_count = 0;

    char *line = NULL;
    while ((line = m3d_ObjGetLine(f))) {
        unsigned int line_length = strlen(line);

		if (!line_length) {
            free(line);
            continue;
		}

        char *tag = m3d_ObjGetTag(line);
        if (!tag) {
            free(line);
            continue;
        }

        int tag_length = strlen(tag);

        char *data = m3d_ObjGetData(line);
        if (!data) {
            free(line);
            free(tag);
            continue;
        }

        char *data_p = data;

        int data_length = strlen(data);

		// process tags
        if (strncmp(tag, "mtllib", 6) == 0) {
            char *path = m3d_UtilGetPath(filename);
            if (path) {
                size_t path_length = strlen(path);

                char *mtl_filename = (char *)malloc(sizeof(char) *
                                                    (data_length+path_length+1));
                if (mtl_filename) {
                    memcpy(mtl_filename, path, path_length);
                    memcpy(mtl_filename+path_length, data, data_length+1);

                    mtlib = m3d_ObjLoadMtlib(mtl_filename);
                    if (!mtlib) {
                        printf("failed to load .mtl \"%s\"", mtl_filename);
                    }

                    free(mtl_filename);
                } else {
                    printf("\"%s\" ignored (alloc. error)\n", tag);
                }

                free(path);
            } else {
                printf("\"%s\" ignored (path. error)\n", tag);
            }
        } else if (strncmp(tag, "usemtl", 6) == 0) {
            if (!mesh->material && mtlib) {
                m3d_Material *material = m3d_ObjMtlibGetMaterial(mtlib, data);
                if (!material) {
                    printf("cannot found material in the mtllib \"%s\" \n",
                           data);
                }

                m3d_MeshSetMaterial(mesh, material);
            } else {
                printf("\"%s\" ignored (mesh has already a material"
                       " or no mtllib present)\n", tag);
            }
        } else if (strncmp(tag, "l", 1) == 0) { // edges
            m3d_geom->prim_type = M3D_GEOMETRY_LINE;
        } else if (strncmp(tag, "f", 1) == 0) {  // face

            long iface_type = 0;
            char **strings_list = m3d_UtilSplitString(data, ' ', &iface_type);

            int i = 0;
            for (i = 0; i < iface_type; i++) {
                char *indices = strings_list[i];
                char **indices_list = NULL;
                long ind_list_length = 0;

                indices_list = m3d_UtilSplitString(indices, '/',
                                                   &ind_list_length);
                if (!indices_list || ind_list_length == 0) {
                    continue;
                }

                if (ind_list_length > 3) {
                    printf("\"%s\" warning: unknow face type\n", filename);
                    continue;
                }

                unsigned int *indice = NULL;
                indice = (unsigned int *)m3d_HashTableGet(indices_hash_table,
                                                          indices);

                if (indice != NULL) { // the data for that indice exist already!
                    if (m3d_GeomAddIndice(m3d_geom, *indice)) {
                        printf("loading of \"%s\" failed "
                               "(face alloc. error)\n", filename);
                        m3d_MeshFree(mesh);

                        fclose(f);
                        break;
                    }
                } else { // the indice does not so we add it with the geom data
                    unsigned int vertice_indice = atoi(indices_list[0]);
                    unsigned int vertice_index  = (vertice_indice-1) * 3;

                    if (vertice_indice > obj_geom->vertice_n) continue;

                    float v1 = obj_geom->vertice[vertice_index];
                    float v2 = obj_geom->vertice[vertice_index+1];
                    float v3 = obj_geom->vertice[vertice_index+2];

                    if (m3d_GeomAddVertice(m3d_geom, v1, v2, v3)) {
                        printf("loading of \"%s\" failed "
                               "(vert. alloc. error)\n", filename);
                        m3d_MeshFree(mesh);

                        fclose(f);
                        break;
                    }
                    if (m3d_GeomAddIndice(m3d_geom, m3d_geom->vertice_n-1)) {
                        printf("loading of \"%s\" failed "
                               "(face alloc. error)\n", filename);
                        m3d_MeshFree(mesh);

                        fclose(f);
                        break;
                    }

                    if (ind_list_length > 1) {
                        unsigned int uv_indice = atoi(indices_list[1]);
                        if (uv_indice > 0) {
                            unsigned int uv_index = (uv_indice-1) * 2;

                            if (uv_indice > obj_geom->texcoord_n) continue;

                            float u = obj_geom->texcoord[uv_index];
                            float v = obj_geom->texcoord[uv_index + 1];

                            if (m3d_GeomAddTexcoord(m3d_geom, u, v)) {
                                printf("loading of \"%s\" failed "
                                       "(uv alloc. error)\n", filename);
                                m3d_MeshFree(mesh);

                                fclose(f);
                                break;
                            }
                        }
                    }

                    if (ind_list_length == 3) {
                        unsigned int normal_indice = atoi(indices_list[2]);
                        unsigned int normal_index = (normal_indice-1) * 3;

                        if (normal_indice > obj_geom->normal_n) continue;

                        float n1 = obj_geom->normal[normal_index];
                        float n2 = obj_geom->normal[normal_index + 1];
                        float n3 = obj_geom->normal[normal_index + 2];

                        if (m3d_GeomAddNormal(m3d_geom, n1, n2, n3)) {
                            printf("loading of \"%s\" failed "
                                   "(normal alloc. error)\n", filename);
                            m3d_MeshFree(mesh);

                            fclose(f);
                            break;
                        }
                    }

                    // we add the indices to the hash table
                    unsigned int *new_indice = (unsigned int *)malloc(
                                                        sizeof(unsigned int));
                    *new_indice = m3d_geom->vertice_n-1;
                    m3d_HashTableSet(indices_hash_table, indices,
                                     new_indice);
                }

                m3d_UtilFreeSplitResult(indices_list, ind_list_length);
            }
            m3d_UtilFreeSplitResult(strings_list, iface_type);

            if (iface_type == 3) {
                m3d_geom->prim_type = M3D_GEOMETRY_TRIANGLE;
            } else {
                printf("loading of \"%s\" failed "
                       "(unsupported face type)\n", filename);
                m3d_MeshFree(mesh);
                fclose(f);
            }
        } else if (strncmp(tag, "s", 1) == 0) {  // smooth
            if (*data == '1') {

            } else { // flat

            }
        } else if (strncmp(tag, "o", 1) == 0) {  // object
            mesh_count++;
            if (mesh_count > 1) { // only one is loaded
                fclose(f);
            }

            m3d_MeshSetName(mesh, data);
        } else if (*tag == 'v' && tag_length == 1) {  // vertice
            float v1 = m3d_UtilParseFloat(&data_p);
            float v2 = m3d_UtilParseFloat(&data_p);
            float v3 = m3d_UtilParseFloat(&data_p);

            if (m3d_GeomAddVertice(obj_geom, v1, v2, v3)) {
                printf("loading of \"%s\" failed (vert. alloc. error)\n",
                                                        filename);

                m3d_MeshFree(mesh);
				fclose(f);
            }
        } else if (strncmp(tag, "vn", 2) == 0) { // normal
            float n1 = m3d_UtilParseFloat(&data_p);
            float n2 = m3d_UtilParseFloat(&data_p);
            float n3 = m3d_UtilParseFloat(&data_p);

            if (m3d_GeomAddNormal(obj_geom, n1, n2, n3)) {
                printf("loading of \"%s\" failed (norm. alloc. error)\n",
                                                        filename);

                m3d_MeshFree(mesh);
				fclose(f);
            }
        } else if (strncmp(tag, "vt", 2) == 0) { // texcoord
            float u = m3d_UtilParseFloat(&data_p);
            float v = m3d_UtilParseFloat(&data_p);

            if (m3d_GeomAddTexcoord(obj_geom, u, v)) {
                printf("loading of \"%s\" failed (uv alloc. error)\n",
                                                        filename);

                m3d_MeshFree(mesh);
                fclose(f);
            }
        } else {
            printf("unknow .obj tag: \"%s\"\n", tag);
        }

		free(line);
        free(tag);
        free(data);
    }

    m3d_GeomFree(obj_geom);

    if (!mesh) {
        m3d_GeomFree(m3d_geom);
    } else {
        if (m3d_geom->indice_n == 0) {
            m3d_geom->prim_type = M3D_GEOMETRY_POINT;
        }

        m3d_MeshSetGeometry(mesh, m3d_geom);

        // we set it as used in the mtllib so m3d_ObjMtlibFree does not free it
        if (mesh->material) {
            m3d_ObjMtlibSetUsed(mtlib, mesh->material->name);
        }
    }

    // free the elements of the hash table
    unsigned int i = 0;
    for (i = 0; i < indices_hash_table->table_size; i++) {
        const char *key = m3d_HashTableGetKeyAt(indices_hash_table, i);
        if (!key) continue;

        free(m3d_HashTableGet(indices_hash_table, key));
    }
    m3d_HashTableFree(indices_hash_table);

    m3d_ObjMtlibFree(mtlib);

    fclose(f);

    return mesh;
}
