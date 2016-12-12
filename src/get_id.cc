#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
void finiteD_levelSet<dim>::get_id(std::vector<int> grid_id, int& controlPoint_id){
	if(dim==2){
		controlPoint_id=grid_id[1]*DOFperDim[0]+grid_id[0];
	}
	else if(dim==3){
		controlPoint_id=grid_id[2]*(DOFperDim[0]*DOFperDim[1])+grid_id[1]*DOFperDim[0]+grid_id[0];
	}
	else {std::cout<<"error in dim"<<std::endl;}

}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;