#ifndef HOLOGRAM_VIDEO_PLAYER_HPP
#define HOLOGRAM_VIDEO_PLAYER_HPP

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <mutex>

#include "settings.h"

void show_hologram(char* video_file, settings* s_, bool* running,
                   std::mutex* stdout_mutex, std::mutex* settings_mutex, std::mutex* running_mutex);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
