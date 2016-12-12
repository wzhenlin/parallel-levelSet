#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <vector>
#include "include/finiteDmesh.h"
#include "include/solutionClass.h"
#include "include/DataOut.h"

//Constant
#define dim 3


using namespace std;



//#include "levelSet_GPU_kernel.h"

#define THREADS_PER_BLOCK 512

__global__ void finiteD_gpu(double *U, double dt, double vn, int totalStep);
__global__ void update_phi(double *U, double dt, double vn, int step);
int main(int argc, char *argv[]){
  std::clock_t start=std::clock();
	int num_point=51;
	//define output for 101*101*101 dof with 20 timesteps
	std::vector<double> point0(dim), point1(dim);
	std::vector<int> ControlPointperDim;
	ControlPointperDim.resize(dim);
	for(unsigned int i=0;i<dim;i++) {
		point0[i]=0.0; point1[i]=10.0;
		ControlPointperDim[i]=num_point;
	}

	finiteDmesh<dim> mesh;
	mesh.generateControlPoint(rectangular, point0, point1, ControlPointperDim);
	//solution class
	solutionClass<dim> phi(mesh, NODAL, SCALAR, std::string("phi"));
	//outputData class
	DataOut<dim> outputData;
	outputData.generateOutputGrid(&mesh);
	
	
	
	std::cout<<"aa"<<std::endl;	
	int totalTimestep=10;
	int dof=num_point*num_point*num_point;
	double *U;
	double *d_U;
	
	//need one more to store initial value
	int size=dof*(totalTimestep+1)*sizeof(double);	
	cudaMalloc(&d_U, size);
	U=(double *)malloc(size);
	
	
	//setting for easy index
	dim3 threadsPerBlock(num_point,1,1);
	dim3 numBlcoks(num_point,num_point);
	
std::clock_t GPU_time=std::clock();
	finiteD_gpu<<<numBlcoks, threadsPerBlock>>>(d_U,0.05 ,1,totalTimestep);

        for(int i=1;i<=totalTimestep;i++){
                update_phi<<<numBlcoks, threadsPerBlock>>>(d_U,0.05, 1, i);
       
        }
cudaMemcpy(U, d_U, size, cudaMemcpyDeviceToHost);
	printf ("\nTime taken:%10.2e sec\n", (std::clock()-GPU_time)/((double)CLOCKS_PER_SEC));
	//output
	for(unsigned int s=0;s<=totalTimestep;s++){
  	for (unsigned int i=0; i<dof; i++){
			phi.values[i]=U[i+dof*s];
  	}
		std::vector<solutionClass<dim>* > outputVariables;
		outputVariables.push_back(&phi);
  	char fileName[200];
		std::sprintf (fileName, "../output/output");
  	//std::sprintf (fileName, "output0");
		outputData.writeMesh(fileName,  s,outputVariables);
	}
	
	
	
	free(U);
	cudaFree(d_U);
	
  //Stats
  printf ("\nTime taken:%10.2e sec\n", (std::clock()-start)/((double)CLOCKS_PER_SEC));
}

