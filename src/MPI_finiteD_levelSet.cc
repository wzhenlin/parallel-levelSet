/*
*finiteD_dislocation constructor and destructor
*/
#include "MPI_finiteD_levelSet.h"

using namespace std;
template <int dim>
MPI_finiteD_levelSet<dim>::MPI_finiteD_levelSet (parametersClass& _params):mesh(),outputData(), params(_params)
{
	MPI_Comm_rank(MPI_COMM_WORLD, &my_PE_num);
	MPI_Comm_size(MPI_COMM_WORLD, &size_PE);
}

template <int dim>
MPI_finiteD_levelSet<dim>::~MPI_finiteD_levelSet(){}


template <int dim>
void MPI_finiteD_levelSet<dim>::get_id(std::vector<int> grid_id, int& controlPoint_id){
	if(dim==2){
		controlPoint_id=grid_id[1]*DOFperDim[0]+grid_id[0];
	}
	else if(dim==3){
		controlPoint_id=grid_id[2]*(DOFperDim[0]*DOFperDim[1])+grid_id[1]*DOFperDim[0]+grid_id[0];
	}
	else {std::cout<<"error in dim"<<std::endl;}

}

using namespace std;

template <int dim>
double MPI_finiteD_levelSet<dim>::getC (double phi){
	double C;
	double absPhi=std::abs(phi);
	if(absPhi<beta) C=1;
	else if(absPhi<gmma) C=(absPhi-gmma*gmma)*(2*absPhi+gmma-3*beta)/std::pow(gmma-beta,3);
	else C=0;
	return C;

}

template <int dim>
void MPI_finiteD_levelSet<dim>::getGrid_id(int controlPoint_id, std::vector<int>& grid_id){
	if(dim==2){
		grid_id[1]=controlPoint_id/DOFperDim[0];
		grid_id[0]=controlPoint_id-grid_id[1]*DOFperDim[0];
	}
	else if(dim==3){
		int numX1X2=DOFperDim[0]*DOFperDim[1];
		grid_id[2]=controlPoint_id/numX1X2;
		grid_id[1]=(controlPoint_id-grid_id[2]*numX1X2)/DOFperDim[0];
		grid_id[0]=controlPoint_id-grid_id[2]*numX1X2-grid_id[1]*DOFperDim[0];
	}
	else {std::cout<<"error in dim"<<std::endl;}

}

using namespace std;

template <int dim>
double MPI_finiteD_levelSet<dim>::getS(double d, double grad_d){
	double S;
	S=d/std::sqrt(d*d+grad_d*grad_d*dr*dr);
	return S;
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;
