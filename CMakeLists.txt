cmake_minimum_required(VERSION 3.10)

project(lajolla)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

if(MSVC)
  add_compile_options(/Wall)
else()
  add_compile_options(-Wall)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)

set(SRCS src/3rdparty/miniz.h
         src/3rdparty/miniz.c
         src/3rdparty/pugiconfig.hpp
         src/3rdparty/pugixml.hpp
         src/3rdparty/pugixml.cpp
         src/3rdparty/stb_image.h
         src/3rdparty/tinyexr.h
         src/3rdparty/tinyply.h
         src/compute_normals.h
         src/flexception.h
         src/hw1.h
         src/hw1_scenes.h
         src/hw2.h
         src/hw3.h
         src/hw4.h
         src/image.h
         src/matrix.h
         src/parallel.h
         src/parse_obj.h
         src/parse_ply.h
         src/parse_scene.h
         src/parse_serialized.h
         src/print_scene.h
         src/torrey.h
         src/transform.h
         src/vector.h
         src/compute_normals.cpp
         src/hw1.cpp
         src/hw2.cpp
         src/hw3.cpp
         src/hw4.cpp
         src/image.cpp
         src/main.cpp
         src/parallel.cpp
         src/parse_obj.cpp
         src/parse_ply.cpp
         src/parse_scene.cpp
         src/parse_serialized.cpp
         src/print_scene.cpp
         src/transform.cpp)

add_library(torrey_lib STATIC ${SRCS})
add_executable(torrey src/main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(torrey Threads::Threads)

target_link_libraries(torrey torrey_lib)
