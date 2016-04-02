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

#include "color.h"
#include "renderer.h"
#include "mandelbox.h"
#include "vector3d.h"

#define SQR(x) ((x)*(x))
#define COMPONENT_FOLD(x) { (x) = (fabs(x) <= 1) ? (x) : (((x) > 0) ? (2-(x)-(x)) : (-2-(x)-(x))); }

#pragma acc declare copyin(mandelBox_params)
extern MandelBoxParams mandelBox_params;

inline double MandelBoxDE(const vec3 &p0, const MandelBoxParams &params, double c1, double c2)
{
  vec3 p = p0;
  double rMin2   = SQR(params.rMin);
  double rFixed2 = SQR(params.rFixed);
  double escape  = SQR(params.escape_time);
  double dfactor = 1; 
  double r2      = -1;
  const double rFixed2rMin2 = rFixed2/rMin2;

  int i = 0;
  while (i < params.num_iter && r2 < escape)
  {
    COMPONENT_FOLD(p.x);
    COMPONENT_FOLD(p.y);
    COMPONENT_FOLD(p.z);
    
    DOT_ASSIGN(r2,p);

    if (r2 < rMin2)
    {
      MULT_SCALAR(p, p, rFixed2rMin2);
      dfactor *= (rFixed2rMin2);
    }
    else if (r2 < rFixed2) 
    {
      const double t = (rFixed2/r2);
      MULT_SCALAR(p, p, (rFixed2/r2));
      dfactor *= t;
    }
    
    dfactor = dfactor * fabs(params.scale) + 1.0;      
    MULT_SCALAR(p, p, params.scale);
    ADD_POINT(p,p,p0);
    i++;
  }
  
  return  (MAGNITUDE(p) - c1) / dfactor - c2;
}

inline double DE(const vec3 &p)
{
  double c1 = fabs(mandelBox_params.scale - 1.0);
  double c2 = pow(fabs(mandelBox_params.scale), 1 - mandelBox_params.num_iter);
  double d = MandelBoxDE(p, mandelBox_params, c1, c2);
  return d;
}

inline void normal(const vec3 & p, vec3 & normal)
{
  // compute the normal at p
  const double sqrt_mach_eps = 1.4901e-08;
  double eps = MAX( MAGNITUDE(p), 1.0 ) * sqrt_mach_eps;
  vec3 t1, e1;  
  double x;

  VEC(e1, eps, 0, 0);
  ADD_POINT(t1, p, e1);
  x = DE(t1);
  SUBTRACT_POINT(t1, p, e1);
  normal.x = DE(t1) - x;
  
  VEC(e1, 0, eps, 0);
  ADD_POINT(t1, p, e1);
  x = DE(t1);
  SUBTRACT_POINT(t1, p, e1);
  normal.y = DE(t1) - x;

  VEC(e1, 0, 0, eps);
  ADD_POINT(t1, p, e1);
  // x = DE(t1);
  SUBTRACT_POINT(t1, p, e1);
  // normal.z = DE(t1) - x;
  
  // calculating either of the last two x1,x2 causes compiler warning:
  // 'No device symbol for address reference'
  // This is dependent on order in source file only.

  NORMALIZE(normal);
}

#pragma acc declare copyin(mandelBox_params)
#pragma acc routine seq
void rayMarch(const RenderParams &render_params, const vec3 &from, const vec3 &direction, double eps,
        pixelData& pix_data, vec3& tests)
{

  double dist = 0.0;
  double totalDist = 0.0;
  
  // We will adjust the minimum distance based on the current zoom
  double epsModified = 0.0;
  
  int steps = 0;
  vec3 p;
  do 
  {
    MULT_SCALAR(p, direction, totalDist);
    ADD_POINT(p, p, from);
    dist = DE(p);
    
    totalDist += 0.95 * dist;
    
    epsModified = totalDist;
    epsModified *= eps;
    steps++;
  }
  while (dist > epsModified && totalDist <= render_params.maxDistance && steps < render_params.maxRaySteps);
  tests.x = DE(from);
  tests.y = DE(direction);
  tests.z = totalDist;
  
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
  else {
    //we have the background colour
    pix_data.escaped = true;
  }
}

