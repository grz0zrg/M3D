#include "png_loader.h"

#include "lodepng/lodepng.h"

m3d_Image *m3d_PngLoad(const char *filename) {
    unsigned char *image = NULL;
    unsigned int width = 0, height = 0;
    unsigned int error = 0;

    unsigned char* png;
    size_t pngsize;
    LodePNGState state;
    lodepng_state_init(&state);

    lodepng_load_file(&png, &pngsize, filename);

    error = lodepng_decode(&image, &width, &height, &state, png, pngsize);

    if(error) {
        printf("loading of image \"%s\" failed (%s)\n",
                filename, lodepng_error_text(error));
        free(png);
        return NULL;
    }

    m3d_ImageFormat image_format = M3D_IMAGE_RGBA;
    switch (state.info_png.color.colortype) {
        case LCT_GREY:
            image_format = M3D_IMAGE_GREYSCALE;
            break;

        case LCT_RGB:
            image_format = M3D_IMAGE_RGB;
            break;

        case LCT_PALETTE:
            image_format = M3D_IMAGE_INDEXED;
            break;

        case LCT_GREY_ALPHA:
            image_format = M3D_IMAGE_GREYSCALE_ALPHA;
            break;

        case LCT_RGBA:
            image_format = M3D_IMAGE_RGBA;
            break;

        default:
            printf("loading of .png image \"%s\" failed (unknow image type)\n",
                    filename);
            free(png);
            lodepng_state_cleanup(&state);
            free(image);
            return NULL;
            break;
    }

    m3d_Image *mimage = m3d_ImageInit(image_format, width, height);
    if (!mimage) {
        printf("loading of image \"%s\" failed (alloc. error)\n",
                    filename);
    }

    m3d_ImageCopyData(mimage, M3D_IMAGE_RGBA, image);

    if (image_format == M3D_IMAGE_INDEXED) {
        mimage->palette = state.info_png.color.palette;
        mimage->palette_size = state.info_png.color.palettesize;
    }

    free(image);
    free(png);
    lodepng_state_cleanup(&state);

    return mimage;
}
