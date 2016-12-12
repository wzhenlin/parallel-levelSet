/*
* Provide storage and projection for FEM results
*data type include scalar, vector and tensor
*/

#ifndef SOLUTIONCLASSES_H_
#define SOLUTIONCLASSES_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include "finiteDmesh.h"

using namespace std;

enum dataType {SCALAR, VECTOR, TENSOR};
enum dataLocation {NODAL, QUADRATURE};

template <int dim>
class solutionClass{
	private:
		finiteDmesh<dim>* mesh;
		
	public:
	 	/*
	 	*class constructor and destructor
		*resize values vector according to data type
	 	*/
		solutionClass(finiteDmesh<dim>& _mesh, dataLocation location, dataType data, std::string  _variableName);
		~solutionClass();
		

		/*
		*vector to store initial data value (at nodes or quadrature points)
		*/
		std::vector<double> values;
		
		/*
		*dataLocation: at NODAL or QUADRATURE
		*datatype:SCALAR, VECTOR or TENSOR
		*/
		dataLocation datalocation;
		dataType datatype;
		
		/*
		*operator  overloading
		*return values 
		*/
		double& operator() (unsigned int _a) {
			if ((datalocation==NODAL) && (datatype==SCALAR)) return values.at(_a);
			else {printf("solutionClass: incompatible arguments\n 1"); exit(-1);}
		}
		double& operator() (unsigned int _a, unsigned int _b) {
			if (datalocation==NODAL){
				if (datatype==VECTOR) return values.at(_a*dim+_b);
				else if (datatype==TENSOR) return values.at(_a*dim*dim+_b);
				else {printf("solutionClass: incompatible datatype\n"); exit(-1);}
			}
			else {printf("solutionClass: incompatible arguments\n 2"); exit(-1);}
		}
		
		/*
		*numQuadPoints=mesh->quadPtStencilSize 
		*numVariablesPerPoint=dim for vector;=dim*dim for tensor
		*/
		unsigned int numVariablesPerPoint;
		/*
		*solution name to be shown in vtk file
		*/
		std::string variableName;
		

};

#endif 