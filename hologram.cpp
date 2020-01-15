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

char video_file[] = "bionic-arm.mov";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(int argc, char** argv) {
  settings s = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  mutex stdout_mutex, settings_mutex, running_mutex;

  bool running = true;

  thread server(launch_server, argc, argv, &s, &running,
                &stdout_mutex, &settings_mutex, &running_mutex);
  thread video_player(show_hologram, video_file, &s, &running,
                      &stdout_mutex, &settings_mutex, &running_mutex);                      

  server.join();
  video_player.join();

  cout << "done." << endl;
  
  return 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

