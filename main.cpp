#include <stdio.h>
#include <stdlib.h>
//C shit for I/O 
#include <fcntl.h>

#include <iostream>
#include <string>

#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>


//#include <Eigen/Dense>


#include <gphoto2/gphoto2-camera.h>



//COmpilo : 
//g++ -Wall -Wno-unused-local-typedefs main.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lgphoto2 -lopencv_video -I ~/eigen  -o main


Camera *camera;  
GPContext *context; 

float apertab[17] = { 
	3.5,
	4.0,
	4.5,
	5,
	5.6,
	6.3,
	7.1,
	8,
	9,
	10,
	11,
	13,
	14,
	16,
	18,
	20,
	22};

	int isotab[7] = {
		100,
		200,
		400,
		800,
		1600,
		3200,
		6400
	};


std::string speedtab[44] = {
"5",
"4",
"3.2",
"2.5",
"2",
"1.6",
"1.3",
"1",
"0.8",
"0.6",
"0.5",
"0.4",
"0.3",
"1/4",
"1/5",
"1/6",
"1/8",
"1/10",
"1/13",
"1/15",
"1/20",
"1/25",
"1/30",
"1/40",
"1/50",
"1/60",
"1/80",
"1/100",
"1/125",
"1/160",
"1/200",
"1/250",
"1/320",
"1/400",
"1/500",
"1/640",
"1/800",
"1/1000",
"1/1250",
"1/1600",
"1/2000",
"1/2500",
"1/3200",
"1/4000"};


static int
_lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child) {
	int ret;
	ret = gp_widget_get_child_by_name (widget, key, child);
	if (ret < GP_OK)
		ret = gp_widget_get_child_by_label (widget, key, child);
	return ret;
}


static int
get_config_value_string (Camera *camera, const char *key, char **str, GPContext *context) {
	CameraWidget		*widget = NULL, *child = NULL;
	CameraWidgetType	type;
	int			ret;
	char			*val;

	ret = gp_camera_get_config (camera, &widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "camera_get_config failed: %d\n", ret);
		return ret;
	}
	ret = _lookup_widget (widget, key, &child);
	if (ret < GP_OK) {
		fprintf (stderr, "lookup widget failed: %d\n", ret);
		goto out;
	}

	/* This type check is optional, if you know what type the label
	 * has already. If you are not sure, better check. */
	ret = gp_widget_get_type (child, &type);
	if (ret < GP_OK) {
		fprintf (stderr, "widget get type failed: %d\n", ret);
		goto out;
	}
	switch (type) {
        case GP_WIDGET_MENU:
        case GP_WIDGET_RADIO:
        case GP_WIDGET_TEXT:
		break;
	default:
		fprintf (stderr, "widget has bad type %d\n", type);
		ret = GP_ERROR_BAD_PARAMETERS;
		goto out;
	}

	/* This is the actual query call. Note that we just
	 * a pointer reference to the string, not a copy... */
	ret = gp_widget_get_value (child, &val);
	if (ret < GP_OK) {
		fprintf (stderr, "could not query widget value: %d\n", ret);
		goto out;
	}
	/* Create a new copy for our caller. */
	//std::cout << "get : "<< val <<std::endl;
	*str = strdup (val);
out:
	gp_widget_free (widget);
	return ret;
}



/* Sets a string configuration value.
 * This can set for:
 *  - A Text widget
 *  - The current selection of a Radio Button choice
 *  - The current selection of a Menu choice
 *
 * Sample (for Canons eg):
 *   get_config_value_string (camera, "owner", &ownerstr, context);
 */
static int
set_config_value_string (Camera *camera, const char *key, const char *val, GPContext *context) {
	CameraWidget		*widget = NULL, *child = NULL;
	CameraWidgetType	type;
	int			ret;

	ret = gp_camera_get_config (camera, &widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "camera_get_config failed: %d\n", ret);
		return ret;
	}
	ret = _lookup_widget (widget, key, &child);
	if (ret < GP_OK) {
		fprintf (stderr, "lookup widget failed: %d\n", ret);
		goto out;
	}

	/* This type check is optional, if you know what type the label
	 * has already. If you are not sure, better check. */
	ret = gp_widget_get_type (child, &type);
	if (ret < GP_OK) {
		fprintf (stderr, "widget get type failed: %d\n", ret);
		goto out;
	}
	switch (type) {
        case GP_WIDGET_MENU:
        case GP_WIDGET_RADIO:
        case GP_WIDGET_TEXT:
		/* This is the actual set call. Note that we keep
		 * ownership of the string and have to free it if necessary.
		 */
		ret = gp_widget_set_value (child, val);
		if (ret < GP_OK) {
			fprintf (stderr, "could not set widget value: %d\n", ret);
			goto out;
		}
		break;
        case GP_WIDGET_TOGGLE: {
		int ival;

		sscanf(val,"%d",&ival);
		ret = gp_widget_set_value (child, &ival);
		if (ret < GP_OK) {
			fprintf (stderr, "could not set widget value: %d\n", ret);
			goto out;
		}
		break;
	}
	default:
		fprintf (stderr, "widget has bad type %d\n", type);
		ret = GP_ERROR_BAD_PARAMETERS;
		goto out;
	}

	/* This stores it on the camera again */
	ret = gp_camera_set_config (camera, widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "camera_set_config failed: %d\n", ret);
		return ret;
	}
out:
	gp_widget_free (widget);
	return ret;
}


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

int update_parameters(float aperture, std::string shutterspeed, int iso){

	char buffer[32];
	char buffer2[32];
	//char buffer3[32];
	
	//char * chaine;

	//get_config_value_string(camera,"aperture",&chaine,context);

	//if(*chaine != aperture){
		if((float)aperture - (int)aperture != 0) snprintf(buffer,32,"%.1f",aperture);
		else snprintf(buffer,32,"%.f",aperture);
  		set_config_value_string(camera,"aperture",buffer,context);
  		std::cout << "Modification de aperture : "<< buffer<<std::endl;
  	//}

  	//get_config_value_string(camera,"shutterspeed",&chaine,context);
  	//if(*chaine != shutterspeed){

	/*snprintf(buffer,32,"%d",shutterspeed_denom);
	snprintf(buffer2,32,"%d",shutterspeed_num);
	strcpy(str,buffer);
	strcat(str,"/");
	strcat(str,buffer2);*/
	//strcpy(str,shutterspeed);
	int shuttersize = shutterspeed.size();
	char str[shuttersize];
	for(int i = 0; i< shuttersize; ++i){
		str[i] = shutterspeed[i];
	}
  	set_config_value_string(camera,"shutterspeed",str,context);
  	std::cout << "Modification de shutterspeed : "<< str<<std::endl;
  	//}


  	//get_config_value_string(camera,"iso",&chaine,context);
  	//if(*chaine != iso){
	snprintf(buffer2,32,"%d",iso);
  	set_config_value_string(camera,"iso",buffer2,context);
  	std::cout << "Modification de iso : "<< buffer2<<std::endl;
  	//}


	return 1;
}

void print_parameters(){
	char * chaine;
	std::cout << "*************"<<std::endl;
	get_config_value_string(camera,"aperture",&chaine,context);
	std::cout<<"Aperture = " << chaine <<std::endl;

	get_config_value_string(camera,"shutterspeed",&chaine,context);
	std::cout<<"shutterspeed : " << chaine <<std::endl;

	get_config_value_string(camera,"iso",&chaine,context);
	std::cout<<"iso : " << chaine <<std::endl;


}



int main (int argc, char* argv[])
{

	//int retval;
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
 int const nShots = 1


 ;
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
 
int decalagex = 30;
int decalagey = 30;
 // do some capturing

//const float val = 0.0;
 
 //sprintf(buffer,"%f",val);
 /*retval =set_config_value_string(camera,"aperture","25",context);
		if (retval != GP_OK) {
			printf("  failed setting shutterspeed to %f: %d\n", val, retval);


			exit (1);
		}*/


 for (i = 1; i <= nShots; i++) {
  snprintf(filename, 256, "shot-%04d.nef", i);
  printf("Capturing to file %s\n", filename);
  print_parameters();
  //update_parameters(apertab[i+4],speedtab[24-i],isotab[i%3]);
  
  set_config_value_string(camera,"whitebalance","1",context);
  /*
  int ret;
  CameraWidget		*widget = NULL, *child = NULL;
  CameraWidgetType	type;
  gp_camera_get_config (camera, &widget, context);
  _lookup_widget (widget, "shutterspeed", &child);
  gp_widget_get_type (child, &type);
  const int val = 2;
  ret = gp_widget_set_value (child, &val);
  std::cout << ret <<" " << GP_OK <<" " << std::endl;
  gp_camera_set_config (camera, widget, context);
  */
  //capture(filename);
  print_parameters();
  img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
  /*if (img[i] == NULL)
  {
    fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return EXIT_FAILURE;
  }*/
  //cvResize(img[i],img[i], cvSize(320, 240));
  cvNamedWindow (filename, CV_WINDOW_AUTOSIZE);
  cv::resize(img[i], img[i], cvSize(320, 240));
  cvMoveWindow(filename, decalagex, decalagey);
  decalagex +=321;
  if(i%3 == 0){ decalagey += 241; decalagex = 30; }
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