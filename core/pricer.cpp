#include "payoffs.h"
#include "pricer.h"
#include <cmath>
#include <numeric>
#include <vector>

MCResult mc_price(OptionParams parameters, int paths, int time_steps, OptionType option_type){
	MCResult res;
	std::vector<double> payoffs(paths);
	std::vector<double> path;
	for(int i = 0; i<paths; i++){
		path = simulate_path(parameters, time_steps);
		double last = path.back();
		option_type == OptionType::CALL ? payoffs[i] = call_payoff(last, parameters.K) : payoffs[i] = put_payoff(last, parameters.K);
	}

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

