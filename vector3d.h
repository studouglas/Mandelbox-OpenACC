#ifndef vec3_h
#define vec3_h

#ifdef _OPENACC
	#include <accelmath.h>
#else
	#include <math.h>
#endif

typedef struct 
{
  double x, y, z;
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

#define DISTANCE_APART(p,q) ({ sqrt(pow(p.x-q.x, 2) + pow(p.y-q.y, 2) + pow(p.z-q.z, 2)); })

#define NORMALIZE(p) {\
    double fMag = ( p.x*p.x + p.y*p.y + p.z*p.z );\
    if (fMag != 0){\
		double fMult = 1.0/sqrt(fMag);\
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

#define MAGNITUDE(p) ({ sqrt( p.x*p.x + p.y*p.y + p.z*p.z ); })

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
	double fInv = 1.0 / s;\
	p.x = v.x * fInv;\
	p.y = v.y * fInv;\
	p.z = v.z * fInv;\
}

inline double clamp(double d, double min, double max) 
{
  const double t = d < min ? min : d;
  return t > max ? max : t;
}

inline void clamp(vec3 &v, double min, double max) 
{
  v.x = clamp(v.x,min,max);
  v.y = clamp(v.y,min,max);
  v.z = clamp(v.z,min,max);
}

#endif
