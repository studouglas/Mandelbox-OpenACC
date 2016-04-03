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
extern void rayMarch (const RenderParams &render_params, const vec3 &from, vec3  &to, double eps, pixelData &pix_data);

#pragma acc routine seq
extern void getColour(vec3 & colour, const pixelData &pixData, const RenderParams &render_params, const vec3 &from, const vec3 &direction);

extern MandelBoxParams mandelBox_params;

void renderFractal(const CameraParams camera_params, const RenderParams renderer_params, unsigned char* image)
{
  // shared among all threads
  const double eps = pow(10.0, renderer_params.detail); 
  vec3 f;
  VEC(f, camera_params.camPos[0], camera_params.camPos[1], camera_params.camPos[2]);
  const vec3 from = f;

  const int height = renderer_params.height;
  const int width  = renderer_params.width;
  const int n = width*height;

  // pixelData pix_data;
  double time = getTime();
  
  vec3* d_to = (vec3*)acc_malloc(n * sizeof(vec3));
  vec3* d_colours = (vec3*)acc_malloc(n * sizeof(vec3));
  double* d_farPoints = (double*)acc_malloc(n * 3 * sizeof(double));
  pixelData* d_pixData = (pixelData*)acc_malloc(n * sizeof(pixelData));

  // [d_to.x, d_to.y, d_to.z]
  const int NUM_TEST_VALS = 8;
  double* testResults = (double*)malloc(n * NUM_TEST_VALS * sizeof(double));

  printf("Starting data region...\n");
  #pragma acc data copy(testResults[:n*NUM_TEST_VALS])
  #pragma acc data copyin(camera_params, renderer_params)
  #pragma acc data deviceptr(d_to, d_colours, d_farPoints, d_pixData)
  #pragma acc data copyin(eps, from)
  #pragma acc data copy(image[0:n*3])
  {
    printf("Starting parallel loop...\n");
    #pragma acc kernels loop independent collapse(2)
    for(int j = 0; j < height; j++)
    {
      for(int i = 0; i < width; i++)
      {
      	int k = j*width + i;

        UnProject(i, j, camera_params, &(d_farPoints[k*3]));
        
       	SUBTRACT_DARRS(d_to[k], (&(d_farPoints[k*3])), camera_params.camPos);
        NORMALIZE(d_to[k]);
        // 'd_to' seems good
        // 'from' vector seems good
        // 'eps' seems good
        // 'renderer_params.maxDistance' looks good

        // render the pixel
        // vec3 tests1;
        // vec3 tests2;
        
        // d_to[k] is fine before hand, but within rayMarch it only sees []
        // const vec3 dir = d_to[k];
        rayMarch(renderer_params, from, d_to[k], eps, d_pixData[k]);
        // testResults[k*NUM_TEST_VALS    ] = (d_pixData[k].escaped) ? 1.0 : 0.0;
        // testResults[k*NUM_TEST_VALS + 2] = tests1.x;
        // testResults[k*NUM_TEST_VALS + 3] = tests1.y;
        // testResults[k*NUM_TEST_VALS + 4] = tests1.z;
        // testResults[k*NUM_TEST_VALS + 5] = tests2.x;
        // testResults[k*NUM_TEST_VALS + 6] = tests2.y;
        // testResults[k*NUM_TEST_VALS + 7] = tests2.z;

        // get the colour at this pixel
        getColour(d_colours[k], d_pixData[k], renderer_params, from, d_to[k]);
        // testResults[k*NUM_TEST_VALS+1] = d_colours[k].x;

        //save colour into texture
        image[k*3 + 2] = (unsigned char)(d_colours[k].x * 255);
        image[k*3 + 1] = (unsigned char)(d_colours[k].y * 255);
        image[k*3]     = (unsigned char)(d_colours[k].z * 255);
      }
    }
  }

  printf("\nRendering done\n");
  // for (int i = 0; i < n; i++) {
  //     int k = i*NUM_TEST_VALS;
  //     printf("[i = %4d] ", i);
  //     printf("escaped  = %f | ", testResults[k]);
  //     printf("colour.r = %f | ", testResults[k+1]);
  //     printf("t0 (magnitude) = %f | ", testResults[k+2]);
  //     printf("t1 (cf.x) = %f | ", testResults[k+3]);
  //     printf("t2 (cf.y) = %f | " , testResults[k+4]);
  //     printf("t3 (DE(from)) = %f | " , testResults[k+5]);
  //     printf("t4 (dot(from))) = %f | " , testResults[k+6]);
  //     printf("t5 (scalar.x) = %f\n" , testResults[k+7]);
  // }
  printf("\n\n");
}
