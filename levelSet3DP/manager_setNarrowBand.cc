#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::manager_setNarrowBand()
{	
	points_per_level.clear();
	activeDOF.clear();
	
	int num_layer=DOFperDim[2]/(size_PE-2);

	
	
	int dof_per_layer=DOFperDim[0]*DOFperDim[1];
	int dof_accu=0;
	for(unsigned int pe=1;pe<size_PE-1;pe++){
		int my_layer_num=num_layer;
		if(pe==size_PE-2) my_layer_num=DOFperDim[2]-num_layer*(size_PE-3);
		
		int size, bottom_ghost_dof, top_ghost_dof;
		bottom_ghost_dof=dof_per_layer;
		top_ghost_dof=dof_per_layer;
		
		if(pe==1) bottom_ghost_dof=0;
		if(pe==size_PE-2) top_ghost_dof=0;
		
		size=my_layer_num*dof_per_layer+bottom_ghost_dof+top_ghost_dof;
		
		MPI_Send(&last_U[dof_accu-bottom_ghost_dof], size, MPI_DOUBLE, pe ,work_setNarrowBand, MPI_COMM_WORLD);
		dof_accu+=my_layer_num*dof_per_layer;
	}
	if(!(dof_accu==totalDOF)){exit(-1);}
	
	for(unsigned int pe=1;pe<size_PE-1;pe++){

		int my_layer_num=num_layer;
		if(pe==size_PE-2) my_layer_num=DOFperDim[2]-num_layer*(size_PE-3);
		MPI_Probe(pe, work_backNarrowBand, MPI_COMM_WORLD, &status);
		int number_amount;
		MPI_Get_count(&status, MPI_DOUBLE, &number_amount);
		double tem_activeDOF[number_amount];
		MPI_Recv(&tem_activeDOF, number_amount, MPI_DOUBLE, pe, work_backNarrowBand, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		activeDOF.insert(activeDOF.end(),tem_activeDOF,tem_activeDOF+number_amount-my_layer_num);
		points_per_level.insert(points_per_level.end(),tem_activeDOF+number_amount-my_layer_num,tem_activeDOF+number_amount);
	}

}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;