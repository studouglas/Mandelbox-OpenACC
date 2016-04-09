/*
   This file is part of the Mandelbox program developed for the course
    CS/SE  Distributed Computer Systems taught by N. Nedialkov in the
    Winter of 2015-2016 at McMaster University.

    Copyright (C) 2015-2016 T. Gwosdz and N. Nedialkov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>


#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"
#include "vector3d.h"
#include "color.h"
#include "openacc.h"

//#include <thread>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>

#define NUM_FRAMES 20
#define MOV_SPEED 0.005
#define CAM_SPEED 0.05
#define FRAMES_FOR_NEW_TARGET 20
#define DISTANCE_THRESHOLD 5

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
		               MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams camera_params, const RenderParams renderer_params, unsigned char* image);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);

void genNewCamParams(CameraParams &curCam, CameraParams &nextCam){
	curCam.camPos[0] += (nextCam.camPos[0] - curCam.camPos[0])*0.01;
	curCam.camPos[1] += (nextCam.camPos[1] - curCam.camPos[1])*0.01;
	curCam.camPos[2] += (nextCam.camPos[2] - curCam.camPos[2])*0.01;
}

/***********************************************
* Global Variables (public)
***********************************************/
MandelBoxParams mandelBox_params;

// set by renderer.cc after each renderFractal
vec3 newLookAt; 

// device pointers, so we only allocate once and free once in entire program execution
vec3* d_to;
vec3* d_colours;
double* d_farPoints;
pixelData* d_pixData;
double* d_distances;
#pragma acc declare deviceptr(d_to, d_colours, d_farPoints, d_pixData, d_distances) copyin(mandelBox_params)

int main(int argc, char** argv)
{
  CameraParams camera_params;
  RenderParams renderer_params;
  getParameters(argv[1], &camera_params, &renderer_params, &mandelBox_params);

  int image_size = renderer_params.width * renderer_params.height;
  unsigned char *image1 = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
  unsigned char *image2 = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
  unsigned char *currImage;

  d_to = (vec3*)acc_malloc(image_size * sizeof(vec3));
  d_colours = (vec3*)acc_malloc(image_size * sizeof(vec3));
  d_farPoints = (double*)acc_malloc(image_size * 3 * sizeof(double));
  d_pixData = (pixelData*)acc_malloc(image_size * sizeof(pixelData));
  d_distances = (double*)acc_malloc(image_size * sizeof(double));

  vec3 furthestPoint;
  char new_file_name[80];

  for (int i = 0; i < NUM_FRAMES; i++) {
	  if (i % 2 == 0) {
      currImage = image1;  
    } else {
      currImage = image2;
    }

    init3D(&camera_params, &renderer_params);
    renderFractal(camera_params, renderer_params, currImage);
    
    vec3 camTarget, camPos;
    VEC(camTarget, camera_params.camTarget[0], camera_params.camTarget[1], camera_params.camTarget[2]);
    VEC(camPos, camera_params.camPos[0], camera_params.camPos[1], camera_params.camPos[2]);

    double distanceFromTarget = DISTANCE_APART(camTarget, newLookAt);
    if (distanceFromTarget < DISTANCE_THRESHOLD) {
      // b-spline interpolation
      // camPos =        A
      // furthestPoint = B
      // newLookAt =     C
      
    }

    if (i % 10 == 0) {
      printf("Done rendering frame %d. furthestPoint = [%f,%f,%f]\n", i, newLookAt.x, newLookAt.y, newLookAt.z);
    }
    sprintf(new_file_name, "image_%d.bmp", i);
    saveBMP(new_file_name, currImage, renderer_params.width, renderer_params.height);  
  }
  free(image1);
  free(image2);

  acc_free(d_to);
  acc_free(d_colours);
  acc_free(d_farPoints);
  acc_free(d_pixData);
  acc_free(d_distances);

  return 0;
}
