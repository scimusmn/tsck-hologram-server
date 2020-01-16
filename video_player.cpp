#include "video_player.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat build_output(Mat, settings*, mutex*);

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

  int video_height = frame.cols;
  int video_width = frame.rows;
  int cx = video_width/2;
  int cy = video_height/2;

  stdout_mutex->lock();
  cout << "video size: " << video_height << "x" << video_width << endl;
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

    Mat result = build_output(frame, s, settings_mutex);

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

Mat build_output(Mat frame, settings* s, mutex* settings_mutex) {
  const float min_size = 0.1;
  int width = frame.rows;
  int height = frame.cols;
  int x0 = width/2;
  int y0 = height/2;

  Mat result = Mat::zeros(width,height, frame.type());
  
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
  
  x1 = x0 - m1.rows- int(s->distance4 * (x0-m1.rows));
  y1 = y0 - m1.cols/2;

  x2 = x0 - m2.rows/2;
  y2 = y0 + int(s->distance1 * (y0-m2.cols));

  x3 = x0 + int(s->distance2 * (x0-m3.rows));
  y3 = y0 - m3.cols/2;
  
  x4 = x0 - m4.rows/2;
  y4 = y0 - m4.cols - int(s->distance3 * (y0-m4.cols));


  settings_mutex->unlock();

  //copy to output
  m1.copyTo(result(Rect(y1,x1,m1.cols,m1.rows)));
  m2.copyTo(result(Rect(y2,x2,m2.cols,m2.rows)));
  m3.copyTo(result(Rect(y3,x3,m3.cols,m3.rows)));
  m4.copyTo(result(Rect(y4,x4,m4.cols,m4.rows)));  
  
  return result;
}
  

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
