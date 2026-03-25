#ifndef PRICER_H
#define PRICER_H
#include "simulation.h"
#include <vector>

typedef struct {
	double price, std_error, ci_lower, ci_upper;
} MCResult;

enum class OptionType { CALL, PUT };

MCResult mc_price(OptionParams parameters, int paths, int time_steps, OptionType option_type);

#endif
