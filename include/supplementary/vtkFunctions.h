/*
* Generate VTK file for results
*/

#ifndef VTKFUNCTIONS_H_
#define VTKFUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include "finiteDmesh.h"
#include "solutionClass.h"

using namespace std;

template <int dim>
/*
*write VTK file called inside of writeMesh
*/
void writeVTK(const char* vtkFileName, std::vector<int>& numPoints, std::vector<std::vector<double> >& coords, std::vector<solutionClass<dim>*>& outputVariables, std::vector<std::vector<double> >& values);

template <int dim>

/*
*write VTK file for user
*/
void writeMesh(const char* meshFileName, unsigned int _cycle, finiteDmesh<dim>* mesh, std::vector<solutionClass<dim>* >& outputVariables);

#endif