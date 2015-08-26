#include "benchmark.h"

#include "m3d.h"

typedef struct {
    double last_time, fps_last_time;
    double window[M3D_BENCHMARK_WINDOW];
    double last_result;
    int curr_window_index;
    int frames;
    int fps;
} m3d_Benchmark;

m3d_Benchmark m3d_benchmark;

void m3d_BenchmarkInit() {
    m3d_benchmark.last_time = m3d_GetTime();
    m3d_benchmark.fps_last_time = m3d_GetTime();

    m3d_benchmark.curr_window_index = 0;
    m3d_benchmark.frames = 0;
    m3d_benchmark.last_result = 0.0;
}

void m3d_BenchmarkRun() {
    double time_diff = m3d_GetTime() - m3d_benchmark.last_time;
    double time_diff_fps = m3d_GetTime() - m3d_benchmark.fps_last_time;

    m3d_benchmark.window[m3d_benchmark.curr_window_index] = time_diff;
    m3d_benchmark.last_time = m3d_GetTime();

    m3d_benchmark.curr_window_index++;

    if (m3d_benchmark.curr_window_index >= M3D_BENCHMARK_WINDOW) {
        int i = 0;

        m3d_benchmark.curr_window_index = 0;

        double bench_result = 0.0;
        for (i = 0; i < M3D_BENCHMARK_WINDOW; i++) {
            bench_result += m3d_benchmark.window[i];
        }
        bench_result /= M3D_BENCHMARK_WINDOW;

        m3d_benchmark.last_result = bench_result;
    }

    if (time_diff_fps >= 1.0) {
        m3d_benchmark.fps_last_time = m3d_GetTime();
        m3d_benchmark.fps = m3d_benchmark.frames-1;
        m3d_benchmark.frames = 0;
    }

    m3d_benchmark.frames++;
}

int m3d_BenchmarkGetFps() {
    return m3d_benchmark.fps;
}

double m3d_BenchmarkGetResult() {
    return m3d_benchmark.last_result;
}
