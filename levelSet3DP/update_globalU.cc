#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::update_globalU(){
	int dof=0;
	for(unsigned int pe=1;pe<size_PE-1;pe++){
		double tem_U[allocation_index[pe-1]];
		MPI_Recv(&tem_U, allocation_index[pe-1] , MPI_DOUBLE, pe, backData, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		//update last_U
		for(unsigned i=0;i<allocation_index[pe-1];i++){

			if(tem_U[i]>gmma) last_U[activeDOF[(i+dof)*3]]=gmma;
			else if(tem_U[i]<-gmma) last_U[activeDOF[(i+dof)*3]]=-gmma;
			else last_U[activeDOF[(i+dof)*3]]=tem_U[i];
		}
		dof=dof+allocation_index[pe-1];
	}
	U.insert(U.end(),last_U.begin(),last_U.end());
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;