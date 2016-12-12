/*
*finiteD_dislocation constructor and destructor
*/
#include "finiteD_levelSet.h"

using namespace std;
template <int dim>
finiteD_levelSet<dim>::finiteD_levelSet (parametersClass& _params):mesh(),outputData(), params(_params)
{

}

template <int dim>
finiteD_levelSet<dim>::~finiteD_levelSet(){}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;
