#include "video_player.hpp"
#include <iostream>

using namespace std;
using namespace cv;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void show_hologram(char* video_file, settings* s_, bool* running,
                   mutex* stdout_mutex, mutex* settings_mutex, mutex* running_mutex) {
  VideoCapture video;
  
  if ( video_file == NULL ) {
    stdout_mutex->lock();
    cout << "FATAL: null video path provided; aborting!" << endl;
    stdout_mutex->unlock();
    
    running_mutex->lock();
    *running = false;
    running_mutex->unlock();
    return;
  }

  video = VideoCapture(video_file);
  if (!video.isOpened()) {
    stdout_mutex->lock();
    cout << "FATAL: could not open video at \"" << video_file << "\"; aborting!" << endl;
    stdout_mutex->unlock();

    running_mutex->lock();
    *running = false;
    running_mutex->unlock();
    return;
  }

  double fps = video.get(CAP_PROP_FPS);
  stdout_mutex->lock();
  cout << "video fps: " << fps << endl;
  stdout_mutex->unlock();

  namedWindow("hologram", WINDOW_AUTOSIZE);

  while(*running) {
    Mat frame;
    bool success = video.read(frame);
    if (success == false) {
      stdout_mutex->lock();
      cout << "EOF" << endl;
      stdout_mutex->unlock();

      running_mutex->lock();
      *running = false;
      running_mutex->unlock();
      break;
    }

    imshow("hologram",frame);

    if (waitKey(int(1000/fps)) == 27) {
      // ESC pressed, quit
      stdout_mutex->lock();
      cout << "user quit!" << endl;
      stdout_mutex->unlock();
      
      running_mutex->lock();
      *running = false;
      running_mutex->unlock();
      break;
    }
  }

  stdout_mutex->lock();
  cout << "shut down video player" << endl;
  stdout_mutex->unlock();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
