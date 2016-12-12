#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
void finiteD_levelSet<dim>::apply_initial_values(){
	for(unsigned int i=0;i<DOFperDim[2];i++){
		for(unsigned int j=0;j<DOFperDim[1];j++){
			for(unsigned int k=0;k<DOFperDim[0];k++){
				int id;
				std::vector<int> grid_id(dim);
				grid_id[0]=k;
				grid_id[1]=j;
				grid_id[2]=i;
				get_id(grid_id, id);
    		std::vector<double> coords;
				coords=mesh.controlPoint[id];
				double tem_phi;
				
				if(coords[2]>5) tem_phi=std::sqrt((coords[0]-3)*(coords[0]-3)+(coords[1]-5)*(coords[1]-5)+(coords[2]-7)*(coords[2]-7))-1;
				else tem_phi=std::sqrt((coords[0]-7)*(coords[0]-7)+(coords[1]-5)*(coords[1]-5)+(coords[2]-3)*(coords[2]-3))-1;
    		if(tem_phi>gmma){
					U[id]=gmma;
    		}
				else if (tem_phi<-gmma){
					U[id]=-gmma;
				}
				else{
					U[id]=tem_phi;
				}
  		}
		}
	}
}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;