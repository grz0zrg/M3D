#include "render.h"

#include "m3d.h"

const M3D_INDICE_TYPE quadIndice[] = {0, 1, 2,
                                      2, 3, 0};

const float quadVertice[] = {-0.0f,  1.0f, 0.0f,
                             -0.0f, -0.0f, 0.0f,
                              1.0f, -0.0f, 0.0f,
                              1.0f,  1.0f, 0.0f};

const float quadUV[] = {0.0f, 1.0f,
                        0.0f, 0.0f,
                        1.0f, 0.0f,
                        1.0f, 1.0f};

void m3d_RenderQuad() {
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, quadUV);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, quadVertice);

	glDrawElements(GL_TRIANGLES, 6, M3D_GL_INDICE_TYPE, quadIndice);
}

void m3d_RenderMesh(m3d_Mesh *mesh) {
    m3d_MeshUpdateMatrix(mesh);

    //glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mesh->model_matrix.m);

    m3d_Material *material = mesh->material;
    if (material->diffuse_texture) {
        glBindTexture(GL_TEXTURE_2D, material->diffuse_texture->id);
    }

    m3d_Geometry *geom = mesh->geometry;
    m3d_Vbo *vbo = geom->vbo;

    if (vbo) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo->data[M3D_TEXCOORD_SLOT].id);
        glVertexPointer(2,GL_FLOAT,0,0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo->data[M3D_VERTICE_SLOT].id);
        glVertexPointer(3,GL_FLOAT,0,0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->data[M3D_INDICE_SLOT].id);
        glDrawElements(GL_TRIANGLES,0, M3D_GL_INDICE_TYPE,
                                                    0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    } else {

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, mesh->geometry->texcoord);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, mesh->geometry->vertice);
    /*
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, mesh->geometry->color);
    */
        glDrawElements(GL_TRIANGLES, mesh->geometry->indice_n, M3D_GL_INDICE_TYPE,
                                                    mesh->geometry->indice);
    }
}

void m3d_RenderText(m3d_Font *font, int xp, int yp, char *string, ...) {
    char szString[512];

    va_list ap;

    va_start(ap, string);
    vsprintf(szString, string, ap);
    va_end(ap);

    unsigned int text_length = strlen(szString);
    // required numbers of vertices for the whole text
    unsigned int vert_length = text_length*6;

    if (font->geom->vertice_n < vert_length) {
        m3d_GeomAllocVertice(font->geom, vert_length);
        m3d_GeomAllocTexcoord(font->geom, vert_length);

        unsigned int i = 0;
        for (i = 0; i < vert_length; i++) {
            font->geom->texcoord[i] = 0.0f;
            font->geom->vertice[i]  = 0.0f;
        }
    }

    float line_spacing = (float)(font->cell_height+font->line_spacing) /
                                                        m3d_getScreenHeight();

    float x = (float)xp/m3d_getScreenWidth();
    float y = 1.0f-line_spacing-(float)yp/m3d_getScreenWidth();

    float poly_w = (float)font->cell_width/m3d_getScreenWidth();
    float poly_h = (float)font->cell_height/m3d_getScreenHeight();

    unsigned int i = 0;
    for (i = 0; i < text_length; i++) {
        char c = szString[i];
        int Row = (c - font->base_character_offset) / font->row_pitch;
        int Col = (c - font->base_character_offset) -
                                                        (Row*font->row_pitch);
        if (c == '\n') {
            x = 0.0f;
            y -= line_spacing;
            continue;
        }

        float u  = Col * font->col_factor;
        float v  = Row * font->row_factor + 0.005f;
        float ur = u + font->col_factor;
        float vb = v + font->col_factor;

        int index  = i * 6;
        float xr = x + poly_w;
        float yb = y + poly_h;

        m3d_GeomSetVertice(font->geom, index, x, y, 0.0f);
        m3d_GeomSetTexcoord(font->geom, index, u, vb);
        index++;
        m3d_GeomSetVertice(font->geom, index, xr, y, 0.0f);
        m3d_GeomSetTexcoord(font->geom, index, ur, vb);
        index++;
        m3d_GeomSetVertice(font->geom, index, x, yb, 0.0f);
        m3d_GeomSetTexcoord(font->geom, index, u, v);
        index++;
        m3d_GeomSetVertice(font->geom, index, xr, y, 0.0f);
        m3d_GeomSetTexcoord(font->geom, index, ur, vb);
        index++;
        m3d_GeomSetVertice(font->geom, index, xr, yb, 0.0f);
        m3d_GeomSetTexcoord(font->geom, index, ur, v);
        index++;
        m3d_GeomSetVertice(font->geom, index, x, yb, 0.0f);
        m3d_GeomSetTexcoord(font->geom, index, u, v);

        x += (float)font->character_widths[(unsigned char)c] /
                                                        m3d_getScreenWidth();
    }

    glLoadIdentity();

    //glTranslatef(0.0f, 0.0, -1.25f);

    glDisableClientState(GL_COLOR_ARRAY);

    glBindTexture(GL_TEXTURE_2D, font->texture->id);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, font->geom->texcoord);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, font->geom->vertice);

    glDrawArrays(GL_TRIANGLES, 0, font->geom->vertice_n);
}
