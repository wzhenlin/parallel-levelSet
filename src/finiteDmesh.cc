/*
*IGA_dislocation constructor and destructor
*/
#include "finiteDmesh.h"

using namespace std;
template <int dim>
finiteDmesh<dim>::finiteDmesh (){
	dx.resize(3);
}

template <int dim>
void finiteDmesh<dim>::generateControlPoint(objectType _object, std::vector<double>point0, std::vector<double>point1, std::vector<int> _GridSize)
{
	if(_object==rectangular) printf("%dD rectangular ",dim);
	else {printf("unknown objectType\n"); exit(-1);}
	int totalPoint=1;
	GridSize=_GridSize;
	for(unsigned int i=0;i<dim;i++){
		totalPoint=totalPoint*GridSize[i];
		dx[i]=(point1[i]-point0[i])/(GridSize[i]-1);
	}
	controlPoint.resize(totalPoint);

	if(dim==2){
		int pointID=0;
		for(unsigned int i=0;i<GridSize[1];i++){
			for(unsigned int j=0;j<GridSize[0];j++){
				controlPoint[pointID].push_back(point0[0]+j*dx[0]);
				controlPoint[pointID].push_back(point0[1]+i*dx[1]);
				pointID++;
			}
		}
	}
	else if(dim==3){
		int pointID=0;
		for(unsigned int i=0;i<GridSize[2];i++){
			for(unsigned int j=0;j<GridSize[1];j++){
				for(unsigned int k=0;k<GridSize[0];k++){
				controlPoint[pointID].push_back(point0[0]+k*dx[0]);
				controlPoint[pointID].push_back(point0[1]+j*dx[1]);
				controlPoint[pointID].push_back(point0[2]+i*dx[2]);
				pointID++;
				}
			}
		}
	}
	printf("mesh generated\n");
}

template <int dim>
finiteDmesh<dim>::~finiteDmesh(){}

template class finiteDmesh<1>;
template class finiteDmesh<2>;
template class finiteDmesh<3>;