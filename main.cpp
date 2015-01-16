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

#include <gphoto2/gphoto2-camera.h>

#include "params.h"

//COmpilo : 
//g++ -Wall -Wno-unused-local-typedefs main.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lgphoto2 -lopencv_video -I ~/eigen  -o main


Camera *camera;  
GPContext *context; 


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

int update_parameters(float aperture, std::string shutterspeed, std::string iso){

	char buffer[32];
	//char buffer3[32];
	//char * chaine;

	//get_config_value_string(camera,"aperture",&chaine,context);

	//if(*chaine != aperture){
		if((float)aperture - (int)aperture != 0) snprintf(buffer,32,"%.1f",aperture);
		else snprintf(buffer,32,"%.f",aperture);
  		set_config_value_string(camera,"aperture",buffer,context);
  		std::cout << "Modification de aperture : "<< buffer<<std::endl;
  	//}

	int size_shutterspeed = shutterspeed.size();
	char str_shutterspeed[size_shutterspeed];
	for(int i = 0; i< size_shutterspeed; ++i){
		str_shutterspeed[i] = shutterspeed[i];
	}
	str_shutterspeed[size_shutterspeed] = '\0';

  	set_config_value_string(camera,"shutterspeed",str_shutterspeed,context);
  	std::cout << "Modification de shutterspeed : "<< str_shutterspeed <<std::endl;
  	//}


  	//get_config_value_string(camera,"iso",&chaine,context);
  	//if(*chaine != iso){
	int size_iso = iso.size();
	char str_iso[size_iso];
	for(int i = 0; i< size_iso; ++i){
		str_iso[i] = iso[i];
	}
	str_iso[size_iso] = '\0';

  	set_config_value_string(camera,"iso",str_iso,context);
  	std::cout << "Modification de iso : "<< str_iso <<std::endl;
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

	get_config_value_string(camera,"whitebalance",&chaine,context);
	std::cout<<"whitebalance : " << chaine <<std::endl;

	get_config_value_string(camera,"meteringmode",&chaine,context);
	std::cout<<"meteringmode : " << chaine <<std::endl;

	get_config_value_string(camera,"manualfocusdrive",&chaine,context);
	std::cout<<"manualfocusdrive : " << chaine <<std::endl;

	get_config_value_string(camera,"eoszoomposition",&chaine,context);
	std::cout<<"eoszoomposition : " << chaine <<std::endl;



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
 int const nShots = 1;
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


//gphoto2 --list-config

 for (i = 1; i <= nShots; i++) {
  snprintf(filename, 256, "shot-%04d.nef", i);
  printf("Capturing to file %s\n", filename);
  //print_parameters();
  update_parameters(aperture_tab[i+4],shutterspeed_tab[24-i],iso_tab[0]);

  set_config_value_string(camera,"whitebalance","Auto",context);
  set_config_value_string(camera,"meteringmode","Center-weighted average",context);
  set_config_value_string(camera,"focusmode","AI Focus",context);
  set_config_value_string(camera,"eoszoom","1",context);
  set_config_value_string(camera,"manualfocusdrive","Near 1",context);
  set_config_value_string(camera,"eoszoomposition","5,5",context);
  set_config_value_string(camera,"whitebalanceadjusta","15",context);
  set_config_value_string(camera,"whitebalanceadjustb","9",context);
  //
  //set_config_value_string(camera,"afdistance ","Auto",context);
  //set_config_value_string(camera,"exposurecompensation ","2",context);
  //set_config_value_string(camera,"iso","Auto",context);
  
  



/* system, ne fonctionne pas car le programme c++ a la main sur l'appareil
	int i;
  printf ("Checking if processor is available...");
  if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
  std::cout << "Executing command DIR..." << std::endl;
  i=system ("gphoto2 --set-config whitebalance=2");
  printf ("The value returned was: %d.\n",i);
*/

  capture(filename);
  print_parameters();
  img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
 

  /*if (img[i] == NULL)
  {
    fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return EXIT_FAILURE;
  }*/
  //cvResize(img[i],img[i], cvSize(320, 240));
  
    /* comments car pas de capture */
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