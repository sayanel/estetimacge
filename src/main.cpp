#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <iostream>
#include <iomanip>
#include <string>

//#include "opencv/highgui.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include "gphoto2/gphoto2-camera.h"

#include "include/params.h"
#include "include/parameters.hpp"
#include "include/capture.hpp"
#include "include/notifications.hpp"
#include "include/config.hpp"
#include "include/widget.hpp"
#include "include/pca.hpp"


#include "detector/include/faceDetector.hpp"
#include "detector/include/contourDetector.hpp"
//#include "detector/include/landscapeDetector.hpp"

#include <sqlite3.h> 

//Compilo : 
//g++ -Wall -Wno-unused-local-typedefs main.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lgphoto2 -lopencv_video -I ~/eigen  -o main
//ou, cd build_estetimacge ; cmake ../estetimacge ; make -j ; ./estetimacge

using namespace cv;
using namespace std;

Camera *camera;  
GPContext *context; 

 /** @function main */
 int main( int argc, const char** argv ){
    int nbContours, nbPers, isPortrait = 0;
    
    /*Mat image = imread( argv[1], 1 );

    nbPers = faceDetector(image, isPortrait);
    cout << "NbPersonnes = " << nbPers << endl;
    if(isPortrait) cout << "PORTRAIT" << endl;
    //nbContours = contourDetector(image);
    //cout << "NbContours = " << nbContours << endl;

    //detectAndDisplayLandscape(image);

 



    Size size(image.cols/2, image.rows/2);
    Mat rz_image;
    resize(image,rz_image,size);//resize image
    /// Create Window
    string source_window = "_Estetimacge_";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, image );
    //imshow( source_window, rz_image );

	//int retval;
    //pca("../src/matrix/database.mat");
    //PCA pour test

    */


    gp_camera_new (&camera);
    context = gp_context_new();

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
 int nShotsDB = 0;
 int i;

cv::Mat img[10];

int decalagex = 30;
int decalagey = 30;

 for (i = 1; i <= nShots; i++) {
  snprintf(filename, 256, "shot-%04d.jpg", i);
  printf("Capturing to file %s\n", filename);
  print_parameters(camera, context);
  update_parameters(camera, context, aperture_tab[2],shutterspeed_tab[10],iso_tab[0]);

  // set_config_value_string(camera,"whitebalance","Auto",context);
  // set_config_value_string(camera,"meteringmode","Center-weighted average",context);
  // set_config_value_string(camera,"focusmode","AI Focus",context);
  // set_config_value_string(camera,"eoszoom","1",context);
  // set_config_value_string(camera,"manualfocusdrive","Near 1",context);
  // set_config_value_string(camera,"eoszoomposition","5,5",context);
  // set_config_value_string(camera,"whitebalanceadjusta","15",context);
  // set_config_value_string(camera,"whitebalanceadjustb","9",context);
  //
  //set_config_value_string(camera,"afdistance ","Auto",context);
  //set_config_value_string(camera,"exposurecompensation ","2",context);
  //set_config_value_string(camera,"iso","Auto",context);
  
  
  capture(camera, context, filename);
  print_parameters(camera, context);
  img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
 
    /* comments car pas de capture */

  cv::resize(img[i], img[i], cvSize(640, 480));
  //DETECTON DE PORTRAIT FORCEE !
  //Mat image = imread( argv[1], 1 );
  nbPers = faceDetector(img[i], isPortrait);
  //nbPers = faceDetector(img[i], isPortrait);
  cout << "NbPersonnes = " << nbPers << endl;
  if(isPortrait) { 
    cout << "PORTRAIT" << endl;
    nShotsDB = 6;

  }
  //nbContours = contourDetector(img[i]);
  cout << "NbContours = " << nbContours << endl;
 
  cvNamedWindow (filename, CV_WINDOW_AUTOSIZE);
  cv::resize(img[i], img[i], cvSize(320, 240));
  cvMoveWindow(filename, decalagex, decalagey);
  decalagex +=321;
  if(i%3 == 0){ decalagey += 241; decalagex = 30; }
  
  //Mat image = imread( filename, 1 );
  
  

  cv::imshow(filename, img[i]);



  //cvShowImage (filename, img[i]);
 }


// DEUXIEME PRISE DE VUE APRES DETECTION



for (i = 1; i <= nShotsDB; i++) {
  snprintf(filename, 256, "shot-%04d.jpg", i);
  std::cout << "__________________________________________________" << std::endl;
  print_parameters(camera, context);
  //update_parameters(camera, context, aperture_tab[i*3],shutterspeed_tab[20-i],iso_tab[0]);
  std::cout << "Initialisation ..." << std::endl;
  switch(i){

    case 1:
        std::cout << "Initialisation premiere photo" << std::endl;
        update_parameters(camera, context, aperture_tab[2],shutterspeed_tab[27],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[0],context);
        //set_config_value_string(camera,"aperture", aperture_tab[2],context);
        //set_config_value_string(camera,"shutterspeed", shutterspeed_tab[27],context);
        break;
    case 2:
        std::cout << "Initialisation 2eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[4],shutterspeed_tab[22],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[1],context);
        //set_config_value_string(camera,"aperture", aperture_tab[4],context);
        //set_config_value_string(camera,"shutterspeed", shutterspeed_tab[22],context);
        break;
    case 3:
        std::cout << "Initialisation 3eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[5],shutterspeed_tab[20],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[0],context);
        //set_config_value_string(camera,"aperture", aperture_tab[5],context);
        //set_config_value_string(camera,"shutterspeed", shutterspeed_tab[20],context);
        break;
    case 4:
        std::cout << "Initialisation 4eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[10],shutterspeed_tab[16],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[3],context);
        //set_config_value_string(camera,"aperture", aperture_tab[10],context);
        //set_config_value_string(camera,"shutterspeed", shutterspeed_tab[16],context);
        break;
    case 5:
        std::cout << "Initialisation 5eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[13],shutterspeed_tab[12],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[0],context);
        //set_config_value_string(camera,"aperture", aperture_tab[13],context);
        //set_config_value_string(camera,"shutterspeed", shutterspeed_tab[12],context);
        break;
    case 6:
        std::cout << "Initialisation 6eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[16],shutterspeed_tab[6],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[5],context);
        //set_config_value_string(camera,"aperture", aperture_tab[16],context);
        //set_config_value_string(camera,"shutterspeed", shutterspeed_tab[6],context);
        break;


    default:
        break;
  }

  //set_config_value_string(camera,"whitebalance", whitebalance_tab[i],context);
  //set_config_value_string(camera,"aperture", whitebalance_tab[i],context);
  //std::cout << "modif FOCUS" << std::endl;
  //set_config_value_string(camera,"manualfocusdrive", manualfocusdrive_tab[i+2],context);

  //set_config_value_string(camera,"whitebalance","Auto",context);
  // set_config_value_string(camera,"meteringmode","Center-weighted average",context);
  // set_config_value_string(camera,"focusmode","AI Focus",context);
  // set_config_value_string(camera,"eoszoom","1",context);
  // set_config_value_string(camera,"manualfocusdrive","Near 1",context);
  //set_config_value_string(camera,"eoszoomposition","5,5",context);
  // set_config_value_string(camera,"whitebalanceadjusta","15",context);
  // set_config_value_string(camera,"whitebalanceadjustb","9",context);
  //
  //set_config_value_string(camera,"afdistance ","Auto",context);
  //set_config_value_string(camera,"exposurecompensation ","2",context);
  //set_config_value_string(camera,"iso","Auto",context);
  
  
  capture(camera, context, filename);
  printf("Capturing to file %s\n", filename);
  print_parameters(camera, context);
  img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
 

  cvNamedWindow (filename, CV_WINDOW_AUTOSIZE);
  cv::resize(img[i], img[i], cvSize(320, 240));
  cvMoveWindow(filename, decalagex, decalagey);
  decalagex +=321;
  if(i%3 == 0){ decalagey += 241; decalagex = 30; }
  
  //Mat image = imread( filename, 1 );
  
  

  cv::imshow(filename, img[i]);



  //cvShowImage (filename, img[i]);
 }

 // FIN DEUXIEME PRISE DE VUE




 // close camera
 gp_camera_unref(camera);
 gp_context_unref(context);

//END OF GPHOTO PART
 //STARTING OPENCV

  

  
  
  
  cvWaitKey(0);
  cvDestroyAllWindows();
  for(i = 0; i < nShots; i++){
    img[i].release();
    //cvReleaseImage(&img[i]);

    waitKey(0); 
    return 0;
 }

}