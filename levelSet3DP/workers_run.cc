#include "MPI_finiteD_levelSet.h"

using namespace std;

template <int dim>
void MPI_finiteD_levelSet<dim>::workers_run(){
	double tem;
	int i=0;
	while(true){
		int flag;
		int number_amount;
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if(status.MPI_TAG==stopWork) {
			break;
		}
		if(status.MPI_TAG==tag_output) {
			MPI_Get_count(&status, MPI_DOUBLE, &number_amount);
			output(number_amount);
		}
		if(status.MPI_TAG==finiteD){
			MPI_Get_count(&status, MPI_DOUBLE, &number_amount);
			finiteDifference(number_amount);
			reInitialization();
		}			
		if(status.MPI_TAG==work_setNarrowBand){
			MPI_Get_count(&status, MPI_DOUBLE, &number_amount);
			setNarrowBand(number_amount);
		}		
	}
}

template class MPI_finiteD_levelSet<1>;
template class MPI_finiteD_levelSet<2>;
template class MPI_finiteD_levelSet<3>;