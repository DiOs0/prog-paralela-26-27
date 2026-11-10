

#include "fractal-simd.h"

#include "palette.h"

#include <immintrin.h>

#include <complex>

extern std::complex<double> c;
extern int max_iteraciones;

void julia_simd(double x_min, double y_min, double x_max, double y_max,
                uint32_t width, uint32_t height,
                uint32_t *pixel_buffer)
{

    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;

    // Estos como son constantes no cambian
    //  8 veces xmin
    // PS Precision simple un float
    __m256 xmin = _mm256_set1_ps(x_min);
    // 8 veces xmin
    __m256 ymax = _mm256_set1_ps(y_max);
    __m256 xscale = _mm256_set1_ps(dx);
    __m256 yscale = _mm256_set1_ps(dy);

    __m256 c_real = _mm256_set1_ps(c.real()); //cx, cx, cx, cx, cx,cx ,cx ,cx
    __m256 c_imag = _mm256_set1_ps(c.imag()); //cy, cy, cy, cy, cy, cy, cy, cy

    for (int i = 0; i < width; i++)
    {
        // Tienen que ser bloques de 8 porque es un DMF
        for (int j = 0; j < height; j += 8)
        {
            __m256 mx = _mm256_set1_ps(i); // i,i,i,i,i,i....
            __m256 my = _mm256_setr_ps(j + 7, j + 6, j + 5, j + 4,
                                       j + 3, j + 2, j + 1, j + 0);

            // xmin+mx+xscale ---> (x0,x1,x2 .....) ----> real
            __m256 cr = _mm256_add_ps(xmin, _mm256_mul_ps(mx, xscale));

            // ymax+my+yscale ---> (y0,y1,y2 .....) ----> imaginario
            __m256 ci = _mm256_add_ps(ymax, _mm256_mul_ps(my, yscale));

            


            int iter = 1;
            __m256 mk= _mm256_set1_ps(iter);
            // verificar si los 8 complejos estan acotados

            //PArte real y la parte imaginaria inicial;
            __m256 zr= cr;
            __m256 zi= ci;


            while (iter < max_iteraciones)
            {
                __m256 zr2=_mm256_mul_ps(zr,zr); //zr elevado al 2
                __m256 zi2=_mm256_mul_ps(zi,zi); //zi elevado al 2
                __m256 zrzi=_mm256_mul_ps(zi,zi); //zi elevado al 2

                //zrelevapd2 -ziele2 +c_real
                zr = _mm256_add_ps(_mm256_sub_ps(zr2,zi2),c_real);


                zi = _mm256_add_ps(_mm256_add_ps(zrzi, zrzi), c_imag);

                iter++;
            }
        }
    }
}