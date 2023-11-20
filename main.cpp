#include <iostream>
#include "interface.hpp"
#include "Maths.hpp"
int main()
{
    int step = 500;
    int nbMonteCarloSteps = 100000;
    double spaceStep = 0.001;

    double S0 = 50;
    double strike = 52;
    double volatility = 0.30;
    double riskFreeRate = 0.05;
    double maturity = 0.5;
    std::unique_ptr<QuantModels> monteCarlo_model(new BasicMonteCarlo(nbMonteCarloSteps, spaceStep)); // Monte Carlo

    std::unique_ptr<QuantModels> crr_model(new CRR(step, spaceStep)); // Binomial Tree

    std::unique_ptr<QuantModels> blackScholes_model(new BlackScholes(spaceStep)); // Black Scholes

    std::unique_ptr<EuropeanOption> callOpt(new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<EuropeanOption> putOpt(new EuropeanPut(S0, strike, volatility, maturity, riskFreeRate));

    std::unique_ptr<AmericanOption> callOpt1(new AmericanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<AmericanOption> putOpt1(new AmericanPut(S0, strike, volatility, maturity, riskFreeRate));

    double priceMC = PriceEuropean(callOpt, monteCarlo_model);
    double priceBS = PriceEuropean(callOpt, blackScholes_model);
    double priceTree = PriceEuropean(callOpt, crr_model);

    std::cout << "Caracteristiques du call europeen : " << std::endl;
    std::cout << std::endl;
    std::cout << "S0: " << S0 << std::endl;
    std::cout << "Strike : " << strike << std::endl;
    std::cout << "Volatilite : " << volatility << std::endl;
    std::cout << "RiskFreeRate : " << riskFreeRate << std::endl;
    std::cout << "Maturity : " << maturity << std::endl;
    std::cout << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << "Price : "<< std::endl;
    std::cout << std::endl;
    std::cout << "Black Scholes model : " << priceBS << std::endl;
    std::cout << "Monte Carlo model : " << priceMC << "| " << "[" << ConfidenceIntervalEuropeanOption(callOpt, monteCarlo_model).first << "," << ConfidenceIntervalEuropeanOption(callOpt, monteCarlo_model).second
        << "]" << " | IntervalPrecision : 95 % | " << nbMonteCarloSteps << " steps" << std::endl;

    std::cout << "Tree model : " << priceTree << " | " << step << " steps" << std::endl;

    return 0;
}