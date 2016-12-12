#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include<mpi.h>
#include <vector>

#include "MPI_finiteD_levelSet.h"

//Constant
#define DIMS 3


using namespace std;

int main(int argc, char *argv[]){
  double t_total;
  int my_PE_num;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_PE_num);
	if(my_PE_num==0) t_total=-MPI_Wtime();
  //set material parameters genric 
  parametersClass params;
	
	
  MPI_finiteD_levelSet<DIMS> problem(params);
  problem.run();
	
  //Stats
  //printf ("\nTime taken:%10.2e sec\n", (std::clock()-start)/((double)CLOCKS_PER_SEC));
	if(my_PE_num==0) printf ("\n total time is Time taken:%10.2e sec\n", MPI_Wtime()+t_total);
	MPI_Finalize();
}

