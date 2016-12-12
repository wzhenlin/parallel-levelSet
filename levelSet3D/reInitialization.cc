#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
void finiteD_levelSet<dim>::reInitialization (){
	int interation=0;
	while(interation<reIniInteration){
		for (unsigned int activelID=0; activelID<activeDOF[0].size();activelID++){
			int my_id;
			//Godunov’s scheme
			std::vector<int> myGrid_id(3);
			double my_phi=activeUn[activelID];
			my_id=activeDOF[0][activelID];
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
			else dmx=(my_phi-activeUn[neibour_local_id] )/dx[0];
		
			neibourGrid_id[0]=myGrid_id[0]+1;
			neibourGrid_id[1]=myGrid_id[1];
			neibourGrid_id[2]=myGrid_id[2];
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0)dpx=0;
			else dpx=(activeUn[neibour_local_id]-my_phi)/dx[0];
			//Godunov Flux x direction
			dDx=std::max(dmx,0.0)*std::max(dmx,0.0)+std::min(dpx,0.0)*std::min(dpx,0.0);
			//
		
			neibourGrid_id[0]=myGrid_id[0];
			neibourGrid_id[1]=myGrid_id[1]-1;
			neibourGrid_id[2]=myGrid_id[2];
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0) dmy=0;
			else dmy=(my_phi-activeUn[neibour_local_id] )/dx[1];
		
			neibourGrid_id[0]=myGrid_id[0];
			neibourGrid_id[1]=myGrid_id[1]+1;
			neibourGrid_id[2]=myGrid_id[2];
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0)dpy=0;
			else dpy=(activeUn[neibour_local_id]-my_phi)/dx[1];
			//Godunov Flux y direction
			dDy=std::max(dmy,0.0)*std::max(dmy,0.0)+std::min(dpy,0.0)*std::min(dpy,0.0);
			
			
			neibourGrid_id[0]=myGrid_id[0];
			neibourGrid_id[1]=myGrid_id[1];
			neibourGrid_id[2]=myGrid_id[2]-1;
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0) dmy=0;
			else dmz=(my_phi-activeUn[neibour_local_id] )/dx[1];
		
			neibourGrid_id[0]=myGrid_id[0];
			neibourGrid_id[1]=myGrid_id[1];
			neibourGrid_id[2]=myGrid_id[2]+1;
			get_id(neibourGrid_id, neibour_id);
			neibour_local_id=global_local_index[neibour_id]-1;
			if(neibour_local_id<0)dpy=0;
			else dpz=(activeUn[neibour_local_id]-my_phi)/dx[1];
			//Godunov Flux y direction
			dDz=std::max(dmz,0.0)*std::max(dmz,0.0)+std::min(dpz,0.0)*std::min(dpz,0.0);
			
			double grad_phi;
			grad_phi=std::sqrt(dDx+dDy+dDz);
			
			double S;
			S=getS(my_phi,grad_phi);
			
			//update d;
			activeU[activelID]=my_phi-S*(grad_phi-1)*dtau;
		}	
		interation++;
		activeUn=activeU;
	}
}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;