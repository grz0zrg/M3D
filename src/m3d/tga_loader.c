#include "tga_loader.h"

#define M3D_TGA_UNCOMPRESSED_INDEXED 1
#define M3D_TGA_UNCOMPRESSED_RGB 2
#define M3D_TGA_UNCOMPRESSED_GREYSCALE 3
#define M3D_TGA_RLE_INDEXED 9
#define M3D_TGA_RLE_RGB 10
#define M3D_TGA_COMPRESSED_GREYSCALE 11
#define M3D_TGA_COMPRESSED_INDEXED 32
#define M3D_TGA_COMPRESSED4_INDEXED 33

#pragma pack(push,1)
typedef struct {
    char id_length;
    char colour_map_type;
    char data_type;
    short int colour_map_origin;
    short int colour_map_length;
    char  colour_map_depth;
    short int x_origin;
    short int y_origin;
    short width;
    short height;
    char  bpp;
    char  image_descriptor;
} m3d_TgaHeader;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct {
    int reserved_1;
    int reserved_2;

    char tag[18];
} m3d_TgaFooter;
#pragma pack(pop)

m3d_Image *m3d_TgaLoad(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("failed to open .tga \"%s\"\n", filename);
        return NULL;
    }

    size_t bytes_read = 0;

    m3d_TgaHeader header;
    bytes_read = fread(&header, 1, sizeof(m3d_TgaHeader), f);
    if (bytes_read < sizeof(m3d_TgaHeader)) {
        printf("loading of image \"%s\" failed (corrupt or invalid .tga)\n",
                filename);
        fclose(f);
        return NULL;
    }

    m3d_ImageFormat image_format = M3D_IMAGE_RGBA;

    switch (header.data_type) {
        case M3D_TGA_UNCOMPRESSED_RGB:
            if (header.image_descriptor&0x0F) {
                image_format = M3D_IMAGE_RGBA;
            } else {
                image_format = M3D_IMAGE_RGB;
            }
            break;

        case M3D_TGA_UNCOMPRESSED_INDEXED:
            image_format = M3D_IMAGE_INDEXED;
            if (header.colour_map_type != 1) {
                printf("loading of .tga image \"%s\" failed"
                       "(unsupported sub-format)\n", filename);
                fclose(f);
                return NULL;
            }

            break;

        case M3D_TGA_UNCOMPRESSED_GREYSCALE:
            if (header.image_descriptor&0x0F) {
                image_format = M3D_IMAGE_GREYSCALE_ALPHA;
            } else {
                image_format = M3D_IMAGE_GREYSCALE;
            }
            break;

        default:
            printf("loading of .tga image \"%s\" failed (unknow image type %i)\n",
                    filename, header.data_type);
            fclose(f);
            return NULL;
            break;
    }

    m3d_Image *image = m3d_ImageInit(image_format,
                                     header.width, header.height);
    if (!image) {
        printf("loading of image \"%s\" failed (alloc. error)\n",
                    filename);
        fclose(f);
        return NULL;
    }

    if (image_format == M3D_IMAGE_INDEXED) {
        bytes_read = fread(image->palette, 1, header.colour_map_length * 4, f);
        if (bytes_read < (unsigned int)(header.colour_map_length * 4)) {
            printf("loading of image \"%s\" failed (corrupt or invalid .tga)\n",
                    filename);
            fclose(f);
            m3d_ImageFree(image);
            return NULL;
        }
    }

    if (header.id_length > 0) { // unused
        char id[256] = {0};
        bytes_read = fread(id, 1, sizeof(char) * header.id_length, f);
    }

    long data_length = header.width * header.height * (image->bpp / 8);
    bytes_read = fread(image->data, 1, data_length, f);
    if (bytes_read < (unsigned long)data_length) {
        printf("loading of image \"%s\" failed (corrupt or invalid .tga)\n",
                filename);
        fclose(f);
        m3d_ImageFree(image);
        return NULL;
    }

    m3d_TgaFooter footer;
    bytes_read = fread(&footer, 1, sizeof(m3d_TgaFooter), f);
    if (bytes_read < sizeof(m3d_TgaHeader)) {
        char *tag = "TRUEVISION-XFILE.";

        if (strncmp(footer.tag, tag, 18) != 0) {
            printf("\"%s\" invalid tga tag (just saying!)\n", filename);
        } else {
            printf("\"%s\" missing tga tag (just saying!)\n", filename);
        }
    }

    fclose(f);

    return image;
}
