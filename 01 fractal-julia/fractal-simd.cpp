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

    __m256 c_real = _mm256_set1_ps(c.real()); // cx, cx, cx, cx, cx,cx ,cx ,cx
    __m256 c_imag = _mm256_set1_ps(c.imag()); // cy, cy, cy, cy, cy, cy, cy, cy

    __m256 max_norma = _mm256_set1_ps(4.0);

    __m256 one = _mm256_set1_ps(1.0);

    for (int i = 0; i < width; i++)
    {
        // Tienen que ser bloques de 8 porque es un DMF
        for (int j = 0; j < height; j += 8)
        {
            __m256 mx = _mm256_set1_ps(i); // i,i,i,i,i,i....
            __m256 my = _mm256_setr_ps(j + 0, j + 1, j + 2, j + 3,
                                       j + 4, j + 5, j + 6, j + 7);

            // xmin+mx+xscale ---> (x0,x1,x2 .....) ----> real
            __m256 cr = _mm256_add_ps(xmin, _mm256_mul_ps(mx, xscale));

            // ymax+my+yscale ---> (y0,y1,y2 .....) ----> imaginario
            __m256 ci = _mm256_sub_ps(ymax, _mm256_mul_ps(my, yscale));

            int iter = 0;
            __m256 mk = _mm256_set1_ps(iter);
            // verificar si los 8 complejos estan acotados

            // PArte real y la parte imaginaria inicial;
            __m256 zr = cr;
            __m256 zi = ci;

            while (iter < max_iteraciones)
            {
                __m256 zr2 = _mm256_mul_ps(zr, zr);  // zr elevado al 2
                __m256 zi2 = _mm256_mul_ps(zi, zi);  // zi elevado al 2
                __m256 zrzi = _mm256_mul_ps(zr, zi); // zi elevado al 2

                // zrelevapd2 -ziele2 +c_real
                zr = _mm256_add_ps(_mm256_sub_ps(zr2, zi2), c_real);

                zi = _mm256_add_ps(_mm256_add_ps(zrzi, zrzi), c_imag);

                // normas
                zr2 = _mm256_mul_ps(zr, zr);             // zr al cuadrado
                zi2 = _mm256_mul_ps(zi, zi);             // zi al cuadrado
                __m256 norma2 = _mm256_add_ps(zr2, zi2); // norma al cuadrado

                // si norma < 4 devuelve 0xFFFFFFFF, sino 0X00000000
                __m256 mask = _mm256_cmp_ps(norma2, max_norma, _CMP_LE_OQ); // NORMA <= 4.0

                mk = _mm256_add_ps(mk, _mm256_and_ps(mask, one));

                // verificar que mask  (0,0,0,0,0,0,0,0)
                if (_mm256_testz_ps(mask, _mm256_set1_ps(-1)))
                {
                    break;
                }

                iter++;
            }

            // verificar iter y seleccionar el color
            float d[8];
            _mm256_storeu_ps(d, mk);

            /**
             * d[0] -> iteraciones para x0+y0i
             * d[1] -> iteraciones para x1+y1i
             * d[2] -> iteraciones para x2+y2i
             * ...
             * d[7] -> iteraciones para x7+y7i
             */

            /*
            [(j+0) * width * + i]
            [(j+1) * width * + i]
            [(j+2) * width * + i]
            ....
            [(j+7) * width * + i]
            */
            for (int it = 0; it < 8; it++)
            {
                int index = (j + it) * width + i;

                if (index < width * height)
                {
                    if (d[it] < max_iteraciones)
                    {
                        int color_index = (int)d[it] % PALETTE_SIZE;
                        pixel_buffer[index] = color_ramp[color_index];
                    }
                    else
                    {
                        pixel_buffer[index] = 0xFF000000; // negro
                    }
                }
            }
        }
    }
}