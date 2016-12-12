#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
void finiteD_levelSet<dim>::output (unsigned int _cycle){
	//fill output vector
  for (unsigned int i=0; i<U.size(); i++){
			phi->values[i]=U[i];
  }
	std::vector<solutionClass<dim>* > outputVariables;
	outputVariables.push_back(phi);
  char fileName[200];
	std::sprintf (fileName, "../../output/output");
  //std::sprintf (fileName, "output0");
	outputData.writeMesh(fileName,  _cycle,outputVariables);
  
}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;