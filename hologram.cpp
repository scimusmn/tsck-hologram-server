// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// standard io
#include <iostream>

// threading
#include <thread>
#include <mutex>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

extern "C" {
  #include "server.h"
}
#include "settings.h"

using namespace std;
using namespace cv;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

settings s = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int main(int argc, char** argv) {
  /*String videoFile( "bionic-arm.mov" );
  if (argc > 1) { videoFile = argv[1]; }

  VideoCapture video(videoFile);
  if (!video.isOpened()) {
    cout << "FATAL: could not find video at \"" << videoFile << "\"; aborting!" << endl;
    return -1;
  }

  double fps = video.get(CAP_PROP_FPS);
  cout << "FPS: " << fps << endl;
  
  namedWindow("hologram", WINDOW_AUTOSIZE);

  bool running = true;

  while (running) {
    Mat frame;
    bool success = video.read(frame);
    if (success == false) {
      cout << "EOF" << endl;
      running = false;
      break;
    }

    imshow("hologram",frame);

    if (waitKey(int(1000/fps)) == 27) {
      // ESC pressed, quit
      cout << "user quit!" << endl;
      running = false;
      break;
    }
    }*/

  bool running = true;

  launch_server(argc, argv, &s, (int*) &running);

  cout << "done." << endl;
  
  return 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

