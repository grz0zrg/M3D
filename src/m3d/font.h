#ifndef __M3D_FONT_H__
#define __M3D_FONT_H__

    #include <stdio.h>
    #include <stdlib.h>

    #include "texture.h"
    #include "geometry.h"
    #include "util.h"

    typedef struct {
        m3d_Texture *texture;
        m3d_Geometry *geom;

        int cell_width;
        int cell_height;
        int line_spacing;

        unsigned char base_character_offset;
        int row_pitch;
        float col_factor;
        float row_factor;

        unsigned char character_widths[256];
    } m3d_Font;

    m3d_Font *m3d_FontInit();
    m3d_Font *m3d_FontLoad(const char *filename);
    void m3d_FontSetup(m3d_Font *font, unsigned char base_char,
                       int cell_width, int cell_height, int spacing);
    void m3d_FontApplyDat(m3d_Font *font, const char *filename);
    void m3d_FontSetLineSpacing(m3d_Font *font, int spacing);
    void m3d_FontFree(m3d_Font *font);

#endif // __M3D_FONT_H__
