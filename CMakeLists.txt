cmake_minimum_required(VERSION 2.8)

project (Estetimacge)

set(EXECUTABLE_NAME estetimacge)



#c++11 support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
list( APPEND ThirdParty_LIBS ${OpenCV_LIBS} )
include_directories( ${OpenCV_INCLUDE_DIRS} )
endif( OpenCV_FOUND )


##Include eigen 
INCLUDE_DIRECTORIES ("~/eigen")
##INCLUDE_DIRECTORIES ("~/OpenCV")

## Compilation du programme
add_library(estetimacge_lib src/capture.cpp src/parameters.cpp   src/config.cpp src/notifications.cpp src/widget.cpp src/ioEigen.cpp src/pca.cpp src/detector/faceDetector.cpp src/detector/contourDetector.cpp src/detector/landscapeDetector.cpp) 



## programme estetimacge qui link avec la librairie
add_executable(estetimacge src/main.cpp)

find_library( SQLITE3_LIBRARY_RELEASE sqlite3 )



#opencv_core opencv_calib3d opencv_imgproc opencv_video opencv_features2d opencv_highgui opencv_nonfree #opencv_objdetect

target_link_libraries(estetimacge sqlite3 gphoto2 ${OpenCV_LIBS} -I ~/eigen)


target_link_libraries(estetimacge estetimacge_lib)


