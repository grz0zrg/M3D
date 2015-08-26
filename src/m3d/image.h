#ifndef __M3D_IMAGE_H__
#define __M3D_IMAGE_H__

    #include <stdio.h>

    #include "util.h"

    #define m3d_ImageFormat int

    #define M3D_IMAGE_GREYSCALE 1
    #define M3D_IMAGE_GREYSCALE_ALPHA 2
    #define M3D_IMAGE_INDEXED 3
    #define M3D_IMAGE_RGB 4
    #define M3D_IMAGE_RGBA 5

    typedef struct {
        unsigned char *data;

        unsigned char *palette;

        size_t palette_size;

        unsigned int width;
        unsigned int height;
        unsigned int bpp;

        int format;
    } m3d_Image;

    m3d_Image *m3d_ImageInit(m3d_ImageFormat format,
                             unsigned int width, unsigned int height);
    void m3d_ImagePaletteInit(m3d_Image *image, unsigned int colors);
    void m3d_ImageCopy(m3d_Image *src, m3d_Image *dst);
    void m3d_ImageCopyData(m3d_Image *img, m3d_ImageFormat src_format,
                       const unsigned char *src_data);
    void m3d_ImageFlipVertical(m3d_Image *img);
    void m3d_ImageScale(m3d_Image *src, m3d_Image *dst);
    void m3d_ImageFree(m3d_Image *image);

#endif // __M3D_IMAGE_H__
