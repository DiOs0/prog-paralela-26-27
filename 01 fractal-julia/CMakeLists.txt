cmake_minimum_required(VERSION 3.16)

project(FractalJulia)

# pasar configuración al compilador de c++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(fractal-julia 
    fractal-serial.cpp
    main.cpp
    palette.cpp
    fractal-simd.cpp
    )
target_link_libraries(fractal-julia PRIVATE 
    fmt::fmt
    SFML::Graphics SFML::Window SFML::System

)