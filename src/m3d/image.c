#include "image.h"

m3d_Image *m3d_ImageInit(m3d_ImageFormat format,
                         unsigned int width, unsigned int height) {
	m3d_Image *image = (m3d_Image *)malloc(sizeof(m3d_Image));

	if (!image) return NULL;

	image->width  = width;
	image->height = height;
	image->format = format;
	image->data   = NULL;

	switch (image->format) {
        case M3D_IMAGE_GREYSCALE:
            image->bpp = 8;
            break;

        case M3D_IMAGE_INDEXED:
            image->bpp = 8;
            break;

        case M3D_IMAGE_GREYSCALE_ALPHA:
            image->bpp = 16;
            break;

        case M3D_IMAGE_RGB:
            image->bpp = 24;
            break;

        case M3D_IMAGE_RGBA:
            image->bpp = 32;
            break;

        default:
            printf("image creation failed, unsupported image format");
            free(image);
            return NULL;
            break;
	}

    long data_size = ((width * height) * (image->bpp/8));
    image->data = (unsigned char *)malloc(sizeof(unsigned char) * data_size);
    if (!image->data) {
        printf("image creation failed, data alloc. error");
        return NULL;
    }
    memset(image->data, 0, data_size);

    image->palette = NULL;
    image->palette_size = 0;

	return image;
}

void m3d_ImagePaletteInit(m3d_Image *image, unsigned int colors) {
    if (image->format == M3D_IMAGE_INDEXED) {
        image->palette = (unsigned char *)malloc(sizeof(unsigned char) *
                                            colors);
        image->palette_size = colors;
    }
}

void m3d_ImageCopyData(m3d_Image *img, m3d_ImageFormat src_format,
                       const unsigned char *src_data) {
    unsigned int i = 0, j = 0;
    unsigned int image_data_size = img->width*img->height*(img->bpp/8);
    if (src_format == M3D_IMAGE_RGBA) {
        if (img->format == M3D_IMAGE_RGB) {
            for (i = 0; i < image_data_size; i+=3) {
                img->data[i]   = src_data[j];
                img->data[i+1] = src_data[j+1];
                img->data[i+2] = src_data[j+2];
                j+=4;
            }
        } else if (img->format == M3D_IMAGE_GREYSCALE) {
            j = 0;
            for (i = 0; i < image_data_size; i++) {
                img->data[i]   = src_data[j];
                j+=4;
            }
        } else if (img->format == M3D_IMAGE_GREYSCALE_ALPHA) {
            j = 0;
            for (i = 0; i < image_data_size; i+=2) {
                img->data[i]   = src_data[j];
                img->data[i+1] = src_data[j+3];
                j+=4;
            }
        } else {
            for (i = 0; i < image_data_size; i++) {
                img->data[i]   = src_data[i];
            }
        }
    }
}

void m3d_ImageCopy(m3d_Image *src, m3d_Image *dst) {
    if (!src) return;
    if (!dst) return;

    if (src->bpp != dst->bpp) return;

    unsigned int bytes_per_pixel = dst->bpp>>3;

    unsigned int x = 0, y = 0;
    unsigned int i = 0;
    for (y = 0; y < src->height; y++) {
        for (x = 0; x < src->width; x++) {
            unsigned int i_dst = (x + y * dst->width) * bytes_per_pixel;
            unsigned int i_src = (x + y * src->width) * bytes_per_pixel;
            for (i = 0; i < bytes_per_pixel; i++) {
                dst->data[i_dst+i] = src->data[i_src+i];
            }
        }
    }
}

void m3d_ImageFlipVertical(m3d_Image *img) {
    if (!img) return;

    int height_m1 = img->height-1;
    int height_d2 = img->height>>1;

    int i = 0;
    unsigned int j = 0;
    for(i = 0; i < height_d2; i++) {
        for(j = 0; j < img->width; j++) {
            img->data[i * img->width + j] = img->data[(height_m1-i)*img->width + j];
        }
    }
}

// TODO: should work for 256x128 etc type of size
void m3d_ImageScale(m3d_Image *src, m3d_Image *dst) {
    if (!src) return;
    if (!dst) return;

    if (src->bpp != dst->bpp) return;

    unsigned int bytes_per_pixel = dst->bpp>>3;

	const float sourceXStep = (float)src->width  / (float)dst->width;
	const float sourceYStep = (float)src->width / (float)dst->height;

	float sy = 0.0f;

    unsigned int x = 0, y = 0;
    unsigned int i = 0;
    for (y = 0; y < dst->height; y++) {
        float sx = 0.0f;
        for (x = 0; x < dst->width; x++) {
            unsigned int i_dst = (x + y * dst->width) * bytes_per_pixel;
            unsigned int i_src = (sx + sy * src->width) * bytes_per_pixel;
            for (i = 0; i < bytes_per_pixel; i++) {
                dst->data[i_dst+i] = src->data[i_src+i];
            }
            sx += sourceXStep;
        }
		sy += sourceYStep;
    }
}

void m3d_ImageFree(m3d_Image *image) {
    if (!image) return;

    free(image->palette);
    free(image->data);
    free(image);
}
