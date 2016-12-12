#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::send_output(int PE, int sp_output_step, int output_size){
	int size=U.size();
	if(!sp_output_step){
		MPI_Send(&U[0], size, MPI_DOUBLE, PE ,tag_output, MPI_COMM_WORLD);
		output_step+=size/totalDOF;
		//clear U for next round storage
		U.clear();
	}
	else if(output_size>0){
		
		//output from special output step
		std::vector<double> U_PE;
		U_PE.insert(U_PE.begin(),U.begin(), U.begin()+totalDOF*output_size);
		U_PE.push_back(sp_output_step);
		MPI_Send(&U_PE[0], U_PE.size(), MPI_DOUBLE, PE ,tag_output, MPI_COMM_WORLD);
		U.erase(U.begin(),U.begin()+totalDOF*output_size);
	}
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;