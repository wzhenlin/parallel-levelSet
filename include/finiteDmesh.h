/*
*derived from IGAbase
*/


#ifndef finiteDmesh_h
#define finiteDmesh_h

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
using namespace std;

enum objectType {rectangular};

template <int dim>
class finiteDmesh
{
	public:
		finiteDmesh();
		~finiteDmesh();
		void generateControlPoint(objectType _object, std::vector<double>point0, std::vector<double>point1, std::vector<int> _GridSize);
		std::vector<std::vector<double> > controlPoint;
		std::vector<int> GridSize;
		std::vector<double> dx;
		
		
};

#endif