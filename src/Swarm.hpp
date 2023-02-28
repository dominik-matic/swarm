#include <iostream>
#include <random>
#include <vector>

class Swarm {
	bool local;
	unsigned neighbourhood;
	size_t iters;
	unsigned popSize;
	unsigned dimSize;
	std::vector<double> xmin;
	std::vector<double> xmax;
	std::vector<double> vmin;
	std::vector<double> vmax;
	double c1;
	double c2;

	double (*func)(std::vector<double>); // specific to the problem


	std::vector<double>* lbest(size_t index, std::vector<std::vector<double>>& pbest, std::vector<double>& pbest_f, std::vector<double>& gbest) {
		if(local) {
			size_t bestInd = index;
			double best_f = pbest_f[index];
			for(size_t i = index - neighbourhood + 1; i < index + neighbourhood; ++i) {
				size_t ind = i;
				if(ind < 0) {
					ind = popSize + ind;
				} else if(ind >= popSize) {
					ind = ind - popSize;
				}
				if(pbest_f[ind] < best_f) {
					bestInd = ind;
					best_f = pbest_f[ind];
				}
			}
			return &pbest[bestInd];
		} else {
			return &gbest;
		}
	}

	public:
	Swarm(bool local, unsigned neighbourhood,
		size_t iters, unsigned popSize, unsigned dimSize,
		std::vector<double> xmin, std::vector<double> xmax,
		std::vector<double> vmin, std::vector<double> vmax,
		double c1, double c2,
		double (*func)(std::vector<double>)) : local(local), neighbourhood(neighbourhood), iters(iters), popSize(popSize), dimSize(dimSize), xmin(xmin), xmax(xmax), vmin(vmin), vmax(vmax), c1(c1), c2(c2), func(func) {}


	void execute() {
		std::random_device rd;  
    	std::mt19937 generator(rd());
		//std::default_random_engine generator;
		std::uniform_real_distribution<double> dist(0.0, 1.0);

		std::vector<std::vector<double>> x(popSize, std::vector<double>(dimSize, 0));			// polozaji svih cestica
		std::vector<std::vector<double>> v(popSize, std::vector<double>(dimSize, 0));			// brzine svih cestica
		std::vector<std::vector<double>> pbest(popSize, std::vector<double>(dimSize, 0));		// najbolji polozaji svih cestica
		std::vector<double> pbest_f(popSize, 0);												// najbolje vrijednosti svih cestica
		std::vector<double> f(popSize, 0);														// trenutne vrijednosti svih cestica
		std::vector<double> gbest(dimSize, 0);
		double gbest_f;
		

		// init pop
		for(size_t i = 0; i < popSize; ++i) {
			for(size_t j = 0; j < dimSize; ++j) {
				std::uniform_real_distribution<double> dx(xmin[j], xmax[j]);
				std::uniform_real_distribution<double> dv(vmin[j], vmax[j]);
				x[i][j] = dx(generator);
				v[i][j] = dv(generator);
			}
		}

		//for(size_t i = 0; i < popSize; ++i) {
		//	for(size_t dim = 0; dim < dimSize; ++dim) {
		//		std::cout << v[i][dim] << " ";
		//	}
		//	std::cout << "\n";
		//}

		// init best as current
		for(size_t i = 0; i < popSize; ++i) {
			pbest[i] = x[i];
			pbest_f[i] = func(x[i]);
		}
		gbest = pbest[0];
		gbest_f = pbest_f[0];

		for(size_t it = 0; it < iters; ++it) {
			// evaluate pop (can be done last as a slight improvement)
			for(size_t i = 0; i < popSize; ++i) {
				f[i] = func(x[i]);
			}

			// did it improve
			for(size_t i = 0; i < popSize; ++i) {
				if(f[i] < pbest_f[i]) {
					pbest_f[i] = f[i];
					pbest[i] = x[i];
				}
			}

			// found new globabl best?
			for(size_t i = 0; i < popSize; ++i) {
				if(f[i] < gbest_f) {
					gbest_f = f[i];
					gbest = x[i];
				}
			}

			//std::cout << x[0][0] << " " << x[0][1] << " " << x[0][2] << " " << x[0][3] << " " << x[0][4] << "\n";


			// update position and speed of every particle
			for(size_t i = 0; i < popSize; ++i) {
				std::vector<double>* lgbest = lbest(i, pbest, pbest_f, gbest);
				//std::cout << (*lgbest)[0] << " " << (*lgbest)[1] << " " << (*lgbest)[2] << " " << (*lgbest)[3] << " " << (*lgbest)[4] << "\n";
				for(size_t j = 0; j < dimSize; ++j) {
					v[i][j] = 0.729 * (v[i][j] 	+ c1 * dist(generator) * (pbest[i][j] - x[i][j])
												+ c2 * dist(generator) * ((*lgbest)[j] - x[i][j]));
					if(v[i][j] > vmax[j]) { v[i][j] = vmax[j]; }
					if(v[i][j] < vmin[j]) { v[i][j] = vmin[j]; }
					x[i][j] = x[i][j] + v[i][j];
				}
			}

			if(it % 100 == 0) {
				std::cout << "Iter " << it <<":\tBest: ";
				for(size_t i = 0; i < dimSize; ++i) {
					std::cout << gbest[i] << " ";
				}
				std::cout << "\tError: " << gbest_f << "\n";
			}
		}

		std::cout << "Iter " << iters << ":\tBest: ";
		for(size_t i = 0; i < dimSize; ++i) {
			std::cout << gbest[i] << " ";
		}
		std::cout << "\tError: " << gbest_f << "\n";
	}
};