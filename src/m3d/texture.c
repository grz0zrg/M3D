#include "texture.h"

#include "util.h"

typedef struct {
    int create_mipmaps;
} m3d_TextureConfig;

m3d_TextureConfig m3d_texture_config = {0};

m3d_Texture *m3d_TextureInit(m3d_Image *image) {
    if (!image) return NULL;
    if (!image->data) return NULL;

	m3d_Texture *texture = (m3d_Texture *)malloc(sizeof(m3d_Texture));
	texture->id = 0;

	if (!texture) return NULL;

    int free_image = 0;

    // build power of two image if necessary
	if (!m3d_UtilIsPowerOfTwo(image->width) ||
        !m3d_UtilIsPowerOfTwo(image->height)) {
        int new_width  = m3d_UtilNextPowerOfTwo(image->width);
        int new_height = m3d_UtilNextPowerOfTwo(image->height);

        m3d_Image *new_image = NULL;
        new_image = m3d_ImageInit(image->format, new_width, new_height);
        if (!new_image) {
            printf("failed to build power of two image...");
            free(texture);
            return NULL;
        }

        m3d_ImageCopy(image, new_image);

        image = new_image;

        free_image = 1;
	}

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    GLint internal_format = GL_RGBA;
    GLenum format = GL_RGBA;

    if (image->format == M3D_IMAGE_RGB) {
        internal_format = format = GL_RGB;
    } else if (image->format == M3D_IMAGE_GREYSCALE) {
        internal_format = format = GL_LUMINANCE;
    } else if (image->format == M3D_IMAGE_GREYSCALE_ALPHA) {
        internal_format = format = GL_LUMINANCE_ALPHA;
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image->width, image->height,
                 0, format, GL_UNSIGNED_BYTE, image->data);

    if (m3d_texture_config.create_mipmaps) {
        int i = 0;
        for (i = 2; i <= 4; i++) {
            m3d_Image *mipmap = NULL;
            mipmap = m3d_ImageInit(image->format, image->width/i, image->height/i);
            if (!mipmap) {
                printf("failed to build mipmaps...");
                glDeleteTextures(1, &texture->id);
                free(texture);
                break;
            }

            m3d_ImageScale(image, mipmap);

            glTexImage2D(GL_TEXTURE_2D, i-1, internal_format,
                         mipmap->width, mipmap->height, 0,
                         format, GL_UNSIGNED_BYTE, mipmap->data);

            m3d_ImageFree(mipmap);
        }
    }

    texture->width  = image->width;
    texture->height = image->height;

    if (free_image) {
        m3d_ImageFree(image);
    }

	return texture;
}

void m3d_TextureSetParameter(m3d_Texture *texture, GLenum pname, GLenum param) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameterf(GL_TEXTURE_2D, pname, param);
}

void m3d_TextureFree(m3d_Texture *texture) {
    if (!texture) return;

    glDeleteTextures(1, &texture->id);

    free(texture);
}

void m3d_TextureCreateMipmaps(int value) {
    m3d_texture_config.create_mipmaps = value;
}
