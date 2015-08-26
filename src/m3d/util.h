#ifndef __M3D_UTIL_H__
#define __M3D_UTIL_H__

    #include <string.h>
    #include <stdlib.h>

    #include <math.h>

    void m3d_UtilToLowercase(char *str);

    char **m3d_UtilSplitString(const char *str, char delimiter, long *n);
    void m3d_UtilFreeSplitResult(char **split_result, long n);

    char *m3d_UtilGetPath(const char *str);
    char *m3d_UtilParseValue(char **sp);
    float m3d_UtilParseFloat(char **sp);
    int   m3d_UtilParseInt(char **sp);
    int   m3d_UtilNextPowerOfTwo(int value);
    int   m3d_UtilIsPowerOfTwo(int value);
    float m3d_UtilDeg2Rad(float deg);
    float m3d_UtilRad2Deg(float rad);

#endif // __M3D_UTIL_H__
