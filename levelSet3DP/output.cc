#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::output (int number_amount){
	std::vector<double> U_output(number_amount);
	MPI_Recv(&U_output[0], number_amount, MPI_DOUBLE, 0, tag_output, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	//begin write resutls;
	char fileName[200];
	std::sprintf (fileName, "../output/output");
	int tem_output_step;
	int num_output=number_amount/totalDOF;
	int mod=number_amount-num_output*totalDOF;
	if(mod==1) tem_output_step=U_output.back();
	for (unsigned int num=0;num<num_output;num++){
		for (unsigned int i=0; i<totalDOF; i++){
			phi->values[i]=U_output[i+totalDOF*num];
 		}
		std::vector<solutionClass<dim>* > outputVariables;
		outputVariables.push_back(phi);
		if(mod==0) {
			outputData.writeMesh(fileName,  output_step,outputVariables);
		 	output_step++;
		}
		else {
			outputData.writeMesh(fileName,  tem_output_step,outputVariables);
			tem_output_step++;
		}
	}
	MPI_Send(&my_PE_num, 1, MPI_INT, 0, finishWriting, MPI_COMM_WORLD);
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;