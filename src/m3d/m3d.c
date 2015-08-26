#include "m3d.h"
#include "resource_manager.h"

typedef struct {
#ifdef GP2X_WIZ
    EGLDisplay glDisplay;
    EGLConfig  glConfig;
    EGLContext glContext;
    EGLSurface glSurface;

    NativeWindowType hNativeWnd;

    SDL_Joystick *joy = NULL;
#else
    GLFWwindow *glfw_window;
#endif

    int screen_width;
    int screen_height;

    int is_running;
} m3d_Data;

m3d_Data *m3d_data = NULL;

#ifndef GP2X_WIZ
void m3d_OnWindowResize(GLFWwindow* window, int width, int height) {
    m3d_SetViewport(0, 0, width, height);

    m3d_data->screen_width = width;
    m3d_data->screen_height = height;

    window = window; // -Wextra
}
#endif

void m3d_Initialize(int screen_width, int screen_height) {
    m3d_data = (m3d_Data *)malloc(sizeof(m3d_Data));
    if (!m3d_data) {
        printf("[M3D] m3d_data alloc failed.");
        return;
    }

    m3d_data->screen_width  = screen_width;
    m3d_data->screen_height = screen_height;

    if (!m3d_ResourceManagerInit()) {
        printf("[M3D] m3d_ResourceManagerInit failed.");
        return;
    }

#ifdef GP2X_WIZ
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;

    EGLint attrib_list[] =
    {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_BUFFER_SIZE, 16,
      EGL_DEPTH_SIZE, 16,
      EGL_NONE, EGL_NONE
    };

    EGLint attrib_list_fsaa[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BUFFER_SIZE, 16,
        EGL_DEPTH_SIZE, 16,
        EGL_SAMPLE_BUFFERS, 1,
        EGL_SAMPLES, 2,
        EGL_NONE
    };

	m3d_data->hNativeWnd = OS_CreateWindow();
	m3d_data->glDisplay = eglGetDisplay((NativeDisplayType)0);

	eglInitialize(m3d_data->glDisplay, &majorVersion, &minorVersion);
	eglChooseConfig(m3d_data->glDisplay, attrib_list, &m3d_data->glConfig, 1,
                                                                   &numConfigs);

	m3d_data->glSurface = eglCreateWindowSurface(m3d_data->glDisplay,
                                        m3d_data->glConfig,
                                        m3d_data->hNativeWnd, NULL);
	m3d_data->glContext = eglCreateContext(m3d_data->glDisplay,
                                    m3d_data->glConfig, EGL_NO_CONTEXT, NULL);

	eglMakeCurrent(m3d_data->glDisplay, m3d_data->glSurface,
                   m3d_data->glSurface, m3d_data->glContext);

    SDL_Init(SDL_INIT_JOYSTICK);
    if (SDL_NumJoysticks() > 0) {
        m3d_data->joy = SDL_JoystickOpen(0);
    }
#else

    if (!glfwInit()) {
		printf("glfwInit failed");
        m3d_Free();
		return;
    }

    glfwWindowHint(GLFW_SAMPLES, 8);

    GLFWwindow *glfw_window = glfwCreateWindow(screen_width,screen_height, "",
                                               NULL, NULL);
    if (!glfw_window) {
		printf("glfwCreateWindow failed");
        m3d_Free();
		return;
    }

    glfwMakeContextCurrent(glfw_window);

    glfwSetWindowSizeCallback(glfw_window, m3d_OnWindowResize);

    glfwSwapInterval(1);

    m3d_data->glfw_window = glfw_window;
#endif

	m3d_data->is_running = 1;

#ifdef DEBUG
    const char *gl_vendor     = (const char *)glGetString(GL_VENDOR);
    const char *gl_renderer   = (const char *)glGetString(GL_RENDERER);
    const char *gl_version    = (const char *)glGetString(GL_VERSION);
    const char *gl_extensions = (const char *)glGetString(GL_EXTENSIONS);

	printf("GL_VENDOR: %s\nGL_RENDERER: %s\n"
                 "GL_VERSION: %s\nGL_EXTENSIONS: %s\n", gl_vendor, gl_renderer,
                                                    gl_version, gl_extensions);
#endif

    m3d_SetViewport(0, 0, screen_width, screen_height);

    m3d_ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m3d_SetSmoothShading();
	m3d_SetNicest(M3D_PERSPECTIVE_CORRECTION);
}

m3d_Image *m3d_GetImage(const char *filename) {
    return m3d_ResourceManagerGetImage(filename);
}

m3d_Texture *m3d_GetTexture(const char *filename) {
    return m3d_ResourceManagerGetTexture(filename);
}

m3d_Mesh *m3d_GetMesh(const char *filename) {
    return m3d_ResourceManagerGetMesh(filename);
}

m3d_Font *m3d_GetFont(const char *filename) {
    return m3d_ResourceManagerGetFont(filename);
}

void m3d_DropImage(const char *filename) {
    m3d_ResourceManagerDropImage(filename);
}

void m3d_DropTexture(const char *filename) {
    m3d_ResourceManagerDropTexture(filename);
}

void m3d_DropMesh(const char *filename) {
    m3d_ResourceManagerDropMesh(filename);
}

void m3d_DropFont(const char *filename) {
    m3d_ResourceManagerDropFont(filename);
}

void m3d_SetViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

void m3d_SwapBuffers() {
#ifdef GP2X_WIZ
    eglSwapBuffers(m3d_data->glDisplay, m3d_data->glSurface);
#else
    glfwSwapBuffers(m3d_data->glfw_window);
#endif
}

int m3d_IsRunning() {
#ifdef GP2X_WIZ
    SDL_JoystickUpdate();

    return m3d_data->is_running && !SDL_JoystickGetButton(m3d_data->joy,
                                                          VK_START);
#else
    glfwPollEvents();

    return (glfwGetKey(m3d_data->glfw_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                    !glfwWindowShouldClose(m3d_data->glfw_window));
#endif
}

double m3d_GetTime() {
#ifdef GP2X_WIZ
    return OS_GetTickCount();
#else
    return glfwGetTime();
#endif
}

inline int m3d_getScreenWidth() {
    return m3d_data->screen_width;
}

inline int m3d_getScreenHeight() {
    return m3d_data->screen_height;
}

void m3d_UseCamera(m3d_Camera *camera) {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera->projection_matrix.m);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void m3d_ClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void m3d_SetFastest(GLenum name) {
    glHint(name, GL_FASTEST);
}

void m3d_SetNicest(GLenum name) {
    glHint(name, GL_NICEST);
}

void m3d_SetFlatShading() {
    glShadeModel(GL_FLAT);
}

void m3d_SetSmoothShading() {
    glShadeModel(GL_SMOOTH);
}

void m3d_Enable(GLenum cap) {
    glEnable(cap);
}

void m3d_Disable(GLenum cap) {
    glDisable(cap);
}

void m3d_UseFrontFaceCW() {
    glFrontFace(GL_CW);
}

void m3d_UseFrontFaceCCW() {
    glFrontFace(GL_CCW);
}

void m3d_CullFrontFace() {
    glCullFace(GL_FRONT);
}

void m3d_CullBackFace() {
    glCullFace(GL_BACK);
}

void m3d_CullFace() {
    glCullFace(GL_FRONT_AND_BACK);
}

void m3d_Free() {
#ifndef GP2X_WIZ
    glfwTerminate();
#else
    if (!m3d_data) return;

    SDL_Quit();

	eglDestroySurface(m3d_data->glDisplay, m3d_data->glSurface);
	eglDestroyContext(m3d_data->glDisplay, m3d_data->glContext);
	eglTerminate(m3d_data->glDisplay);
	free(m3d_data->hNativeWnd);
#endif

    m3d_ResourceManagerFree();

    free(m3d_data);

#ifdef GP2X_WIZ
    chdir("/usr/gp2x");
    execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif
}
