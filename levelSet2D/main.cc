#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <vector>

#include "finiteD_levelSet.h"

//Constant
#define DIMS 2


using namespace std;

int main(int argc, char *argv[]){
  std::clock_t start=std::clock();

  //set material parameters genric 
  parametersClass params;

	
	//model parameters
  params.setString("order", "Linear");
  params.setInt("knots",10);
	
	//output path default:current folder
	params.setString("output_path", "output0");
	params.setInt("knotsName",10);
	
	
  finiteD_levelSet<DIMS> problem(params);
  problem.run();
	
  //Stats
  printf ("\nTime taken:%10.2e sec\n", (std::clock()-start)/((double)CLOCKS_PER_SEC));
}

