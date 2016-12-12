#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>


__device__ void reInitialization(double *U);
//kernel
__global__ void finiteD_gpu(double *U, double dt, double vn, int totalStep)
{		
	reInitialization(U);
}


__device__ void reInitialization(double *U)
{
        double d_dh=0.2;
	int x=threadIdx.x;
	int y=blockIdx.x;
	int z=blockIdx.y;
	
	int index=z*blockDim.x*gridDim.x+y*blockDim.x+x;
	
	double coords[3];
	coords[0]=x*d_dh;
	coords[1]=y*d_dh;
	coords[2]=z*d_dh;
	
	double tem_phi;
	if(coords[2]>5)tem_phi=std::sqrt((coords[0]-3)*(coords[0]-3)+(coords[1]-5)*(coords[1]-5)+(coords[2]-7)*(coords[2]-7))-1;
	else tem_phi=std::sqrt((coords[0]-7)*(coords[0]-7)+(coords[1]-5)*(coords[1]-5)+(coords[2]-3)*(coords[2]-3))-1;
U[index]=tem_phi;
	//U[index]=fmin(tem_phi1,tem_phi2);
	
}

__global__ void update_phi(double *U,double dt, double vn, int step)
{
	double d_dh=0.2;

	int my_x=threadIdx.x;
	int my_y=blockIdx.x;
	int my_z=blockIdx.y;
	
	int nei_x,nei_y,nei_z;
	int nei_index;
	
	int dof=blockDim.x*gridDim.x*gridDim.y;
	
	//start from time step i for U
	int index=dof*step+my_z*blockDim.x*gridDim.x+my_y*blockDim.x+my_x;
	double my_phi=U[index-dof];
	
	//
	double dmx, dpx,dmy,dpy,dmz,dpz,fluxx,fluxy,fluxz;

	nei_x=my_x-1;
	nei_y=my_y;
	nei_z=my_z;
	//
	nei_index=dof*(step-1)+nei_z*blockDim.x*gridDim.x+nei_y*blockDim.x+nei_x;
	if(nei_x<0) dmx=0;
	else dmx=(my_phi-U[nei_index])/d_dh;
	
	nei_x=my_x+1;
	nei_y=my_y;
	nei_z=my_z;
	//
	nei_index=dof*(step-1)+nei_z*blockDim.x*gridDim.x+nei_y*blockDim.x+nei_x;
	if(nei_x>blockDim.x-1) dpx=0;
	else dpx=(U[nei_index]-my_phi)/d_dh;
	
	fluxx=fmax(dmx,0.0)*fmax(dmx,0.0)+fmin(dpx,0.0)*fmin(dpx,0.0);
	//--------------------------
	
	nei_x=my_x;
	nei_y=my_y-1;
	nei_z=my_z;
	//
	nei_index=dof*(step-1)+nei_z*blockDim.x*gridDim.x+nei_y*blockDim.x+nei_x;
	if(nei_y<0) dmy=0;
	else dmy=(my_phi-U[nei_index])/d_dh;
	
	nei_x=my_x;
	nei_y=my_y+1;
	nei_z=my_z;
	//
	nei_index=dof*(step-1)+nei_z*blockDim.x*gridDim.x+nei_y*blockDim.x+nei_x;
	if(nei_y>gridDim.x-1) dpy=0;
	else dpy=(U[nei_index]-my_phi)/d_dh;
	
	fluxy=fmax(dmy,0.0)*fmax(dmy,0.0)+fmin(dpy,0.0)*fmin(dpy,0.0);
	//--------------------------
	
	nei_x=my_x;
	nei_y=my_y;
	nei_z=my_z-1;
	//
	nei_index=dof*(step-1)+nei_z*blockDim.x*gridDim.x+nei_y*blockDim.x+nei_x;
	if(nei_z<0) dmz=0;
	else dmz=(my_phi-U[nei_index])/d_dh;
	
	nei_x=my_x;
	nei_y=my_y;
	nei_z=my_z+1;
	//
	nei_index=dof*(step-1)+nei_z*blockDim.x*gridDim.x+nei_y*blockDim.x+nei_x;
	if(nei_z>gridDim.y-1) dpz=0;
	else dpz=(U[nei_index]-my_phi)/d_dh;
	
	fluxz=fmax(dmz,0.0)*fmax(dmz,0.0)+fmin(dpz,0.0)*fmin(dpz,0.0);
	
	
	U[index]=my_phi-vn*sqrt(fluxx+fluxy+fluxz)*dt;
	
}
