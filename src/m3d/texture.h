#ifndef __M3D_TEXTURE_H__
#define __M3D_TEXTURE_H__

    #include <stdlib.h>
    #include <stdio.h>

    #include "cross_gl.h"

    #include "image.h"

    typedef struct {
        GLuint id;

        int width;
        int height;
    } m3d_Texture;

    m3d_Texture *m3d_TextureInit(m3d_Image *image);
    void m3d_TextureFree(m3d_Texture *texture);
    void m3d_TextureCreateMipmaps(int value);

    void m3d_TextureSetParameter(m3d_Texture *texture, GLenum pname,
                                 GLenum param);

#endif // __M3D_TEXTURE_H__
