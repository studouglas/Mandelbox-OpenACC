#ifndef vec3_h
#define vec3_h

#ifdef _OPENACC
	#include <accelmath.h>
#else
	#include <math.h>
#endif

typedef struct 
{
  float x, y, z;
}  vec3;

#define SET_POINT(p,v) { p.x=v[0]; p.y=v[1]; p.z=v[2]; }

#define SUBTRACT_POINT(p,v,u) {\
  p.x=(v.x)-(u.x);\
  p.y=(v.y)-(u.y);\
  p.z=(v.z)-(u.z);\
}
#define SUBTRACT_DARRS(p,d1,d2) {\
  p.x=(d1[0])-(d2[0]);\
  p.y=(d1[1])-(d2[1]);\
  p.z=(d1[2])-(d2[2]);\
}

#define ADD_POINT(p,v,u) {\
  p.x=(v.x)+(u.x);\
  p.y=(v.y)+(u.y);\
  p.z=(v.z)+(u.z);\
}

#define NORMALIZE(p) {\
    float fMag = ( p.x*p.x + p.y*p.y + p.z*p.z );\
    if (fMag != 0){\
		float fMult = 1.0/sqrtf(fMag);\
		p.x *= fMult;\
		p.y *= fMult;\
		p.z *= fMult;\
    }\
}

#define SUBTRACT_SCALAR(p,v,s) { \
  	p.x = v.x - s; \
  	p.y = v.y - s; \
  	p.z = v.z - s; \
}

#define ADD_SCALAR(p,v,s) {\
  	p.x = v.x + s; \
  	p.y = v.y + s; \
  	p.z = v.z + s; \
}

#define MAGNITUDE(p) ({ sqrtf( p.x*p.x + p.y*p.y + p.z*p.z ); })

#define DOT_ASSIGN(d,p) {  d=( p.x*p.x + p.y*p.y + p.z*p.z ); }
#define DOT(p,q) { (p.x*q.x + p.y*q.y + p.z*q.z) }
#define MAX(a,b) ( ((a)>(b)) ? (a) : (b))

#define VEC(v,a,b,c) { v.x = a; v.y = b; v.z = c; }

#define MULT_POINTWISE(p,v,u) { p.x = (v.x)*(u.x); p.y = (v.y)*(u.y); p.z = (v.z)*(u.z); }

#define MULT_SCALAR(p,v,s) {\
	p.x = v.x * s;\
	p.y = v.y * s;\
	p.z = v.z * s;\
}

#define DIV_SCALAR(p,v,s) {\
	float fInv = 1.0 / s;\
	p.x = v.x * fInv;\
	p.y = v.y * fInv;\
	p.z = v.z * fInv;\
}

inline float clamp(float d, float min, float max) 
{
  const float t = d < min ? min : d;
  return t > max ? max : t;
}

inline void clamp(vec3 &v, float min, float max) 
{
  v.x = clamp(v.x,min,max);
  v.y = clamp(v.y,min,max);
  v.z = clamp(v.z,min,max);
}

#endif
