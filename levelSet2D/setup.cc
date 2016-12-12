#include "finiteD_levelSet.h"

using namespace std;

template<int dim>
void finiteD_levelSet<dim>::setup (){	
  //initialize global data structures

	DOFperDim.resize(dim);
	dx.resize(dim);
	totalDOF=mesh.controlPoint.size()*dofPerControlPoint;
	std::cout<<"number of control point is "<<mesh.controlPoint.size()<<"; totalDOF is "<<totalDOF<<std::endl;
	for(unsigned int i=0; i<dim;i++) {
		DOFperDim[i]=ControlPointperDim[i]*dofPerControlPoint;
	}
	dr=0;
	
	for(unsigned int i=0;i<dim;i++){
		dx[i]=mesh.dx[i];
		dr=dr+dx[i]*dx[i];
	}
	
	dr=std::sqrt(dr);
	beta=2*dr;
	gmma=2*dr;
	outBand=100;
	innerBand=-100;	
	vn=1.0;
	//dt*vn<dx
	dt=0.5*dx[0];
	dtau=0.5*dt;
	U.resize(totalDOF);  
}
template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;