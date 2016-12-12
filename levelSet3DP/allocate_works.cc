#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::allocate_works(){
	allocation_index.clear();
	int activePoint=activeDOF.size()/3;
	double works_per_PE=activePoint/(size_PE-2);
	//allocate work layer by layer, each processs should gain largely same amount of works
	/*|s-A|<|s+c-A|->s>b-c/2
	*/
	int allocated_points=0;
	int send_points=0;
	int allocated_PE=1;
	int ghost_bottom_row_points=0;
	int bottom_row_points=0;
	int top_row_points=0;
	int ghost_top_row_points=0;
	bool bottom_row_found=false;
	
	
	for(int i=0;i<DOFperDim[2];i++){
		if(points_per_level[i]>0) {
			if(!bottom_row_found){
				if(i>0 and points_per_level[i-1]>0){
					ghost_bottom_row_points=points_per_level[i-1];
				}
				bottom_row_points=points_per_level[i];
			  bottom_row_found=true;
			}

			send_points+=points_per_level[i];
		}
		//if all points have been included
		if(allocated_points+send_points==activePoint){
			std::vector<double> activeDOF_PE;
			activeDOF_PE.insert(activeDOF_PE.begin(),&activeDOF[(allocated_points-ghost_bottom_row_points)*3],&activeDOF[(allocated_points+send_points+ghost_top_row_points)*3]);			activeDOF_PE.push_back(ghost_bottom_row_points);
			activeDOF_PE.push_back(bottom_row_points);
			activeDOF_PE.push_back(top_row_points);
			activeDOF_PE.push_back(ghost_top_row_points);
			if(!(allocated_PE==size_PE-2)){exit(-1);}
			MPI_Send(&activeDOF_PE[0], activeDOF_PE.size(), MPI_DOUBLE, allocated_PE ,finiteD, MPI_COMM_WORLD);
			allocated_points+=send_points;
			allocation_index.push_back(send_points);
		}	
		else if(double(send_points)>=works_per_PE-points_per_level[i+1]/2 and allocated_PE<(size_PE-2)){
			if(points_per_level[i+1]>0) {
				top_row_points=points_per_level[i];
				ghost_top_row_points=points_per_level[i+1];
			}
			
			std::vector<double> activeDOF_PE;
			activeDOF_PE.insert(activeDOF_PE.begin(),&activeDOF[(allocated_points-ghost_bottom_row_points)*3],&activeDOF[(allocated_points+send_points+ghost_top_row_points)*3]);
			activeDOF_PE.push_back(ghost_bottom_row_points);
			activeDOF_PE.push_back(bottom_row_points);
			activeDOF_PE.push_back(top_row_points);
			activeDOF_PE.push_back(ghost_top_row_points);
			MPI_Send(&activeDOF_PE[0], activeDOF_PE.size(), MPI_DOUBLE, allocated_PE ,finiteD, MPI_COMM_WORLD);
			
			//update and reset 
			allocation_index.push_back(send_points);
			allocated_PE++;
			allocated_points+=send_points;
			bottom_row_found=false;
			
			activeDOF_PE.clear();
			send_points=0;
			ghost_bottom_row_points=0;
			bottom_row_points=0;
			top_row_points=0;
			ghost_top_row_points=0;
			

			
		}
		//all points are allocated
		if(allocated_points==activePoint)	break;
	}

	if(!(allocated_points==activePoint)) {std::cout<<"miss point"<<allocated_points<<std::endl; exit(-1);}
	
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;