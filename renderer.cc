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

#include "color.h"
#include "mandelbox.h"
#include "camera.h"
#include "vector3d.h"
#include "3d.h"

extern double getTime();
extern void   printProgress( double perc, double time );

extern void rayMarch (const RenderParams &render_params, const vec3 &from, const vec3  &to, double eps, pixelData &pix_data);
#pragma acc routine seq
extern vec3 getColour(const pixelData &pixData, const RenderParams &render_params,
                      const vec3 &from, const vec3  &direction);

// inline vec3 SUBTRACT_POINT(const double* p1, const double* p2) {
// 	vec3 res;
// 	res.x = p1[0] - p2[0];
// 	res.y = p1[1] - p2[1];
// 	res.z = p1[2] - p2[2];
// 	return res;
// }
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, 
       unsigned char* image)
{
  // shared among all threads
  const double eps = pow(10.0, renderer_params.detail); 
  vec3 from;
  VEC(from, camera_params.camPos[0], camera_params.camPos[1], camera_params.camPos[2]);
  
  const int height = renderer_params.height;
  const int width  = renderer_params.width;
  
  pixelData pix_data;
  double farPoint[3] = {0,0,0};
  double time = getTime();
  int i,j;
  
  #pragma acc data copyin(pix_data, eps, from, camera_params[:sizeof(CameraParams)], farPoint)
  #pragma acc data copy(image[0:width * height * 3])
  {
    #pragma acc kernels loop private(farPoint)
    for(j = 0; j < height; j++)
    {
      for(i = 0; i < width; i++)
      {
          vec3 to = {1,2,3};
          vec3 color = {2,3,4};
          
      
         // get point on the 'far' plane
         // since we render one frame only, we can use the more specialized method
         // UnProject(i, j, camera_params, farPoint);
        
       	 SUBTRACT_DARRS(to, farPoint, camera_params.camPos);
         NORMALIZE(to);
        
         //render the pixel
         // rayMarch(renderer_params, from, to, eps, pix_data);
        
         //get the colour at this pixel
         // color = getColour(pix_data, renderer_params, from, to);
          
         //save colour into texture
         int k = (j * width + i) * 3;
         image[k+2] = (unsigned char)(color.x * 255);
         image[k+1] = (unsigned char)(color.y * 255);
         image[k]   = (unsigned char)(color.z * 255);
      }
    }
  }
  printf("\n rendering done:\n");
}
