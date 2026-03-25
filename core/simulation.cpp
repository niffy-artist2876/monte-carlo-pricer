#include "simulation.h"
#include <cmath>
#include <vector>
#include <random>

std::vector<double> simulate_path(OptionParams parameters, int time_steps){
	std::mt19937_64 rng(std::random_device{}());
	std::normal_distribution<double> dist(0.0, 1.0);
	double dt = static_cast<double>(parameters.T) / time_steps;
    double drift = (parameters.r - 0.5 * parameters.sigma * parameters.sigma) * dt;
    double diffusion = parameters.sigma * std::sqrt(dt);
	std::vector<double> path(time_steps+1);
	path[0] = parameters.S;
	for(int i = 1; i<=time_steps; i++){
		path[i] = path[i - 1] * std::exp(drift + diffusion * dist(rng));
	}
	return path;
}
