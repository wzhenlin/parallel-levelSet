#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <vector>

#include "finiteD_levelSet.h"

//Constant
#define DIMS 3


using namespace std;

int main(int argc, char *argv[]){
  std::clock_t start=std::clock();

  //set material parameters genric 
  parametersClass params;
	
	
  finiteD_levelSet<DIMS> problem(params);
  problem.run();
	
  //Stats
  printf ("\nTime taken:%10.2e sec\n", (std::clock()-start)/((double)CLOCKS_PER_SEC));
}

