#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
void finiteD_levelSet<dim>::finiteDifference (){
	global_local_index.clear();
	for (unsigned int activelID=0; activelID<activeDOF[0].size();activelID++){
		global_local_index[activeDOF[0][activelID]]=activelID+1;//0 is saved for outside U
	}
	for (unsigned int activelID=0; activelID<activeDOF[0].size();activelID++){
		if(activeDOF[1][activelID]==1) {continue;} //skip dof belong to N
		std::vector<int> myGrid_id(dim);
		int my_id;
		double C;
		double my_phi=activeUn[activelID];
		my_id=activeDOF[0][activelID];
		getGrid_id(my_id,myGrid_id);
		C=getC(my_phi);
		
		//fixed boundary condition
		std::vector<int> neibourGrid_id(dim);
		int neibour_id, neibour_local_id;
		double dmx, dpx,dmy,dpy,fluxx,fluxy;
		
		neibourGrid_id[0]=myGrid_id[0]-1;
		neibourGrid_id[1]=myGrid_id[1];
		get_id(neibourGrid_id, neibour_id);
		neibour_local_id=global_local_index[neibour_id]-1;
		if(neibour_local_id<0)dmx=0;
		else dmx=(my_phi-activeUn[neibour_local_id] )/dx[0];
		
		neibourGrid_id[0]=myGrid_id[0]+1;
		neibourGrid_id[1]=myGrid_id[1];
		get_id(neibourGrid_id, neibour_id);
		neibour_local_id=global_local_index[neibour_id]-1;
		if(neibour_local_id<0)dpx=0;
		else dpx=(activeUn[neibour_local_id]-my_phi)/dx[0];
		//Godunov Flux x direction
		fluxx=std::max(dmx,0.0)*std::max(dmx,0.0)+std::min(dpx,0.0)*std::min(dpx,0.0);
		//fluxx=dmx;
		//
		
		neibourGrid_id[0]=myGrid_id[0];
		neibourGrid_id[1]=myGrid_id[1]-1;
		get_id(neibourGrid_id, neibour_id);
		neibour_local_id=global_local_index[neibour_id]-1;
		if(neibour_local_id<0) dmy=0;
		else dmy=(my_phi-activeUn[neibour_local_id] )/dx[1];
		
		neibourGrid_id[0]=myGrid_id[0];
		neibourGrid_id[1]=myGrid_id[1]+1;
		get_id(neibourGrid_id, neibour_id);
		neibour_local_id=global_local_index[neibour_id]-1;
		if(neibour_local_id<0)dpy=0;
		else dpy=(activeUn[neibour_local_id]-my_phi)/dx[1];
		//Godunov Flux y direction
		fluxy=std::max(dmy,0.0)*std::max(dmy,0.0)+std::min(dpy,0.0)*std::min(dpy,0.0);
		//fluxy=dmy;
		activeU[activelID]=my_phi-vn*std::sqrt(fluxx+fluxy)*dt;
		//activeU[activelID]=my_phi-vn*1*0.1;
	}
		activeUn=activeU;
}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;