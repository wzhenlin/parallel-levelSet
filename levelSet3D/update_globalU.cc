#include "finiteD_levelSet.h"

using namespace std;

template <int dim>
void finiteD_levelSet<dim>::update_globalU(){
	for (unsigned int activelID=0; activelID<activeDOF[0].size();activelID++){
		if(activeU[activelID]>gmma) U[activeDOF[0][activelID]]=gmma;
		else if(activeU[activelID]<-gmma)U[activeDOF[0][activelID]]=-gmma;
		else	U[activeDOF[0][activelID]]=activeU[activelID];
	}
}

template class finiteD_levelSet<1>;
template class finiteD_levelSet<2>;
template class finiteD_levelSet<3>;