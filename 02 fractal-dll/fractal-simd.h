#ifndef FRACTAL_SIMD_H
#define FRACTAL_SIMD_H

#include <cstdint>

extern int max_iteraciones;
// Esto es para que no salga con nombres raros, para ser usado desde fuera
extern "C"
void julia_simd(double x_min, double y_min, double x_max, double y_max,
                uint32_t width, uint32_t height,
                int max_iteraciones,
                uint32_t *pixel_buffer);


#endif