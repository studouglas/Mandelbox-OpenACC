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
#include <assert.h>
#include <algorithm>
#include <stdio.h>

#include "color.h"
#include "renderer.h"

// #pragma acc routine seq
extern double DE(const vec3 &p);
void normal (const vec3 & p, vec3 & normal);

// #pragma acc routine seq
void rayMarch(const RenderParams &render_params, const vec3 &from, const vec3 &direction, double eps,
	      pixelData& pix_data)
{
  double dist = 0.0;
  double totalDist = 0.0;
  
  // We will adjust the minimum distance based on the current zoom

  double epsModified = 0.0;
  
  int steps=0;
  vec3 p;
  do 
    {
    	MULT_SCALAR(p, direction, totalDist);
    	ADD_POINT(p, p, from);
      dist = DE(p);
      
      totalDist += .95*dist;
      
      epsModified = totalDist;
      epsModified*=eps;
      steps++;
    }
  while (dist > epsModified && totalDist <= render_params.maxDistance && steps < render_params.maxRaySteps);
  
  if (dist < epsModified) 
    {
      //we didnt escape
      pix_data.escaped = false;
      
      // We hit something, or reached MaxRaySteps
      pix_data.hit = p;
      
      //figure out the normal of the surface at this point
      vec3 temp;
      MULT_SCALAR(temp, direction, epsModified);
      SUBTRACT_POINT(temp, p, temp);
      const vec3 normPos = temp;
      normal(normPos, pix_data.normal);
    }
  else 
    //we have the background colour
    pix_data.escaped = true;
}


void normal(const vec3 & p, vec3 & normal)
{
  // compute the normal at p
  const double sqrt_mach_eps = 1.4901e-08;

  double eps = std::max( MAGNITUDE(p), 1.0 )*sqrt_mach_eps;

  vec3 e1, e2, e3;
  VEC(e1, eps, 0,   0);
  VEC(e2, 0  , eps, 0);
  VEC(e3, 0  , 0, eps);
  
  vec3 t1, t2, t3, t4, t5, t6;
  ADD_POINT(t1, p, e1);
  SUBTRACT_POINT(t2, p, e1);
  ADD_POINT(t3, p, e2);
  SUBTRACT_POINT(t4, p, e2);
  ADD_POINT(t5, p, e3);
  SUBTRACT_POINT(t6, p, e3);
  VEC(normal, DE(t1)-DE(t2), DE(t3)-DE(t4), DE(t5)-DE(t6));
  
  NORMALIZE(normal);
}
