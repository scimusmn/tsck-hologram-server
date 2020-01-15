cmake_minimum_required(VERSION 3.10)

find_package(OpenCV REQUIRED)

set(GXX_COMPILE_FLAGS "-fpermissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GXX_COMPILE_FLAGS}")

project(hologram)

add_executable(hologram server.c mg/mongoose.c hologram.cpp)
target_link_libraries(hologram ssl crypto ws2_32 ${OpenCV_LIBS})
