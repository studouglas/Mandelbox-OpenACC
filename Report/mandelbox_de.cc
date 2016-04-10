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
      dfactor *= rFixed2rMin2;
    }
    else if (r2 < rFixed2) 
    {
      const double t = (rFixed2/r2);
      MULT_SCALAR(p, p, (rFixed2/r2));
      dfactor *= t;
    }
    
    dfactor = dfactor * fabs(params.scale) + 1.0;      
    MULT_SCALAR(p, p, params.scale);
    ADD_POINT(p, p, p0);
    i++;
  }
  
  return  (MAGNITUDE(p) - c1) / dfactor - c2;
}
