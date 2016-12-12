#ifndef finiteD_levelSet_h
#define finiteD_levelSet_h

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <map>
#include "finiteDmesh.h"
#include "supplementary/parameters.h"
#include "supplementary/solutionClass.h"
#include "supplementary/DataOut.h"
using namespace std;
template <int dim>
class finiteD_levelSet
{
public:

  finiteD_levelSet (parametersClass& _params);
  ~finiteD_levelSet();
	
  void run();
	void setup();
  void apply_initial_values();
	void getGrid_id(int controlPoint_id, std::vector<int>& grid_id);
	void get_id(std::vector<int> grid_id, int& controlPoint_id);
	void resetNarrowBand();
	void setNarrowBand();
	double getC(double phi);
	double getS(double d, double grad_d);
	void reInitialization();
	void update_globalU();
	void update_localU();
	
  void output (unsigned int _cycle);
	
  void finiteDifference();
	
  finiteDmesh<dim> mesh;
  parametersClass& params;
	
	std::vector<double> U;
	std::vector<double> activeU, activeUn;
	
	int totalDOF,dofPerControlPoint;
	std::vector<int> ControlPointperDim, DOFperDim;
	
	std::vector<std::vector<int> > activeDOF;
	std::vector<int> activeDOFrowIndex;
	std::map<int,int> global_local_index;
	
	solutionClass<dim>* phi;
	std::vector<solutionClass<dim>* > outputVariables;
	
	DataOut<dim> outputData;

	double beta, gmma;
	double outBand,innerBand;
	std::vector<double> dx;
	double dr, vn, dt,dtau;
	
	int reIniInteration;
	double currentTime;
	double totalTime;
	
};

#endif