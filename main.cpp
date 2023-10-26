#include <iostream>
#include "interface.hpp"

int main()
{
    int nbMonteCarloSteps = 100000;
    double spaceStep = 0.001;

    double strike = 100;
    double S0 = 100;
    double volatility = 0.30;
    double riskFreeRate = 0.05;
    double maturity = 0.2;

    std::unique_ptr<QuantModels> modelMC(new BasicMonteCarlo(nbMonteCarloSteps, spaceStep));
    std::unique_ptr<QuantModels> modelBS(new BlackScholes(spaceStep));
    std::unique_ptr<EuropeanOption> callOpt(new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<EuropeanOption> putOpt(new EuropeanPut(S0, strike, volatility, maturity, riskFreeRate));

    auto callMC = PriceAndGreeks(callOpt, modelMC);
    auto callBS = PriceAndGreeks(callOpt, modelBS);

    auto putMC = PriceAndGreeks(putOpt, modelMC);
    auto putBS = PriceAndGreeks(putOpt, modelBS);

    /* Somehow this is not working
    auto callMC = PriceAndGreeks(callOpt, modelMC);
    auto putBS = PriceAndGreeks(putOpt, modelBS);
    auto callBS = PriceAndGreeks(callOpt, modelBS);
    auto putMC = PriceAndGreeks(putOpt, modelMC);
    */
    std::cout << "Caracteristiques du call europeen via MC" << std::endl;
    display(putMC);

    std::cout << std::endl;
    std::cout << "Caracteristiques du put europeen via BS" << std::endl;
    display(putBS);
    return 0;
}