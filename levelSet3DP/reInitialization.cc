#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::reInitialization (){
	std::vector<double> result;
	int interation=0;
	while(interation<reIniInteration){
		//update ghost value
		if(ghost_bottom_row_points>0){
			double ghost_DOF [ghost_bottom_row_points];
			MPI_Recv(&ghost_DOF, ghost_bottom_row_points , MPI_DOUBLE, my_PE_num-1, up, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(&local_activeUn[ghost_bottom_row_points], bottom_row_points, MPI_DOUBLE, my_PE_num-1, down, MPI_COMM_WORLD);
			for(unsigned i=0;i<ghost_bottom_row_points;i++) local_activeUn[i]=ghost_DOF[i];
		}
		if(ghost_top_row_points>0){
			MPI_Send(&local_activeUn[ghost_bottom_row_points+local_num_active_points-top_row_points], top_row_points, MPI_DOUBLE, my_PE_num+1, up, MPI_COMM_WORLD);
			double ghost_DOF [ghost_top_row_points];
			MPI_Recv(&ghost_DOF, ghost_top_row_points, MPI_DOUBLE, my_PE_num+1, down, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			int tem=local_activeUn.size()-ghost_top_row_points;
			for(unsigned i=0;i<ghost_top_row_points;i++) local_activeUn[i+tem]=ghost_DOF[i];
		}
		int activelID;
		for (unsigned int i=0; i<local_num_active_points;i++){
			activelID=i+ghost_bottom_row_points;
			int my_id;
			//Godunov’s scheme
			std::vector<int> myGrid_id(3);
			double my_phi=local_activeUn[activelID];
			my_id=local_activeDOF[0][activelID];
			getGrid_id(my_id,myGrid_id);
		
			std::vector<int> neibourGrid_id(dim);
			int neibour_id, neibour_local_id;
			double dmx, dpx,dmy,dpy,dmz,dpz, dDx,dDy, dDz;
		
			neibourGrid_id[0]=myGrid_id[0]-1;
			neibourGrid_id[1]=myGrid_id[1];
			neibourGrid_id[2]=myGrid_id[2];
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0)dmx=0;
			else dmx=(my_phi-local_activeUn[neibour_local_id] )/dx[0];
		
			neibourGrid_id[0]=myGrid_id[0]+1;
			neibourGrid_id[1]=myGrid_id[1];
			neibourGrid_id[2]=myGrid_id[2];
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0)dpx=0;
			else dpx=(local_activeUn[neibour_local_id]-my_phi)/dx[0];
			//Godunov Flux x direction
			dDx=std::max(dmx,0.0)*std::max(dmx,0.0)+std::min(dpx,0.0)*std::min(dpx,0.0);
			//
		
			neibourGrid_id[0]=myGrid_id[0];
			neibourGrid_id[1]=myGrid_id[1]-1;
			neibourGrid_id[2]=myGrid_id[2];
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0) dmy=0;
			else dmy=(my_phi-local_activeUn[neibour_local_id] )/dx[1];
		
			neibourGrid_id[0]=myGrid_id[0];
			neibourGrid_id[1]=myGrid_id[1]+1;
			neibourGrid_id[2]=myGrid_id[2];
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0)dpy=0;
			else dpy=(local_activeUn[neibour_local_id]-my_phi)/dx[1];
			//Godunov Flux y direction
			dDy=std::max(dmy,0.0)*std::max(dmy,0.0)+std::min(dpy,0.0)*std::min(dpy,0.0);
			
			
			neibourGrid_id[0]=myGrid_id[0];
			neibourGrid_id[1]=myGrid_id[1];
			neibourGrid_id[2]=myGrid_id[2]-1;
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0) dmy=0;
			else dmz=(my_phi-local_activeUn[neibour_local_id] )/dx[1];
		
			neibourGrid_id[0]=myGrid_id[0];
			neibourGrid_id[1]=myGrid_id[1];
			neibourGrid_id[2]=myGrid_id[2]+1;
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0)dpy=0;
			else dpz=(local_activeUn[neibour_local_id]-my_phi)/dx[1];
			//Godunov Flux y direction
			dDz=std::max(dmz,0.0)*std::max(dmz,0.0)+std::min(dpz,0.0)*std::min(dpz,0.0);
			
			double grad_phi;
			grad_phi=std::sqrt(dDx+dDy+dDz);
			
			double S;
			S=getS(my_phi,grad_phi);
			
			//update d;
			local_activeU[activelID]=my_phi-S*(grad_phi-1)*dtau;
		}	
		interation++;
		local_activeUn=local_activeU;
	}
	
	std::vector<double> results;
	results.insert(results.begin(), &local_activeU[ghost_bottom_row_points], &local_activeU[ghost_bottom_row_points+local_num_active_points]);
	MPI_Send(&results[0], local_num_active_points, MPI_DOUBLE, 0, backData, MPI_COMM_WORLD);
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;