#include "../../include/supplementary/solutionClass.h"
/*
*class for solution storage
*data type include scalar, vector and tensor
*/
template <int dim>
solutionClass<dim>::solutionClass(finiteDmesh<dim>& _mesh, dataLocation _datalocation, dataType _datatype, std::string _variableName): mesh(&_mesh), datalocation(_datalocation), datatype(_datatype), variableName(_variableName){
	if (datalocation==NODAL){
		if (datatype==SCALAR) {values.resize(mesh->controlPoint.size(), 0.0); numVariablesPerPoint=1;}
		else if (datatype==VECTOR) {values.resize(mesh->controlPoint.size()*dim, 0.0); numVariablesPerPoint=dim;}
		else if (datatype==TENSOR) {values.resize(mesh->controlPoint.size()*dim*dim, 0.0); numVariablesPerPoint=dim*dim;}
		else {printf("unknown dataType\n"); exit(-1);}
	}
	else if (datalocation==QUADRATURE){
		printf("finite difference only, cannot interolate data at QUADRATURE\n"); exit(-1);
	}
	else {printf("unknown dataLocation\n"); exit(-1);}
}



template <int dim>
solutionClass<dim>::~solutionClass(){

}

template class solutionClass<1>;
template class solutionClass<2>;
template class solutionClass<3>;