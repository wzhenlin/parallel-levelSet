#ifndef MPI_finiteD_levelSet_h
#define MPI_finiteD_levelSet_h

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <map>
#include <mpi.h>
#include "finiteDmesh.h"
#include "supplementary/parameters.h"
#include "supplementary/solutionClass.h"
#include "supplementary/DataOut.h"
using namespace std;
template <int dim>
class MPI_finiteD_levelSet
{
public:

  MPI_finiteD_levelSet (parametersClass& _params);
  ~MPI_finiteD_levelSet();
	
  void run();
	void setup();
  void apply_initial_values();
	void getGrid_id(int controlPoint_id, std::vector<int>& grid_id);
	void get_id(std::vector<int> grid_id, int& controlPoint_id);
	void resetNarrowBand();
	void setNarrowBand(int number_amount);
	double getC(double phi);
	double getS(double d, double grad_d);
	void reInitialization();
	void update_globalU();
	void update_localU();
	void output (int number_amount);
	
	//MPI functions
	void manager_setNarrowBand();
	void send_output(int PE, int sp_output_step=0, int output_size=0);
	void allocate_works();
	void workers_run();
  
	
  void finiteDifference(int number_amount);
	
  finiteDmesh<dim> mesh;
  parametersClass& params;
	
	std::vector<double> U;
	std::vector<double> last_U;
	
	int totalDOF,dofPerControlPoint;
	std::vector<int> ControlPointperDim, DOFperDim;
	
	std::vector<double> activeDOF;
	std::vector<int> activeDOFrowIndex;
	std::vector<int> allocation_index;
	
	//local parameters
	std::vector<std::vector<double> > local_activeDOF;
	std::vector<double> local_activeUn, local_activeU;
	std::map<int,int> global_local_index;
	//for reconstruct data structure 
	int ghost_bottom_row_points;
	int bottom_row_points;
	int top_row_points;
	int ghost_top_row_points;
	int local_num_active_points;
	
	
	
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
	int output_step;
	std::vector<int> points_per_level;
	
	// MPI parameters;
	int my_PE_num;
	int size_PE;
	MPI_Status status;  
	
	//tag
	int tag_output=0;
	int backData=2;
	int finishWriting=3;
	int stopWork=4;
	int finiteD=5;
	int work_setNarrowBand=6;
	int work_backNarrowBand=7;
	
	int down=6;
	int up=7;
	
	//control parameters
	int totalOutPut;
	int totalStep;
	
	
	
};

#endif