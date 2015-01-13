#include <stdio.h>
#include <stdlib.h>
//C shit for I/O 
#include <fcntl.h>

#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>


//#include <Eigen/Dense>


#include <gphoto2/gphoto2-camera.h>



//COmpilo : 
// g++ -Wall -Wno-unused-local-typedefs Estetimacge.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -I ~/eigen  -o Estetimacge


Camera *camera;  
GPContext *context; 



void error_func (GPContext *context, const char *format, va_list args, void *data) {
 fprintf  (stderr, "*** Contexterror ***\n");
 vfprintf (stderr, format, args);
 fprintf  (stderr, "\n");
}

void message_func (GPContext *context, const char *format, va_list args, void *data) {
 vprintf (format, args);
 printf ("\n");
}


int capture (const char *filename) {
 int fd, retval;
 CameraFile *file;
 CameraFilePath camera_file_path;

 // this was done in the libphoto2 example code, but doesn't seem to be necessary
 // NOP: This gets overridden in the library to /capt0000.jpg
 //snprintf(camera_file_path.folder, 1024, "/");
 //snprintf(camera_file_path.name, 128, "foo.jpg");

 // take a shot
 retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
 
 if (retval) {
  // do some error handling, probably return from this function
 }

 printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

 fd = open(filename, O_CREAT | O_WRONLY, 0644);
 // create new CameraFile object from a file descriptor
 retval = gp_file_new_from_fd(&file, fd);
 
 if (retval) {
  // error handling
 }

 // copy picture from camera
 retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
   GP_FILE_TYPE_NORMAL, file, context);
 
 if (retval) {
  // error handling
 }

 printf("Deleting\n");
 // remove picture from camera memory
 retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
   context);
 
 if (retval) {
  // error handling
 }

 // free CameraFile object
 gp_file_free(file);

 // Code from here waits for camera to complete everything.
 // Trying to take two successive captures without waiting
 // will result in the camera getting randomly stuck.
 int waittime = 100;
 CameraEventType type;
 void *data;

 printf("Wait for events from camera\n");
 while(1) {
  retval = gp_camera_wait_for_event(camera, waittime, &type, &data, context);
  if(type == GP_EVENT_TIMEOUT) {
   break;
  }
  else if (type == GP_EVENT_CAPTURE_COMPLETE) {
   printf("Capture completed\n");
   waittime = 100;
  }
  else if (type != GP_EVENT_UNKNOWN) {
   printf("Unexpected event received from camera: %d\n", (int)type);
  }
 }
 return 0;
}


int main (int argc, char* argv[])
{


    gp_camera_new (&camera);
 context = gp_context_new();

 // set callbacks for camera messages
 //gp_context_set_error_func(context, error_func, NULL);
 //gp_context_set_message_func(context, message_func, NULL);

 //This call will autodetect cameras, take the first one from the list and use it
 printf("Camera init. Can take more than 10 seconds depending on the "
  "memory card's contents (remove card from camera to speed up).\n");
 int ret = gp_camera_init(camera, context);
 if (ret < GP_OK) {
  printf("No camera auto detected.\n");
  gp_camera_free(camera);
  return 1;
 }

 // take 10 shots
 char filename[256];
 int const nShots = 2;
 int i;


//Image de base
//IplImage* basic = cvCreateImage(cvSize(320, 240), 8, 3);
//IplImage* img[10] = {NULL};
//const char* window_title = "Hello, OpenCV!";
//cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);

cv::Mat img[10];// = {NULL}; //= cv::imread("something.jpg");
//cv::Mat src = cv::imread("src.jpg");

// Resize src so that is has the same size as img
//cv::resize(src, src, img.size());
 
int decalagex = 100;
 // do some capturing
 for (i = 0; i < nShots; i++) {
  snprintf(filename, 256, "shot-%04d.nef", i);
  printf("Capturing to file %s\n", filename);
  capture(filename);
  img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
  /*if (img[i] == NULL)
  {
    fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return EXIT_FAILURE;
  }*/
  //cvResize(img[i],img[i], cvSize(320, 240));
    cvNamedWindow (filename, CV_WINDOW_AUTOSIZE);
  cv::resize(img[i], img[i], cvSize(320, 240));
  cvMoveWindow(filename, decalagex, 100);
  decalagex +=321;
  cv::imshow(filename,img[i]);
  //cvShowImage (window_title, img[i]);
 }

 // close camera
 gp_camera_unref(camera);
 gp_context_unref(context);

//END OF GPHOTO PART
 //STARTING OPENCV

  
  /*if (argc < 2)
  {
    fprintf (stderr, "usage: %s IMAGE\n", argv[0]);
    return EXIT_FAILURE;
  }*/
  
  
  
  //cvShowImage (window_title, img[1]);
  cvWaitKey(0);
  cvDestroyAllWindows();
  for(i = 0; i < nShots; i++){
    img[i].release();
    //cvReleaseImage(&img[i]);
  }
  return EXIT_SUCCESS;
}