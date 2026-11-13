cmake_minimum_required(VERSION 3.16)

project(FractalJulia)

# pasar configuración al compilador de c++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

# En este caso vamos a crear una libreria no un ejecutable
add_library(fractal-julia  SHARED    
    palette.cpp
    fractal-simd.cpp
    )