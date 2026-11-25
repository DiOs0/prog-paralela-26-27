

#include "fractal-openmp.h"
#include "palette.h"

#include <omp.h>
#include <complex> //Trabajar con numeros complejos

extern std::complex<double> c;
extern int max_iteraciones;

uint32_t acotado_openmp(std::complex<double> z0)
{

    int iter = 0;

    std::complex<double> z = z0;

    while (iter < max_iteraciones && std::abs(z) < 2.0)
    {
        z = z * z + c;
        iter++;
    }

    if (iter < max_iteraciones)
    {
        // norma>2
        // return 0xFFFFFFFF; // Blanco
        int index = iter % PALETTE_SIZE;
        return color_ramp[index];
    }

    // norma<=2
    return 0x00000000; // Negro
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

                std::complex<double> z0(x, y);

                uint32_t color = acotado_openmp(z0);

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

            std::complex<double> z0(x, y);

            uint32_t color = acotado_openmp(z0);

            pixel_buffer[j * width + i] = color;
        }
    }
}