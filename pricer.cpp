#include "payoffs.h"
#include "pricer.h"
#include <cmath>
#include <numeric>
#include <vector>
#include <random>
#include <omp.h>



MCResult mc_price(OptionParams parameters, int paths, int time_steps, OptionType option_type){
	
	std::vector<double> payoffs(paths);
	#pragma omp parallel
	{
		std::vector<double> path;
		int thread = omp_get_thread_num();
		std::mt19937_64 rng;
		rng.seed(thread);
		#pragma omp for
		for(int i = 0; i<paths; i++){
			path = simulate_path(parameters, time_steps, rng);
			double last = path.back();
			option_type == OptionType::CALL ? payoffs[i] = call_payoff(last, parameters.K) : payoffs[i] = put_payoff(last, parameters.K);
		}
	}
	MCResult res;
	double mean = 0.0;
	for(auto payoff: payoffs){
		mean += payoff;
	}
	mean = mean/payoffs.size();
	res.price = mean*exp(-(parameters.r)*(parameters.T));
	
	double var = 0.0;
	for(auto payoff: payoffs){
		var += (payoff-mean)*(payoff-mean);
	}
	var = var/(paths-1);
	res.std_error = sqrt(var/paths)*exp(-parameters.r * parameters.T);

	res.ci_lower = res.price - 1.96 * res.std_error;
	res.ci_upper = res.price + 1.96 * res.std_error;
	return res;


}

