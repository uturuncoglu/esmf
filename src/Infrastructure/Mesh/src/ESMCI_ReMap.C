#include <Mesh/include/ESMCI_ReMap.h>
#include <math.h>

namespace ESMCI {

  //template<class Derived>
  //Derived* AbstractSingleton<Derived>::pInstance_ = 0;
  template<>
  ReMap* AbstractSingleton<ReMap>::pInstance_ = 0;

  //ReMap* AbstractSingleton<ReMap>::pInstance_ = 0;

  void ReMap::ConvertFrom(double &x, double &y, double &z){
    if(AbstractSingleton<ReMap>::IsSet == true){
      if(rt_ == IDENTITY)return;
      switch(rt_){
      case TORUS:
	ConvertFromTorus(x,y,z);
	break;
      case CUBED_SPHERE:
	ConvertFromCubedSphere(x,y,z);
	  break;
      case CYLINDER:
	ConvertFromCylinder(x,y,z);
	break;
      };
    }
  };
  
  void ReMap::ConvertTo(double &u, double &v, double &w){

    
    if(AbstractSingleton<ReMap>::IsSet == true){
      if(rt_ == IDENTITY)return;
      switch(rt_){
      case TORUS:
	ConvertToTorus(u,v,w);
	break;
      case CUBED_SPHERE:
	ConvertToCubedSphere(u,v,w);
	break;
      case CYLINDER:
	ConvertToCylinder(u,v,w);
	break;
      };
    }
  };
  
  void ReMap::ConvertFromTorus(double &x, double &y, double &z){
    double u,v;
    
    // 3D coordinates lying on Torus transformed to u,v
    
    u = (0.5/pi_)*Lx_*atan((y-R_)/(x-R_));
    v = (0.5/pi_)*Ly_*asin(z/r_);
    
    z = 0.0;
    x = u;
    y = v;
    
  };

  void ReMap::ConvertFromCubedSphere(double &x, double &y, double &z){};
  void ReMap::ConvertFromCylinder(double &x, double &y, double &z){};
  
  void ReMap::ConvertToTorus(double &u, double &v, double &w){
    double x,y,z;
    // u,v --> x,y,z
    
    x = (R_ + r_*cos(v*2.0*pi_/Ly_))*cos(u*2.0*pi_/Lx_);
    y = (R_ + r_*cos(v*2.0*pi_/Ly_))*sin(u*2.0*pi_/Lx_);
    z = r_*sin(v*2.0*pi_/Ly_);
    u =x;v=y;w=z;
  };

  void ReMap::ConvertToCubedSphere(double &x, double &y, double &z){};
  void ReMap::ConvertToCylinder(double &x, double &y, double &z){};
  
};
