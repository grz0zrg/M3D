#include "color.h"

void m3d_SetColor3f(m3d_Color3f *color, float r, float g, float b) {
    color->r = r; color->g = g; color->b = b; color->unused = 0;
}
