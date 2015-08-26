#include "m3d/m3d.h"
#include "m3d/hash_table.h"
const M3D_INDICE_TYPE quadIndice2[] = {0, 1, 2,
                                      2, 3, 0};

 float quadVertice2[] = {-0.0f,  1.0f, 0.0f,
                             -0.0f, -0.0f, 0.0f,
                              1.0f, -0.0f, 0.0f,
                              1.0f,  1.0f, 0.0f};

 float quadUV2[] = {0.0f, 1.0f,
                        0.0f, 0.0f,
                        1.0f, 0.0f,
                        1.0f, 1.0f};

int main(int argc, char **argv)
{
    m3d_Initialize(320, 240);

    //m3d_TextureCreateMipmaps(M3D_TRUE);

	m3d_Mesh *cube = m3d_GetMesh("data/2plane.obj");
	m3d_Mesh *cube2 = m3d_GetMesh("data/cylinder_e.obj");
	m3d_Texture *tex = m3d_GetTexture("data/tex.png");
	m3d_Texture *laser = m3d_GetTexture("data/laser.png");
//	m3d_Texture *box = m3d_GetTexture("data/box.png");
	m3d_Texture *tn = m3d_GetTexture("data/tex.png");
	//m3d_GeomSetupVbo(cube->geometry);
	//m3d_Texture *debt = m3d_GetTexture("data/debug.png");
//	m3d_Mesh *plan = m3d_GetMesh("data/sphere.obj");

cube2->z = -4.0f;
    m3d_Font *f = m3d_GetFont("data/Arial.bff");

//    m3d_Mesh *quad = m3d_MeshInit();

    unsigned int i = 0;
    if (cube) {
       /* for (i = 0; i < cube->geometry->indice_n; i++) {
            unsigned char r = ((float)rand()/RAND_MAX)*255;
            unsigned char g = ((float)rand()/RAND_MAX)*255;
            unsigned char b = ((float)rand()/RAND_MAX)*255;
            m3d_GeomAddColor(cube->geometry, r, g, b, 1);
        }

        for (i = 0; i < cube->geometry->texcoord_n*2; i++) {
            cube->geometry->texcoord[i] *= 4.0;
        }
*/

    }

for (i = 0; i < 8; i++) {
quadUV2[i]*=2.0f;
}
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glEnable(GL_CULL_FACE);

    m3d_Camera *camera = m3d_CameraInit(M3D_CAMERA_PERSPECTIVE,
                            75.0f, 320.0f/240.0f, 0.1f, 10000.0f);
    m3d_Camera *camera_ortho = m3d_CameraInit(M3D_CAMERA_ORTHOGRAPHIC,
                            0.0f, 1.0f, 0.0f, 1.0f);
    m3d_UseCamera(camera);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glClearDepthf(1.0f);

    m3d_BenchmarkInit();
//m3d_Texture *te = cube->material->diffuse_texture;
	float a = 0;
	float ssc = 0.25f;
//double time = m3d_GetTime();
float z = -1.0f, g = 0.0, r = 0.0;
/*glEnable(GL_FOG);
glFogf(GL_FOG_START, 0.001);
glFogf(GL_FOG_END, 10000.001);
glFogf(GL_FOG_COLOR, 0xFF);
glFogf(GL_FOG_MODE, GL_EXP2);
*/
    while (m3d_IsRunning()) {
                    m3d_Mesh *m = cube;
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//time = m3d_GetTime();
		/*glLoadIdentity();
		glTranslatef(0, 0, z);
		glRotatef(a, 0.5, 0.5, 1);*/
		//z-= sinf(g/8)/2000.0f;

g+=0.05f;
r+=0.00005f;
m->rz = sinf(g/40)/200.0f;//a/4;
//cube->x = sinf(g*2)/4;
cube->sx = cube->sy = cube->sz = 0.25f;
//cube->ry = a;
cube->z = z;
cube->material->diffuse_texture = tn;
        m3d_UseCamera(camera);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_ALPHA_TEST);
        //glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        //glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);


glColor4f(1.0f, 1.0f, 1.0f, 0.08);


        //glAlphaFunc(GL_GEQUAL, 1.0-abs(z));
        //cube->rx = i*cube->rz;
		//m3d_RenderMesh(m);
/*
cube2->material->diffuse_texture = tn;
cube2->z = -1;
cube2->x = 0;cube2->y = 0;
cube2->rz = cube2->rx =cube2->ry= 0;
//cube2->sy= 2;
cube2->ry = a/4;

m3d_RenderMesh(cube2);*/
glDisable(GL_CULL_FACE);
        m3d_UseCamera(camera_ortho);

        glBindTexture(GL_TEXTURE_2D, tn->id);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

glLoadIdentity();
ssc+=0.001f;
//m3d_RenderQuad();
/*for (i = 0; i < 4; i++) {
quadUV2[i]+=0.004f;
}

for (i = 3; i < 8; i++) {
quadUV2[i]+=0.004f;
}*/

 glBindTexture(GL_TEXTURE_2D, laser->id);

 glLoadIdentity();

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, quadUV2);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, quadVertice2);

	glDrawElements(GL_TRIANGLES, 6, M3D_GL_INDICE_TYPE, quadIndice2);
	  //glBindTexture(GL_TEXTURE_2D, debt->id);
//m3d_RenderQuad();
        glEnable(GL_ALPHA_TEST);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glAlphaFunc(GL_GEQUAL, 0.001f);
		m3d_RenderText(f, 0, 0, "\nFPS:%i\nBench:%f",
                         m3d_BenchmarkGetFps(),
                         m3d_BenchmarkGetResult());

		m3d_SwapBuffers();

		a += 0.005f;

        m3d_BenchmarkRun();
	}

    m3d_CameraFree(camera);

	m3d_Free();

	return EXIT_SUCCESS;
}
