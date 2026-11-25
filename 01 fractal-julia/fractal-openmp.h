#pragma once

#include <cstdint>

extern int max_iteraciones;

void julia_openmp(double x_min, double y_min, double x_max, double y_max,
                    uint32_t width, uint32_t height,
                    uint32_t *pixel_buffer);

void julia_openmp_for(double x_min, double y_min, double x_max, double y_max,
                    uint32_t width, uint32_t height,
                    uint32_t *pixel_buffer);