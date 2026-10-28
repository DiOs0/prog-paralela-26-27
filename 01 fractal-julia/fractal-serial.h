//Sea visible fuera del archivo
#pragma once


#include <cstdint>


void julia_serial(double x_min,double y_min,double x_max, double y_max, 
    uint32_t width, uint32_t height,
    uint32_t*pixel_buffer);
