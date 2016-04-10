inline double MandelBulbDE(const vec3 &p0, const MandelBoxParams &params)
{
  vec3 z;
  z = p0;
  
  double dr = 1.0;
  double r = 0.0;

  double Bailout = params.rMin;
  double Power = params.rFixed;

  for (int i = 0; i < params.num_iter; i++) 
	{
	  r = MAGNITUDE(z);
	  if (r > Bailout) {
	  	break; 
	  }

	  double theta = acos(z.z/r);
	  double phi   = atan2(z.y, z.x);
	  dr = pow(r, Power - 1.0) * Power * dr + 1.0;

	  double zr = pow(r, Power);
	  theta     = theta * Power;
	  phi       = phi * Power;

	  z.x = zr*sin(theta)*cos(phi);
	  z.y = zr*sin(phi)*sin(theta);
	  z.z = zr*cos(theta);

	  z.x = z.x + p0.x;
	  z.y = z.y + p0.y;
	  z.z = z.z + p0.z;
	}
  return 0.5*log(r)*r/dr;
}
