#include "resource_manager.h"

#define M3D_EXT_OBJ ".obj"
#define M3D_EXT_TGA ".tga"
#define M3D_EXT_PNG ".png"

#define M3D_RES_IMAGE   0
#define M3D_RES_FONT    1
#define M3D_RES_MESH    2
#define M3D_RES_TEXTURE 3

typedef struct {
    void *data;

    int type;
} m3d_Resource;

typedef struct {
    m3d_HashTable *data;
} m3d_ResourceManager;

m3d_ResourceManager *m3d_resource_mngr;

int m3d_ResourceManagerInit() {
	m3d_resource_mngr = (m3d_ResourceManager *)malloc(
                                                   sizeof(m3d_ResourceManager));

	if (!m3d_resource_mngr) return 0;

    m3d_resource_mngr->data = m3d_HashTableInit();
    if (!m3d_resource_mngr->data) return 0;

    return 1;
}

m3d_Resource *m3d_ResourceInit(int type, void *data) {
    if (!data) return NULL;

	m3d_Resource *resource = (m3d_Resource *)malloc(sizeof(m3d_Resource));
	if (!resource) return NULL;

    resource->type = type;
	resource->data = data;

	return resource;
}

char *m3d_ResourceManagerBuildKey(const char *filename,
                                        const char *keyword) {
    size_t filename_length = strlen(filename);
    size_t keyword_length  = strlen(keyword);
    char *key = (char *)malloc(sizeof(char) * filename_length+keyword_length+1);
    if (!key) return NULL;

    strncpy(key, keyword, keyword_length);
    strncpy(key+keyword_length, filename, filename_length+1);

    return key;
}

void m3d_ResourceFree(m3d_Resource *resource) {
    if (!resource) return;

    m3d_Texture *texture = NULL;
    m3d_Image   *image   = NULL;
    m3d_Font    *font    = NULL;
    m3d_Mesh    *mesh    = NULL;

    if (resource->data) {
        switch (resource->type) {
            case M3D_RES_IMAGE:
                image = (m3d_Image *)resource->data;
                m3d_ImageFree(image);
                break;

            case M3D_RES_FONT:
                font = (m3d_Font *)resource->data;
                m3d_FontFree(font);
                break;

            case M3D_RES_MESH:
                mesh = (m3d_Mesh *)resource->data;
                m3d_GeomFree(mesh->geometry);
                m3d_MaterialFree(mesh->material);
                m3d_MeshFree(mesh);
                break;

            case M3D_RES_TEXTURE:
                texture = (m3d_Texture *)resource->data;
                m3d_TextureFree(texture);
                break;

            default:
                printf("warning: m3d_ResourceFree invalid resource type\n");
                break;
        }
    }

    free(resource);
}

void m3d_ResourceManagerDrop(const char *filename) {
    if (!filename) return;
    if (!m3d_resource_mngr) return;

    m3d_HashTable *data = m3d_resource_mngr->data;

    m3d_ResourceFree((m3d_Resource *)m3d_HashTableGet(data, filename));

    m3d_HashTableDelete(data, filename);
}

void m3d_ResourceManagerDropImage(const char *filename) {
    m3d_ResourceManagerDrop(filename);
}

void m3d_ResourceManagerDropFont(const char *filename) {
    m3d_ResourceManagerDrop(filename);
}

void m3d_ResourceManagerDropMesh(const char *filename) {
    m3d_ResourceManagerDrop(filename);
}

void m3d_ResourceManagerDropTexture(const char *filename) {
    if (!filename) return;
    if (!m3d_resource_mngr) return;

    char *key = m3d_ResourceManagerBuildKey(filename, "TEXTURE:");
    if (!key) return;

    m3d_HashTable *data = m3d_resource_mngr->data;

    m3d_ResourceFree((m3d_Resource *)m3d_HashTableGet(data, key));

    m3d_HashTableDelete(data, filename);

    free(key);
}

m3d_Texture *m3d_ResourceManagerGetTexture(const char *filename) {
    if (!filename) return NULL;
    if (!m3d_resource_mngr) return NULL;

    // build unique key for textures as they could have the same key as images
    char *key = m3d_ResourceManagerBuildKey(filename, "TEXTURE:");
    if (!key) return NULL;

    m3d_Resource *resource = NULL;
    resource = (m3d_Resource *)m3d_HashTableGet(m3d_resource_mngr->data,
                                                key);
    if (resource) {
        if (resource->type != M3D_RES_TEXTURE) {
            printf("warning: \"%s\" is not a valid texture resource, "
                   "it is already loaded as a resource of type %i\n",
                   key, resource->type);
            return NULL;
        }
        return (m3d_Texture *)resource->data;
    }

    m3d_Image *image = m3d_ResourceManagerGetImage(filename);

    m3d_Texture *texture = m3d_TextureInit(image);

    resource = m3d_ResourceInit(M3D_RES_TEXTURE, texture);
    if (!resource) {
        m3d_TextureFree(texture);
    }

    if (!m3d_HashTableSet(m3d_resource_mngr->data, key, resource)) {
        m3d_TextureFree(texture);
    }

    m3d_ResourceManagerDrop(filename); // don't need it anymore

    free(key);

    return texture;
}

m3d_Font *m3d_ResourceManagerGetFont(const char *filename) {
    if (!filename) return NULL;
    if (!m3d_resource_mngr) return NULL;

    m3d_Resource *resource = NULL;
    resource = (m3d_Resource *)m3d_HashTableGet(m3d_resource_mngr->data,
                                                filename);
    if (resource) {
        if (resource->type != M3D_RES_FONT) {
            printf("warning: \"%s\" is not a valid font resource, "
                   "it is already loaded as a resource of type %i\n",
                   filename, resource->type);
            return NULL;
        }
        return (m3d_Font *)resource->data;
    }

    m3d_Font *font = NULL;
    font = m3d_FontLoad(filename);

    resource = m3d_ResourceInit(M3D_RES_FONT, font);
    if (!resource) {
        m3d_FontFree(font);
    }

    if (!m3d_HashTableSet(m3d_resource_mngr->data, filename, resource)) {
        m3d_FontFree(font);
    }

    return font;
}

m3d_Image *m3d_ResourceManagerGetImage(const char *filename) {
    if (!filename) return NULL;
    if (!m3d_resource_mngr) return NULL;

    m3d_Resource *resource = NULL;
    resource = (m3d_Resource *)m3d_HashTableGet(m3d_resource_mngr->data,
                                                filename);
    if (resource) {
        if (resource->type != M3D_RES_IMAGE) {
            printf("warning: \"%s\" is not a valid image resource, "
                   "it is already loaded as a resource of type %i\n",
                   filename, resource->type);
            return NULL;
        }
        return (m3d_Image *)resource->data;
    }

    m3d_Image *image = NULL;

    char *ext = strrchr(filename, '.');

    if (!ext) return NULL;

    if (strncmp(ext, M3D_EXT_TGA, 3) == 0) {
        image = m3d_TgaLoad(filename);
    } else if (strncmp(ext, M3D_EXT_PNG, 3) == 0) {
        image = m3d_PngLoad(filename);
    } else {
        printf("m3d_ResourceGetImage: cannot load image \"%s\", "
               "unknow format\n", filename);
    }

    resource = m3d_ResourceInit(M3D_RES_IMAGE, image);
    if (!resource) {
        m3d_ImageFree(image);
    }

    if (!m3d_HashTableSet(m3d_resource_mngr->data, filename, resource)) {
        m3d_ImageFree(image);
    }

    return image;
}

m3d_Mesh *m3d_ResourceManagerGetMesh(const char *filename) {
    if (!filename) return NULL;
    if (!m3d_resource_mngr) return NULL;

    m3d_Resource *resource = NULL;
    resource = (m3d_Resource *)m3d_HashTableGet(m3d_resource_mngr->data,
                                                filename);
    if (resource) {
        if (resource->type != M3D_RES_MESH) {
            printf("warning: \"%s\" is not a valid mesh resource, "
                   "it is already loaded as a resource of type %i\n",
                   filename, resource->type);
            return NULL;
        }
        return (m3d_Mesh *)resource->data;
    }

    m3d_Mesh *mesh = NULL;

    char *ext = strrchr(filename, '.');

    if (!ext) return NULL;

    if (strncmp(ext, M3D_EXT_OBJ, 3) == 0) {
        mesh = m3d_ObjLoad(filename);
    } else {
        printf("m3d_ResourceGetMesh: cannot load mesh \"%s\", "
               "unknow format\n", filename);
    }

    resource = m3d_ResourceInit(M3D_RES_MESH, mesh);
    if (!resource) {
        m3d_MeshFree(mesh);
    }

    if (!m3d_HashTableSet(m3d_resource_mngr->data, filename, resource)) {
        m3d_MeshFree(mesh);
    }

    return mesh;
}

void m3d_ResourceManagerFree() {
    if (!m3d_resource_mngr) return;

    // this free all resources
    // stuff inside the hash table is stored as void pointers so
    // we need to handle the freeing ourselve
    m3d_HashTable *data = m3d_resource_mngr->data;
    unsigned int i = 0;
    for (i = 0; i < data->table_size; i++) {
        const char *key = m3d_HashTableGetKeyAt(data, i);
        if (!key) continue;

        char *ext = strrchr(key, '.');

        if (!ext) continue;

        m3d_ResourceFree((m3d_Resource *)m3d_HashTableGet(data, key));
    }

    m3d_HashTableFree(m3d_resource_mngr->data);
	free(m3d_resource_mngr);
}
