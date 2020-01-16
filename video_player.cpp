#include "video_player.hpp"
#include <iostream>
#include <algorithm>

#if WIN32
  #include <windows.h>
#else
  #include <X11/Xlib.h>
#endif

using namespace std;
using namespace cv;

Mat build_output(Mat, int, int, settings*, mutex*);
void get_screen_size(int* width, int* height);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void show_hologram(char* video_file, settings* s, bool* running,
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

  namedWindow("hologram", WINDOW_NORMAL);
  setWindowProperty("hologram", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

  Mat frame;
  video.read(frame);

  int video_height = frame.rows;
  int video_width = frame.cols;
  int screen_height, screen_width;
  get_screen_size(&screen_width, &screen_height);

  stdout_mutex->lock();
  cout << "video size: "  << video_width  << "x" << video_height  << endl;
  cout << "screen size: " << screen_width << "x" << screen_height << endl;
  stdout_mutex->unlock();

  while(*running) {
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

    Mat result = build_output(frame, screen_width, screen_height,
                              s, settings_mutex);

    imshow("hologram",result);

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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Mat build_output(Mat frame, int width, int height, settings* s, mutex* settings_mutex) {
  const float min_size = 0.1;
  int x0 = width/2;
  int y0 = height/2;
  int xmax = min(width,height)/2;
  int ymax = xmax;

  Mat result = Mat::zeros(height,width, frame.type());
  
  Mat m1, m2, m3, m4;
  int x1, x2, x3, x4;
  int y1, y2, y3, y4;
  float s1, s2, s3, s4;

  // load settings
  settings_mutex->lock();

  s1 = s->size1 > min_size ? s->size1 : min_size;
  s2 = s->size2 > min_size ? s->size2 : min_size;
  s3 = s->size3 > min_size ? s->size3 : min_size;
  s4 = s->size4 > min_size ? s->size4 : min_size;

  resize(frame,m1,Size(),s1,s1,INTER_LINEAR);
  resize(frame,m2,Size(),s2,s2,INTER_LINEAR);
  resize(frame,m3,Size(),s3,s3,INTER_LINEAR);
  resize(frame,m4,Size(),s4,s4,INTER_LINEAR);

  rotate(m2, m2, ROTATE_90_CLOCKWISE);
  rotate(m3, m3, ROTATE_180);
  rotate(m4, m4, ROTATE_90_COUNTERCLOCKWISE);

  x1 = x0 - m1.cols/2;
  y1 = y0 - m1.rows - int(s->distance1 * (ymax-m1.rows));
  
  x2 = x0 + int(s->distance2 * (xmax-m2.cols));
  y2 = y0 - m2.rows/2;

  x3 = x0 - m3.cols/2;
  y3 = y0 + int(s->distance3 * (ymax-m3.rows));
  
  x4 = x0 - m4.cols - int(s->distance4 * (xmax-m4.cols));
  y4 = y0 - m4.rows/2;


  settings_mutex->unlock();

  //copy to output
  m1.copyTo(result(Rect(x1,y1,m1.cols,m1.rows)));
  m2.copyTo(result(Rect(x2,y2,m2.cols,m2.rows)));
  m3.copyTo(result(Rect(x3,y3,m3.cols,m3.rows)));
  m4.copyTo(result(Rect(x4,y4,m4.cols,m4.rows)));  
  
  return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void get_screen_size(int* width, int* height) {
  #if WIN32
  *width  = (int) GetSystemMetrics(SM_CXSCREEN);
  *height = (int) GetSystemMetrics(SM_CYSCREEN);
  #else
  Display* disp = XOpenDisplay(NULL);
  Screen*  scrn = DefaultScreenOfDisplay(disp);
  *width  = scrn->width;
  *height = scrn->height;
  #endif
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
