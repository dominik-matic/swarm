#include <iostream>
#include "Swarm.hpp"
#include "func.hpp"

int main(int argc, char* argv[]) {

	if(argc < 2) {
		std::cerr << "Please supply neighbourhood.\n";
		exit(-1);
	}

	std::string neigh = argv[1];
	unsigned n = stoi(neigh);

	Swarm swarm2(
		true,									// local
		n,										// neighbourhood
		1000,									// iters
		50,										// popSize
		6,										// dimSize
		{-10, -10, -10, -10, -10, -10},			// xmin
		{10, 10, 10, 10, 10, 10},				// xmax
		{-1, -1, -1, -1, -1, -1},				//vmin
		{1, 1, 1, 1, 1, 1},						//vmax
		2.05,									// c1
		2.05,									// c2
		func									// func
	);
	swarm2.execute();
	
}