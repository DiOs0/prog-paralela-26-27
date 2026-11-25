

#include "fractal-openmp.h"
#include "palette.h"

#include <omp.h>
#include <complex> //Trabajar con numeros complejos

extern std::complex<double> c;
extern int max_iteraciones;

uint32_t acotado_openmp(double x, double y)
{

    // datos: c, z0
    // Zn+1 = Zn^2 + c
    int iter = 0;

    double zr = x;
    double zi = y;

    while (iter < max_iteraciones && (zr * zr + zi * zi) < 4.0){
        //Zn+1 = Zn² + c
        double new_zr = zr*zr - zi*zi + c.real();
        double new_zi = 2 *zr * zi + c.imag();

        zr = new_zr;
        zi = new_zi;

        iter++;
    }

    if (iter < max_iteraciones){
        // return 0xFFFFFFFF; // blanco
        int index = iter % PALETTE_SIZE;
        return color_ramp[index];
    }
    else{
        // norma<=2
        return 0x00000000; // negro
    }
}

void julia_openmp(double x_min, double y_min, double x_max, double y_max,
                  uint32_t width, uint32_t height,
                  uint32_t *pixel_buffer)
{

    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;

#pragma omp parallel
    {
        int thread_count = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        // Nos movemos verticalmente, entonces es ancho/#threads
        int delta = std::ceil(width * 1.0 / thread_count);

        int start = thread_id * delta;
        int end = (thread_id + 1) * delta;

        if (thread_id == thread_count - 1)
        {
            // Estamos en el ultimo hilo
            end = width;
        }

        for (int i = start; i < end; i++)
        {
            for (int j = 0; j < height; j++)
            {
                double x = x_min + i * dx;
                double y = y_max - j * dy;

                uint32_t color = acotado_openmp(x,y);

                pixel_buffer[j * width + i] = color;
            }
        }
    }
}

void julia_openmp_for(double x_min, double y_min, double x_max, double y_max,
                      uint32_t width, uint32_t height,
                      uint32_t *pixel_buffer)
{

    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;

#pragma omp parallel for
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double x = x_min + i * dx;
            double y = y_max - j * dy;

            uint32_t color = acotado_openmp(x,y);

            pixel_buffer[j * width + i] = color;
        }
    }
}