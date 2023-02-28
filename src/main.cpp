#include <iostream>
#include "Swarm.hpp"
#include "func.hpp"

int main() {

	Swarm swarm(
		false,									// local
		0,										// neighbourhood
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

	swarm.execute();
}