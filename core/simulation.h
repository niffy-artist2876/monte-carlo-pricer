#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>

typedef struct {
	double S;
	double K;
	double r;
	double sigma;
	double T;
} OptionParams;

std::vector<double> simulate_path(OptionParams parameters, int time_steps);
#endif
