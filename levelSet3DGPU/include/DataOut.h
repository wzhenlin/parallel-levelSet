/*
* Provide storage and projection for FEM results
*data type include scalar, vector and tensor
*/

#ifndef DATAOUT_H_
#define DATAOUT_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include "solutionClass.h"

using namespace std;

template <int dim>
class DataOut{
	private:
		
	public:
	 	/*
	 	*class constructor and destructor
		*resize values vector according to data type
	 	*/
		DataOut();
		~DataOut();
		void generateOutputGrid(finiteDmesh<dim>* mesh);
		
		void writeVTK(const char* vtkFileName, std::vector<solutionClass<dim>*>& outputVariables, std::vector<std::vector<double> >& values);
		void writeMesh(const char* meshFileName, unsigned int _cycle, std::vector<solutionClass<dim>* >& outputVariables);
		
		
		std::vector<std::vector<double> > coords; 
		std::vector<int> numPoints;
		int numControlPoint;
		

};



#endif 