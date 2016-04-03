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
#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "mandelbox.h"
#include "camera.h"
#include "vector3d.h"
#include "3d.h"
#include "openacc.h"

extern double getTime();
extern void   printProgress( double perc, double time );

#pragma acc routine seq
extern void rayMarch (const RenderParams &render_params, const vec3 &from, const vec3  &to, double eps, pixelData &pix_data);

#pragma acc routine seq
extern void getColour(vec3 & colour, const pixelData &pixData, const RenderParams &render_params, const vec3 &from, const vec3 &direction);

extern MandelBoxParams mandelBox_params;
extern vec3* d_to;
extern vec3* d_colours;
extern double* d_farPoints;
extern pixelData* d_pixData;

void renderFractal(const CameraParams camera_params, const RenderParams renderer_params, unsigned char* image)
{
  const double eps = pow(10.0, renderer_params.detail); 
  vec3 fromTemp;
  VEC(fromTemp, camera_params.camPos[0], camera_params.camPos[1], camera_params.camPos[2]);
  const vec3 from = fromTemp;

  const int height = renderer_params.height;
  const int width  = renderer_params.width;
  const int n = width*height;

  // [d_to.x, d_to.y, d_to.z]
  // const int NUM_TEST_VALS = 9;
  // double* testResults = (double*)malloc(n * NUM_TEST_VALS * sizeof(double));

  //printf("Starting data region...\n");
  // #pragma acc data copy(testResults[:n*NUM_TEST_VALS])
  #pragma acc data copyin(camera_params, renderer_params, eps, from)
  #pragma acc data deviceptr(d_to, d_colours, d_farPoints, d_pixData)
  #pragma acc data copyout(image[:n*3])
  {
    //printf("Starting parallel loop...\n");
    #pragma acc kernels loop independent collapse(2)
    for(int j = 0; j < height; j++)
    {
      for(int i = 0; i < width; i++)
      {
      	int k = j*width + i;

        UnProject(i, j, camera_params, &(d_farPoints[k*3]));
        
       	SUBTRACT_DARRS(d_to[k], (&(d_farPoints[k*3])), camera_params.camPos);
        NORMALIZE(d_to[k]);
        
        // 'd_to' seems good, but isn't const!
        // 'from' vector seems good
        // 'eps' seems good
        // 'renderer_params.maxDistance' looks good

        rayMarch(renderer_params, from, d_to[k], eps, d_pixData[k]);

        // get the colour at this pixel
        getColour(d_colours[k], d_pixData[k], renderer_params, from, d_to[k]);

        //save colour into texture
        image[k*3 + 2] = (unsigned char)(d_colours[k].x * 255);
        image[k*3 + 1] = (unsigned char)(d_colours[k].y * 255);
        image[k*3]     = (unsigned char)(d_colours[k].z * 255);
      }
    }
  }
  //printf("\nRendering done\n");
  // for (int i = 0; i < n; i++) {
  //     int k = i*NUM_TEST_VALS;
  //     printf("[i = %4d] ", i);
  //     printf("from = [%f,%f,%f] | ", testResults[k], testResults[k+1], testResults[k+2]);
  //     printf("dir = [%f,%f,%f] | ", testResults[k+3], testResults[k+4], testResults[k+5]);
  //     printf("d_to[k] = [%f,%f,%f]\n " , testResults[k+6],testResults[k+7],testResults[k+8]);
  // }
  //printf("\n\n");
}
