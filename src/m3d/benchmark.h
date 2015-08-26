#ifndef __M3D_BENCHMARK_H__
#define __M3D_BENCHMARK_H__

    #include <stdlib.h>

    #define M3D_BENCHMARK_WINDOW 128

    void m3d_BenchmarkInit();
    void m3d_BenchmarkRun();
    int m3d_BenchmarkGetFps();
    double m3d_BenchmarkGetResult();

#endif // __M3D_BENCHMARK_H__
