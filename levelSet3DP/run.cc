#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::run (){
	//generate finite difference mesh
	std::vector<double> point0(dim), point1(dim);
	ControlPointperDim.resize(dim);
	for(unsigned int i=0;i<dim;i++) {
		point0[i]=0.0; point1[i]=10.0;
		ControlPointperDim[i]=51;
	}
	mesh.generateControlPoint(rectangular, point0, point1, ControlPointperDim);
	
	/*generate solution class and initialize outPutData grid 
	*this is for every processor since finally every process may do writing
	*/
	solutionClass<dim> _phi(mesh, NODAL, SCALAR, std::string("phi"));
	phi=&_phi;
	outputData.generateOutputGrid(&mesh);
	output_step=0;
	//
	dofPerControlPoint=1;
	reIniInteration=1;	
	output_step=0;
  setup();
	
	//below are only done by manager processor
	if(my_PE_num==0){
		output_step=0;
		
		apply_initial_values();
		send_output(size_PE-1);
		currentTime=0;
		totalTime=50*dt;
		int iteration=1;
		int tem;
		
		while(currentTime<totalTime)
		{
			manager_setNarrowBand();
			allocate_works();
			update_globalU();
			
			iteration++;
			currentTime+=dt;
			if(currentTime<totalTime){
				//still on calculation
				//check if process finish writing
				int flag;
				MPI_Iprobe(size_PE-1, finishWriting, MPI_COMM_WORLD, &flag, &status);
				if(flag==true) {
					//MPI_Recv(&tem, 1, MPI_INT, size_PE-1, finishWriting, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					send_output(size_PE-1);
				}
			}
			else{
				int tem_output_step=output_step;
				int flag;
				MPI_Iprobe(size_PE-1, finishWriting, MPI_COMM_WORLD, &flag, &status);
				if(flag==true) {
					int num=U.size()/totalDOF/(size_PE-1);
					
					for(unsigned int pe=1;pe<size_PE-1;pe++){
						send_output(pe,tem_output_step,num);
						tem_output_step+=num;
					}
					//for last PE output rest of results
					send_output(size_PE-1,tem_output_step, U.size()/totalDOF);
				}
				else{
					int num=U.size()/totalDOF/(size_PE-2);
					
					for(unsigned int pe=1;pe<size_PE-2;pe++){
						send_output(pe,tem_output_step,num);
						tem_output_step+=num;
					}
					//for last PE output rest of results
					send_output(size_PE-2,tem_output_step, U.size()/totalDOF);
				 }
			}
		}
		
		//send stop command
		for(unsigned i=1;i<size_PE;i++) MPI_Send(&tem, 0, MPI_INT,i ,stopWork, MPI_COMM_WORLD);
	}
	else {
		workers_run();
	}

}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;
