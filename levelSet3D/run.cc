#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
void finiteD_levelSet<dim>::run (){
	//generate finite difference mesh
	std::vector<double> point0(dim), point1(dim);
	ControlPointperDim.resize(dim);
	for(unsigned int i=0;i<dim;i++) {
		point0[i]=0.0; point1[i]=10.0;
		ControlPointperDim[i]=51;
	}
	mesh.generateControlPoint(rectangular, point0, point1, ControlPointperDim);
	
	//generate solution class and initialize outPutData grid
	solutionClass<dim> _phi(mesh, NODAL, SCALAR, std::string("phi"));
	phi=&_phi;
	outputData.generateOutputGrid(&mesh);
	
	//
	dofPerControlPoint=1;
	reIniInteration=1;	
  setup();
  apply_initial_values();
	output(0);

	setNarrowBand();
	currentTime=0;
	totalTime=4*dt;
	int iteration=1;
	while(currentTime<totalTime)
	{
		finiteDifference();
		reInitialization ();
		update_globalU();
		setNarrowBand();
		output(iteration);
		iteration++;
		currentTime+=dt;

	}

}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;