cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 11)

add_executable(raytracer main.cpp vec3.h ray.cpp ray.h hitable.h sphere.h hitable_list.h camera.h material.h RenderImage.h lodepng.h lodepng.cpp)