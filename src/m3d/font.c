#include "font.h"

#include "resource_manager.h"

#pragma pack(push,1)
typedef struct {
    short int id;
    int font_width;
    int font_height;
    int cell_width;
    int cell_height;
    unsigned char bpp;
    unsigned char base_character_offset;
    unsigned char character_widths[256];
} m3d_FontBff;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct {
    int font_width;
    int font_height;
    int cell_width;
    int cell_height;
    unsigned char base_character_offset;
    unsigned char character_widths[256];
} m3d_FontDat;
#pragma pack(pop)

m3d_Font *m3d_FontInit() {
	m3d_Font *font = (m3d_Font *)malloc(sizeof(m3d_Font));

	if (!font) return NULL;

    memset(font, 0, sizeof(m3d_Font));

    font->geom = m3d_GeomInit();

    font->base_character_offset = 32;

	return font;
}

m3d_Font *m3d_FontLoadBff(const char *filename) {
    if (!filename) return NULL;

    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("failed to open font file \"%s\"", filename);
        fclose(f);
        return NULL;
    }

    size_t bytes_read = 0;

    m3d_FontBff bff;
    bytes_read = fread(&bff, 1, sizeof(m3d_FontBff), f);
    if (bytes_read < sizeof(m3d_FontBff)) {
        printf("failed to open font file \"%s\" (corrupt or invalid .bff)",
                filename);
        fclose(f);
        return NULL;
    }

    int image_format = M3D_IMAGE_RGB;
    switch (bff.bpp) {
        case 8:
            image_format = M3D_IMAGE_GREYSCALE;
            break;

        case 24:
            image_format = M3D_IMAGE_RGB;
            break;

        case 32:
            image_format = M3D_IMAGE_RGBA;
            break;

        default:
            printf("failed to open font file \"%s\" (image bpp invalid)",
                    filename);
            fclose(f);
            return NULL;
            break;
    }

    m3d_Image *image = m3d_ImageInit(image_format,
                                     bff.font_width,
                                     bff.font_height);
    if (!image) {
        fclose(f);
        return NULL;
    }

    long data_length = 0;
    data_length = (bff.font_width * bff.font_height) * (image->bpp/8);
    bytes_read = fread(image->data, 1, data_length, f);
    if (bytes_read < (unsigned long)data_length) {
        printf("loading of font \"%s\" failed (corrupt or invalid .bff)",
                filename);
        fclose(f);
        m3d_ImageFree(image);
        return NULL;
    }

    // loading successfull, init the font!
    m3d_Texture *texture = m3d_TextureInit(image);
    if (!texture) {
        printf("loading of font \"%s\" failed (m3d_TextureInit alloc. error)",
                filename);
        fclose(f);
        m3d_ImageFree(image);
        return NULL;
    }

    m3d_Font *font = m3d_FontInit();
    if (!font) {
        printf("loading of font \"%s\" failed (m3d_FontInit alloc. error)",
                filename);
        fclose(f);
        m3d_TextureFree(texture);
        m3d_ImageFree(image);
        return NULL;
    }

    font->texture = texture;

    font->row_pitch   = bff.font_width  / bff.cell_width;
    font->col_factor  = (float)bff.cell_width  / bff.font_width;
    font->row_factor  = (float)bff.cell_height / bff.font_height;
    font->cell_width  = bff.cell_width;
    font->cell_height = bff.cell_height;
    font->base_character_offset = bff.base_character_offset;

    int i = 0;
    for (i = 0; i < 256; i++) {
        font->character_widths[i] = bff.character_widths[i];
    }

    m3d_ImageFree(image);

    fclose(f);

    return font;
}

m3d_Font *m3d_FontLoad(const char *filename) {
    if (!filename) return NULL;

    char *ext = strrchr(filename, '.');

    if (!ext) return NULL;

    m3d_Font *font = NULL;

    if (strncmp(ext, ".bff", 3) == 0) {
        font = m3d_FontLoadBff(filename);
    } else { // standard image font
        m3d_Texture *texture = m3d_ResourceManagerGetTexture(filename);
        if (!texture) {
            printf("cannot load \"%s\" image font \n", filename);
            return NULL;
        }

        font = m3d_FontInit();
        if (!font) {
            printf("loading of image font \"%s\" failed "
                   "(m3d_FontInit alloc. error)",
                    filename);
            m3d_TextureFree(texture);
            return NULL;
        }

        font->texture = texture;
    }

    return font;
}

void m3d_FontApplyDat(m3d_Font *font, const char *filename) {
    if (!filename || !font) return;

    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("failed to open font data file \"%s\"", filename);
        fclose(f);
    }

    size_t bytes_read = 0;

    m3d_FontDat dat;
    bytes_read = fread(&dat, 1, sizeof(m3d_FontDat), f);
    if (bytes_read < sizeof(m3d_FontDat)) {
        printf("failed to open font data file \"%s\" (corrupt or invalid data)",
                filename);
        fclose(f);
    }

    font->row_pitch   = dat.font_width  / dat.cell_width;
    font->col_factor  = (float)dat.cell_width  / dat.font_width;
    font->row_factor  = (float)dat.cell_height / dat.font_height;
    font->cell_width  = dat.cell_width;
    font->cell_height = dat.cell_height;
    font->base_character_offset = dat.base_character_offset;

    int i = 0;
    for (i = 0; i < 256; i++) {
        font->character_widths[i] = dat.character_widths[i];
    }

    fclose(f);
}

void m3d_FontSetup(m3d_Font *font, unsigned char base_char,
                   int cell_width, int cell_height, int spacing) {
    if (!font) return;

    font->row_pitch   = font->texture->width  / cell_width;
    font->col_factor  = (float)cell_width / font->texture->width;
    font->row_factor  = (float)cell_height / font->texture->height;
    font->cell_width  = cell_width;
    font->cell_height = cell_height;
    font->base_character_offset = base_char;

    int i = 0;
    for (i = 0; i < 256; i++) {
        font->character_widths[i] = spacing;
    }
}

void m3d_FontSetLineSpacing(m3d_Font *font, int spacing) {
    if (!font) return;

    font->line_spacing = spacing;
}

void m3d_FontFree(m3d_Font *font) {
    if (!font) return;

    m3d_TextureFree(font->texture);
    m3d_GeomFree(font->geom);

	free(font);
}
