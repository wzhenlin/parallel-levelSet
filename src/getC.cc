#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
double finiteD_levelSet<dim>::getC (double phi){
	double C;
	double absPhi=std::abs(phi);
	if(absPhi<beta) C=1;
	else if(absPhi<gmma) C=(absPhi-gmma*gmma)*(2*absPhi+gmma-3*beta)/std::pow(gmma-beta,3);
	else C=0;
	return C;

}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;