/*
 * cross_gl.h
 *
 * Include & Definitions for cross-platform OpenGL support.
 *
 */

#ifndef __M3D_CROSS_GL_H__
#define __M3D_CROSS_GL_H__

    #ifdef GP2X_WIZ
        #include <opengles/gl.h>
        #include <opengles/egl.h>
        #include <fake_os.h>
    #else
        #include <GL/glew.h>
        #include <GLFW/glfw3.h>
    #endif

#endif // __M3D_CROSS_GL_H__
