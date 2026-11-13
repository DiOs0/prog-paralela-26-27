package com.programacion.paralela;

import jnr.ffi.LibraryLoader;

import java.nio.Buffer;

public interface FractalDll {

    String LIBRARY_NAME="fractal-julia";

    FractalDll INSTANCE= LibraryLoader.create(FractalDll.class)
            .search("/home/labcom/Escritorio/paralela/prog-paralela-26-27/02 fractal-dll/build/Release")
            .load(LIBRARY_NAME);

    void julia_simd(double x_min, double y_min, double x_max, double y_max,
                    int width, int height, int max_iteraciones,
                    Buffer pixel_buffer);
}
