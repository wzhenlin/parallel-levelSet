#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
double finiteD_levelSet<dim>::getS(double d, double grad_d){
	double S;
	S=d/std::sqrt(d*d+grad_d*grad_d*dr*dr);
	return S;
}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;