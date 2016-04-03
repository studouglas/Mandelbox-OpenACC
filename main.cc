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
#include "openacc.h"
#include "vector3d.h"
#include "color.h"

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
		   MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams camera_params, const RenderParams renderer_params, unsigned char* image);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);

#define NUM_FRAMES 1

#pragma acc declare copyin(mandelBox_params)
MandelBoxParams mandelBox_params;


vec3* d_to;
vec3* d_colours;
double* d_farPoints;
pixelData* d_pixData;
#pragma acc declare deviceptr(d_to, d_colours, d_farPoints, d_pixData)

int main(int argc, char** argv)
{
  CameraParams    camera_params;
  RenderParams    renderer_params;
  
  getParameters(argv[1], &camera_params, &renderer_params, &mandelBox_params);

  int image_size = renderer_params.width * renderer_params.height;
  unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));

  d_to = (vec3*)acc_malloc(image_size * sizeof(vec3));
  d_colours = (vec3*)acc_malloc(image_size * sizeof(vec3));
  d_farPoints = (double*)acc_malloc(image_size * 3 * sizeof(double));
  d_pixData = (pixelData*)acc_malloc(image_size * sizeof(pixelData));


  init3D(&camera_params, &renderer_params);

  for (int i = 0; i < NUM_FRAMES; i++) {
    renderFractal(camera_params, renderer_params, image);
  }
  saveBMP(renderer_params.file_name, image, renderer_params.width, renderer_params.height);
  
  free(image);

  acc_free(d_to);
  acc_free(d_colours);
  acc_free(d_farPoints);
  acc_free(d_pixData);

  return 0;
}
