#include "../include/vtkFunctions.h"

template <int dim>
void writeVTK(const char* vtkFileName, std::vector<int>& numPoints, std::vector<std::vector<double> >& coords, std::vector<solutionClass<dim>*>& outputVariables, std::vector<std::vector<double> >& values){
	std::cout<<"write VTK"<<std::endl;
	//opening inp file
	std::fstream vtkFile;
	vtkFile.open(vtkFileName, std::fstream::out | std::fstream::trunc);
	vtkFile << "<?xml version=\"1.0\"?> \n<VTKFile type=\"StructuredGrid\" version=\"0.1\">\n<StructuredGrid WholeExtent=\"";
	for (unsigned int i=0; i<3; i++){
		char output[30]; 
		if (i<dim) sprintf(output, "%u %u ", 0, numPoints[i]-1);
		else sprintf(output, "%u %u ", 0, 0);
		vtkFile << output;
	}
	vtkFile << "\">\n<Piece Extent=\"";
	for (unsigned int i=0; i<3; i++){
		char output[30]; 
		if (i<dim) sprintf(output, "%u %u ", 0, numPoints[i]-1);
		else sprintf(output, "%u %u ", 0, 0);
		vtkFile << output;
	}
	vtkFile << "\">\n";
	//write output variable type and names
	vtkFile << "<PointData>\n";
	/*
	if (dim>1){ //for some reason, vtk files in 1D don't seem to like data type specified in PointData
		for (typename std::vector<solutionClass<dim>* >::iterator it=outputVariables.begin(); it<outputVariables.end();  it++){
			if ((*it)->datatype==SCALAR || (dim==1)){ //if dim==1, all variables are scalars
				char output[200]; sprintf(output, "Scalars=\"%s\" ", (*it)->variableName.c_str());
				vtkFile << output;
			}
			else if ((*it)->datatype==VECTOR){
				char output[200]; sprintf(output, "Vectors=\"%s\" ", (*it)->variableName.c_str());
				vtkFile << output;
			}
			else if ((*it)->datatype==TENSOR){
				char output[200]; sprintf(output, "Tensors=\"%s\" ", (*it)->variableName.c_str());
				vtkFile << output;
			}
		}
	}
	vtkFile << ">\n";
	*/
	
	//loop over output variables
	unsigned int variableID=0;
	for (typename std::vector<solutionClass<dim>* >::iterator it=outputVariables.begin(); it<outputVariables.end();  it++){
		if ((*it)->datatype==SCALAR || (dim==1)){ //if dim==1, all variables are scalars
			char output[200]; sprintf(output, "<DataArray type=\"Float32\" Name=\"%s\" format=\"ascii\" NumberOfComponents=\"%u\">\n", (*it)->variableName.c_str(), 1);
			vtkFile << output;
			for (unsigned int i=0; i<values[variableID].size(); i++){
				char outputVal[30]; sprintf(outputVal, "%14.6e ", values[variableID][i]);  
				vtkFile << outputVal;
			}
			variableID++;
		}
		else if ((*it)->datatype==VECTOR){
			char output[200]; sprintf(output, "<DataArray type=\"Float32\" Name=\"%s\" format=\"ascii\" NumberOfComponents=\"%u\">\n", (*it)->variableName.c_str(), 3);
			vtkFile << output;
			for (unsigned int i=0; i<values[variableID].size(); i++){
				for (unsigned int j=0; j<3; j++){
					char outputVal[30]; 
					if (j<dim) sprintf(outputVal, "%14.6e ", values[variableID+j][i]);  
					else sprintf(outputVal, "%14.6e ", 0.0); 
					vtkFile << outputVal;
				}
			}
			variableID+=dim;
		}
		else if ((*it)->datatype==TENSOR){
			char output[200]; sprintf(output, "<DataArray type=\"Float32\" Name=\"%s\" format=\"ascii\" NumberOfComponents=\"%u\">\n", (*it)->variableName.c_str(), 9);
			vtkFile << output;
			for (unsigned int i=0; i<values[variableID].size(); i++){
				for (unsigned int j=0; j<3; j++){
					for (unsigned int k=0; k<3; k++){
						char outputVal[30]; 
						if ((j<dim) && (k<dim)) sprintf(outputVal, "%14.6e ", values[variableID+j*dim+k][i]); 
						else sprintf(outputVal, "%14.6e ", 0.0);
						vtkFile << outputVal;
					}
				}
			}
			variableID+=dim*dim;
		}
		char output[100]; sprintf(output, "\n</DataArray>\n");
		vtkFile << output;
	}
	vtkFile << "</PointData>\n";
		
	
	//Coords output
	char output2[200]; sprintf(output2, "<Points>\n<DataArray type=\"Float32\" NumberOfComponents=\"%u\"> \n", 3);
	vtkFile << output2;	
	for (unsigned int i=0; i<coords[0].size(); i++){
		for (unsigned int j=0; j<3; j++){
			char outputVal[20]; 
			if (j<dim) sprintf(outputVal, "%14.6e ", coords[j][i]);
			else sprintf(outputVal, "%14.6e ", 0.0);
			vtkFile << outputVal;
		}
	}
	char output3[200]; sprintf(output3, "\n</DataArray>\n</Points>\n</Piece>\n</StructuredGrid>\n</VTKFile>\n");
	vtkFile << output3;
	//close vtk file
	vtkFile.close();
}
template void writeVTK<1>(const char* vtkFileName, std::vector<int>& numPoints, std::vector<std::vector<double> >& coords, std::vector<solutionClass<1>*>& outputVariables, std::vector<std::vector<double> >& values);
template void writeVTK<2>(const char* vtkFileName, std::vector<int>& numPoints, std::vector<std::vector<double> >& coords, std::vector<solutionClass<2>*>& outputVariables, std::vector<std::vector<double> >& values);
template void writeVTK<3>(const char* vtkFileName, std::vector<int>& numPoints, std::vector<std::vector<double> >& coords, std::vector<solutionClass<3>*>& outputVariables, std::vector<std::vector<double> >& values);



template <int dim>
void writeMesh(const char* meshFileName, unsigned int _cycle, finiteDmesh<dim>* mesh, std::vector<solutionClass<dim>* >& outputVariables){
	std::vector<std::vector<double> > coords(dim); 
	unsigned int numVariables=0;
	for (typename std::vector<solutionClass<dim>* >::iterator it=outputVariables.begin(); it<outputVariables.end();  it++){
		numVariables+=(*it)->numVariablesPerPoint;
	}
	int numControlPoint=mesh->controlPoint.size();
	std::vector<std::vector<double> > values(numVariables); //solution variables
	
	std::vector<int> numPoints(dim, 1); 
	for (unsigned int a=0; a<dim; a++) {numPoints[a]=mesh->GridSize[a];}
	//for solution variables
	unsigned int variableID=0;
	unsigned int dofID;
	for (typename std::vector<solutionClass<dim>* >::iterator it=outputVariables.begin(); it<outputVariables.end();  it++){
		dofID=0;
		int dofPerPoint=(*it)->numVariablesPerPoint;
		for(unsigned int numPoint=0; numPoint<numControlPoint; numPoint++){
			for (unsigned int l=0; l<dofPerPoint; l++){
				values[variableID+l].push_back((*it)->values[dofID]);
				dofID++;
			}
			for(unsigned int a=0;a<dim;a++) coords[a].push_back(mesh->controlPoint[numPoint][a]);
		}

		variableID+=dofPerPoint;
	}
	std::string outputFile(meshFileName); 
	char output[100]; sprintf(output, "_%u", _cycle);
	outputFile +=output; 
	outputFile +=".vts";
	writeVTK<dim>(outputFile.c_str(), numPoints, coords, outputVariables, values);
	printf("mesh written to:%s\n", outputFile.c_str());
}

template void writeMesh<1>(const char* meshFileName, unsigned int _cycle, finiteDmesh<1>* mesh, std::vector<solutionClass<1>* >& outputVariables);
template void writeMesh<2>(const char* meshFileName, unsigned int _cycle, finiteDmesh<2>* mesh, std::vector<solutionClass<2>* >& outputVariables);
template void writeMesh<3>(const char* meshFileName, unsigned int _cycle, finiteDmesh<3>* mesh, std::vector<solutionClass<3>* >& outputVariables);
