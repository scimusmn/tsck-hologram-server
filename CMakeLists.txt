cmake_minimum_required(VERSION 3.10)

project(hologram)

add_executable(hologram mg/mongoose.c hologram.c)
target_link_libraries(hologram ssl crypto ws2_32)
