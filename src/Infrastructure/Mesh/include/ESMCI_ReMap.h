#ifndef ESMCI_ReMap
#define ESMCI_ReMap


namespace ESMCI {
  

  // 
  // Singleton class anything derived 
  // from this will be a Singleton.
  //
  
  template<class Derived>
  class AbstractSingleton{
    
    static Derived* pInstance_;
    
    AbstractSingleton(const AbstractSingleton &in){};
    
  protected:
    
    AbstractSingleton(): IsSet(false) {};
    bool IsSet;
    
  public:
    
    static Derived& Instance(){
      if(!pInstance_){
	static Derived obj;
	pInstance_ = &obj;
	return obj;
      }else{
	return *pInstance_;
      }
    }; 
  };
  
  //template<class Derived>
  //Derived* AbstractSingleton<Derived>::pInstance_ = 0;
  

  class ReMap : public AbstractSingleton<ReMap> {

  public:

    typedef enum {IDENTITY=0,TORUS, CUBED_SPHERE, CYLINDER} ReMapType;
   

  private:

    ReMapType rt_;
    double Lx_,Ly_,r_,R_;
    const static double pi_   =  3.141592653589793238462643383279;

  public:

    // Set up the state of the unique object ...
    void SetReMapType(ReMapType& rt){rt_ = rt; AbstractSingleton<ReMap>::IsSet=true;}
    void Set2DScaling(double Lx, double Ly){Lx_ = Lx;Ly_ = Ly;}
    void SetTorus_r(double r){r_ = r;}
    void SetTorus_R(double R){R_ = R;}

    // Use it.
    void ConvertFrom(double &x, double &y, double &z);
    void ConvertTo(  double &u, double &v, double &w);

  private:

    void ConvertFromTorus(double &x, double &y, double &z);
    void ConvertFromCubedSphere(double &x, double &y, double &z);
    void ConvertFromCylinder(double &x, double &y, double &z);

    void ConvertToTorus(double &u, double &v, double &w);
    void ConvertToCubedSphere(double &x, double &y, double &z);
    void ConvertToCylinder(double &x, double &y, double &z);
    
  };
  
};
#endif
