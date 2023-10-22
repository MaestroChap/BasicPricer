#include <iostream>
#include "interface.hpp"

int main()
{
    int nbMonteCarloSteps = 100000;
    double spaceStep = 0.01;

    double strike = 100;
    double S0 = 100;
    double volatility = 0.30;
    double riskFreeRate = 0.1;
    double maturity = 0.2;

    std::unique_ptr<QuantModels> modelMC(new BasicMonteCarlo(nbMonteCarloSteps, spaceStep));
    std::unique_ptr<QuantModels> modelBS(new BlackScholes(spaceStep));
    std::unique_ptr<EuropeanOption> callOpt (new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate));
    //std::unique_ptr<EuropeanOption> putOpt(new EuropeanPut(S0, strike, volatility, maturity, riskFreeRate));

    auto callGreeksMC = PriceAndGreeks(callOpt, modelMC);
    auto callPriceBS = PriceAndGreeks(callOpt, modelBS);
    std::cout << "prix du call via MC" << std::endl;
    display(callGreeksMC);
    std::cout << std::endl;
    std::cout << "prix du call via BS" << std::endl;
    display(callPriceBS);
    return 0;
}