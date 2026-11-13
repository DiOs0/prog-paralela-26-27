package com.programacion.paralela;

import jnr.ffi.util.BufferUtil;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

public class FractalMain {

    // Dice que la libreria esta implementado en otro lenguaje en este caso en C
    //native void test();

    static void main() {

        System.out.println("Fractal Main");

        System.out.println(FractalDll.INSTANCE);

        int w= 1600;
        int h= 900;
        double x_min= -1.5;
        double x_max= 1.5;
        double y_min= -1.0;
        double y_max= 1.0;
        int max_iteraciones= 10;

        IntBuffer pixelBuffer = ByteBuffer.allocateDirect(w * h * Integer.BYTES)
                .asIntBuffer();

        FractalDll.INSTANCE.julia_simd(
                x_min, y_min, x_max, y_max, w, h, max_iteraciones, pixelBuffer
        );

    }
}
