#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::setNarrowBand(int number_amount){
	double local_U[number_amount];
	int num_layer=DOFperDim[2]/(size_PE-2);
	int dof_per_layer=DOFperDim[0]*DOFperDim[1];
	
	int dof_begin, size,layer_begin;

	dof_begin=(my_PE_num-1)*dof_per_layer*num_layer;
	//account for ghost cell
	if(my_PE_num>1)dof_begin=dof_begin- dof_per_layer;
	layer_begin=(my_PE_num-1)*num_layer;
	if(my_PE_num==size_PE-2) num_layer=DOFperDim[2]-num_layer*(size_PE-3);
	MPI_Recv(&local_U, number_amount, MPI_DOUBLE, 0, work_setNarrowBand, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
	points_per_level.clear();
	activeDOF.clear();
	bool activeFlag1=false;
	bool activeFlag2=false;
	int last_ActRow_number=0;
	int last_ActCol_number=0;//store col last activeDOF 
	int lastColNum=0;
	int LastColAccu;
	int currentColAccu;
	for(int i=0;i<num_layer;i++){
		int points=0;
		int layer=i+layer_begin;
		for(int j=0;j<DOFperDim[1];j++){
			for(int k=0;k<DOFperDim[0];k++){
				int my_id;
				activeFlag1=false;
				std::vector<int> grid_id(dim);
				grid_id[0]=k;
				grid_id[1]=j;
				grid_id[2]=layer;
				double my_phi;
				get_id(grid_id,my_id);
				my_phi=local_U[my_id-dof_begin];
				//N
				if(std::abs(my_phi)<gmma) {
					activeDOF.push_back(my_id);
					activeDOF.push_back(my_phi);
					activeDOF.push_back(2);
					points++;
				}
				else{
					for(int p=layer-1;p<=layer+1;p++){
						for(int q=j-1; q<=j+1;q++){
							for(int l=k-1; l<=k+1;l++){
								if(p<0 or p>=DOFperDim[2] or q<0 or q>=DOFperDim[1] or l<0 or l>=DOFperDim[0]) continue;

					 	 	 	int tem_id;
								double tem_phi;
								std::vector<int> tem_grid_id(dim);
								tem_grid_id[0]=l;
								tem_grid_id[1]=q;
								tem_grid_id[2]=p;
								get_id(tem_grid_id,tem_id);
								tem_phi=local_U[tem_id-dof_begin];
								if(std::abs(tem_phi)<gmma) activeFlag1=true;
							}
						}
					}	
					if(activeFlag1){
					 	activeDOF.push_back(my_id);
						activeDOF.push_back(my_phi);
						activeDOF.push_back(1);
						points++;
				 	}
			 	}
			}
  	}
		points_per_level.push_back(points);
	}
	activeDOF.insert(activeDOF.end(),points_per_level.begin(),points_per_level.end());
	MPI_Send(&activeDOF[0], activeDOF.size(), MPI_DOUBLE, 0 ,work_backNarrowBand, MPI_COMM_WORLD);
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;