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

#ifndef _3d_H
#define _3d_H

#define NEAR 1
#define FAR 100

#include "camera.h"
#include "renderer.h"


void   LoadIdentity   (float *matrix);
void   Perspective    (float fov, float aspect, float zNear, float zFar, float *projMatrix);
void   Frustum        (float left, float right, float bottom, float top, float znear, float zfar, float *matrix);
void   LookAt         (float *eye, float *target, float *up, float *modelMatrix);
float LengthVector   (float *vector);
void   NormalizeVector(float *vector);
void   ComputeNormalOfPlane(float *normal, float *v1, float *v2);
void   MultiplyMatrices(float *result, const float *matrix1, const float *matrix2);
void   MultiplyMatrixByVector(float *resultvector, const float *matrix, float *pvector);
int    InvertMatrix(float *m, float *out);
void   Translate(float *result, float x, float y, float z);
int    UnProject(float winX, float winY, float winZ, const float *model, 
                 const float *proj, const int *view, float *obj);

#pragma acc routine seq
int    UnProject(float winX, float winY, const CameraParams & camP, float *obj);

#endif
