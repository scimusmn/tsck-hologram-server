// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// standard io
#include <iostream>

// threading
#include <thread>
#include <mutex>

#include "video_player.hpp"
#include "server.hpp"
#include "settings.h"

using namespace std;
using namespace cv;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(int argc, char** argv) {
  settings s = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0 };
  mutex stdout_mutex, settings_mutex, running_mutex;

  char* video_file = "bionic-arm.mov";
  if (argc > 1) {
    video_file = argv[1];
  }
  
  bool running = true;

  thread server(launch_server, argc, argv, &s, &running,
                &stdout_mutex, &settings_mutex, &running_mutex);
  thread video_player(show_hologram, video_file, &s, &running,
                      &stdout_mutex, &settings_mutex, &running_mutex);                      

  if (server.joinable()) {
    server.join();
  }

  if (video_player.joinable()) {
    video_player.join();
  }

  cout << "done." << endl;
  
  return 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

