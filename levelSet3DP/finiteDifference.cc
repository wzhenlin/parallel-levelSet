#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::finiteDifference (int number_amount){
	//get data
	double packed_local_activeDOF [number_amount];
	MPI_Recv(&packed_local_activeDOF, number_amount, MPI_DOUBLE, 0, finiteD, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	//reconstruct data structure
	local_activeDOF.clear();
	local_activeUn.clear();
	local_activeU.clear();
	global_local_index.clear();
	
	local_activeDOF.resize(2);
	ghost_bottom_row_points=packed_local_activeDOF[number_amount-4];
	bottom_row_points=packed_local_activeDOF[number_amount-3];
	top_row_points=packed_local_activeDOF[number_amount-2];
	ghost_top_row_points=packed_local_activeDOF[number_amount-1];
		
	local_num_active_points=(number_amount-4)/3-ghost_bottom_row_points-ghost_top_row_points;
	
	//include ghost cell
	for (unsigned int activelID=0; activelID<(number_amount-4)/3;activelID++){
		int id=packed_local_activeDOF[activelID*3];
		local_activeUn.push_back(packed_local_activeDOF[activelID*3+1]);
		local_activeDOF[0].push_back(id);
		local_activeDOF[1].push_back(packed_local_activeDOF[activelID*3+2]);
		global_local_index[id]=activelID+1;//0 is saved for outside U
		
	}
	local_activeU=local_activeUn;
	
	//only update active points
	int activelID;
	int a=0;
	for (unsigned int i=0; i<1000;i++){
			for (unsigned int i=0; i<1000;i++){
					a=a*a;
			}
	}
	for (unsigned int i=0; i<local_num_active_points;i++){
		activelID=i+ghost_bottom_row_points;
		
		if(local_activeDOF[1][activelID]==1) {continue;} //skip dof belong to N
		std::vector<int> myGrid_id(dim);
		int my_id;
		double C;
		double my_phi=local_activeUn[activelID];
		my_id=local_activeDOF[0][activelID];
		getGrid_id(my_id,myGrid_id);
		C=getC(my_phi);
		
		//fixed boundary condition
		std::vector<int> neibourGrid_id(3);
		int neibour_id, neibour_local_id;
		double dmx, dpx,dmy,dpy,dmz,dpz,fluxx,fluxy,fluxz;
		
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
		fluxx=std::max(dmx,0.0)*std::max(dmx,0.0)+std::min(dpx,0.0)*std::min(dpx,0.0);
		//fluxx=dmx;
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
		fluxy=std::max(dmy,0.0)*std::max(dmy,0.0)+std::min(dpy,0.0)*std::min(dpy,0.0);
		//fluxy=dmy;
		
		neibourGrid_id[0]=myGrid_id[0];
		neibourGrid_id[1]=myGrid_id[1];
		neibourGrid_id[2]=myGrid_id[2]-1;
		get_id(neibourGrid_id, neibour_id);
		neibour_local_id=global_local_index[neibour_id]-1;
		if(neibour_local_id<0) dmy=0;
		else dmz=(my_phi-local_activeUn[neibour_local_id] )/dx[2];
		
		neibourGrid_id[0]=myGrid_id[0];
		neibourGrid_id[1]=myGrid_id[1];
		neibourGrid_id[2]=myGrid_id[2]+1;
		get_id(neibourGrid_id, neibour_id);
		neibour_local_id=global_local_index[neibour_id]-1;
		if(neibour_local_id<0)dpy=0;
		else dpz=(local_activeUn[neibour_local_id]-my_phi)/dx[2];
		//Godunov Flux y direction
		fluxz=std::max(dmz,0.0)*std::max(dmz,0.0)+std::min(dpz,0.0)*std::min(dpz,0.0);
		//fluxy=dmz;
		
		local_activeU[activelID]=my_phi-vn*std::sqrt(fluxx+fluxy+fluxz)*dt;
		//activeU[activelID]=my_phi-vn*1*0.1;
	}
		local_activeUn=local_activeU;
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;