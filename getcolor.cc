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
#include "vector3d.h"

//---lightning and colouring---------
double CamLightW = 1.8;// 1.27536;
double CamLightMin = 0.3;// 0.48193;

#define CAM_LIGHT 1.0
#define BASE_COLOR 1.0
#define BACK_COLOR 0.4

inline void lighting(const vec3 &n, const vec3 &color, const vec3 &pos, const vec3 &direction,  vec3 &outV)
{
  vec3 nn;
  SUBTRACT_SCALAR(nn, n, 1.0);
  double d = DOT(direction, nn);
  double ambient = MAX(CamLightMin, d) * CamLightW;
  vec3 camLight;
  VEC(camLight, CAM_LIGHT, CAM_LIGHT, CAM_LIGHT);
  MULT_SCALAR(nn, camLight, ambient);
  MULT_POINTWISE(outV, color, nn);
}

#pragma acc declare copyin(CamLightW, CamLightMin)
#pragma acc routine seq
inline void getColour(vec3 &hitColor, const pixelData &pixData, const RenderParams &render_params, const vec3 &from, const vec3 &direction)
{
  VEC(hitColor, BASE_COLOR, BASE_COLOR, BASE_COLOR);

  if (pixData.escaped == false) 
  {
    //apply lighting
    lighting(pixData.normal, hitColor, pixData.hit, direction, hitColor);
    
    //add normal based colouring
    if (render_params.colourType == 0 || render_params.colourType == 1) {
      MULT_POINTWISE(hitColor, hitColor, pixData.normal);
      ADD_SCALAR(hitColor, hitColor, 1.0);
      DIV_SCALAR(hitColor, hitColor, 2.0);
      MULT_SCALAR(hitColor, hitColor, render_params.brightness);

      //gamma correction
      clamp(hitColor, 0.0, 1.0);
      MULT_POINTWISE(hitColor, hitColor, hitColor)
    }
    if (render_params.colourType == 1)
    {
     //"swap" colors
     double t = hitColor.x;
     hitColor.x = hitColor.z;
     hitColor.z = t;
    }
  }
  else 
  {
    //we have the background colour
  	VEC(hitColor, BACK_COLOR, BACK_COLOR, BACK_COLOR);
  }
}
