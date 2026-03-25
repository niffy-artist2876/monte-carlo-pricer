#include <iostream>
#include <cstdlib>
#include <string>
#include "core/payoffs.h"
#include "core/pricer.h"
#include "core/simulation.h"

int main(int argc, char* argv[]) {
    // Defaults
    double S = 100.0, K = 100.0, r = 0.05, sigma = 0.20, T = 1.0;
    int paths = 100000, time_steps = 252;
    std::string option_type_str = "call";

    // Parse CLI args: S K r sigma T paths time_steps option_type
    if (argc >= 2) S          = std::atof(argv[1]);
    if (argc >= 3) K          = std::atof(argv[2]);
    if (argc >= 4) r          = std::atof(argv[3]);
    if (argc >= 5) sigma      = std::atof(argv[4]);
    if (argc >= 6) T          = std::atof(argv[5]);
    if (argc >= 7) paths      = std::atoi(argv[6]);
    if (argc >= 8) time_steps = std::atoi(argv[7]);
    if (argc >= 9) option_type_str = std::string(argv[8]);

    OptionType opt = (option_type_str == "put") ? OptionType::PUT : OptionType::CALL;
    OptionParams parameters = {S, K, r, sigma, T};

    MCResult res = mc_price(parameters, paths, time_steps, opt);

    // Output as simple key=value pairs for easy parsing
    std::cout << "price="     << res.price     << "\n"
              << "std_error=" << res.std_error  << "\n"
              << "ci_lower="  << res.ci_lower   << "\n"
              << "ci_upper="  << res.ci_upper   << "\n";
    return 0;
}
