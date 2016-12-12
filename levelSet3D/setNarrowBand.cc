#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
void finiteD_levelSet<dim>::setNarrowBand(){
	activeU.clear();
	activeUn.clear();
	//activeDOF.resize(2);
	//activeDOF[0].clear();
	//activeDOF[1].clear();
	activeDOF.clear();
	activeDOF.resize(2);
	bool activeFlag1=false;
	bool activeFlag2=false;
	int last_ActRow_number=0;
	int last_ActCol_number=0;//store col last activeDOF 
	int lastColNum=0;
	int LastColAccu;
	int currentColAccu;
	for(int i=0;i<DOFperDim[2];i++){
		for(int j=0;j<DOFperDim[1];j++){
			for(int k=0;k<DOFperDim[0];k++){
				int my_id;
				activeFlag1=false;
				std::vector<int> grid_id(dim);
				grid_id[0]=k;
				grid_id[1]=j;
				grid_id[2]=i;
				double my_phi;
				get_id(grid_id,my_id);
				my_phi=U[my_id];
				//N
				if(std::abs(my_phi)<gmma) {
					activeDOF[0].push_back(my_id);
					activeDOF[1].push_back(2);
					activeUn.push_back(my_phi);
				}
				else{
					for(int p=i-1;p<=i+1;p++){
						for(int q=j-1; q<=j+1;q++){
							for(int l=k-1; l<=k+1;l++){
								if(p<0 or p>=DOFperDim[1] or q<0 or q>=DOFperDim[0] or l<0 or l>=DOFperDim[2]) continue;

					 	 	 	int tem_id;
								double tem_phi;
								std::vector<int> tem_grid_id(dim);
								tem_grid_id[0]=l;
								tem_grid_id[1]=q;
								tem_grid_id[2]=p;
								get_id(tem_grid_id,tem_id);
								tem_phi=U[tem_id];
								if(std::abs(tem_phi)<gmma) activeFlag1=true;
							}
						}
					}	
					if(activeFlag1){
					 	activeDOF[0].push_back(my_id);activeDOF[1].push_back(1);
					 	activeUn.push_back(my_phi);
				 	}
			 	}
			}
  	}
	}
	activeU=activeUn;
	std::cout<<"activeDOF.size="<<activeDOF[0].size()<<std::endl;
}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;
