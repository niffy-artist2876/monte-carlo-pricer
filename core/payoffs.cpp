#include "payoffs.h"
#include <algorithm>

double call_payoff(double S_T, double K){
	return std::max(S_T-K, 0.0);
}

double put_payoff(double S_T, double K){
	return std::max(K-S_T, 0.0);
}
