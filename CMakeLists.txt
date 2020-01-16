cmake_minimum_required(VERSION 3.2)

project(hologram C CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

set(GXX_COMPILE_FLAGS "-fpermissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GXX_COMPILE_FLAGS}")

add_executable(hologram video_player.cpp server.cpp mg/mongoose.c hologram.cpp)

target_link_libraries(hologram ssl crypto Threads::Threads ${OpenCV_LIBS})

if(WIN32)
        target_link_libraries(hologram ws2_32)
endif()       

