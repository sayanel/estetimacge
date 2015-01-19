#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <gphoto2/gphoto2-camera.h>


int capture (Camera *camera, GPContext *context, const char * filename);