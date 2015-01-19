#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "config.hpp"

#include <gphoto2/gphoto2-camera.h>


//int update_parameters(float, std::string, std::string);
int update_parameters(Camera *camera, GPContext *context, float aperture, std::string shutterspeed, std::string iso);
void print_parameters(Camera *camera, GPContext *context);