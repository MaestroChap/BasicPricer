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

    std::unique_ptr<Option> EuropeanCall1(new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<Option> EuropeanPut1(new EuropeanPut(S0, strike, volatility, maturity, riskFreeRate));

    std::unique_ptr<Option> AmericanCall1(new AmericanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<Option> AmericanPut1(new AmericanPut(S0, strike, volatility, maturity, riskFreeRate));

    PricingAnalytics statsBSCall = stats(EuropeanCall1, blackScholes_model);
    PricingAnalytics statsBSPut = stats(EuropeanPut1, blackScholes_model);

    PricingAnalytics statsMCCall = stats(EuropeanCall1, monteCarlo_model);
    PricingAnalytics statsMCPut = stats(EuropeanPut1, monteCarlo_model);

    PricingAnalytics statsCRRCall = stats(EuropeanCall1, crr_model);
    PricingAnalytics statsCRRPut = stats(EuropeanPut1, crr_model);


    std::cout << "Caracteristiques du put europeen : " << std::endl;
    std::cout << std::endl;
    std::cout << "S0: " << S0 << std::endl;
    std::cout << "Strike : " << strike << std::endl;
    std::cout << "Volatilite : " << volatility << std::endl;
    std::cout << "RiskFreeRate : " << riskFreeRate << std::endl;
    std::cout << "Maturity : " << maturity << std::endl;
    std::cout << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << std::endl;

    std::cout << "BLACK SCHOLES " << std::endl;
    std::cout << "prix : " << statsBSCall.price << std::endl;
    
    for (auto& element : statsBSCall.greeks)
    {
        std::cout << element.first << " : " << element.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << std::endl;
    std::cout << "MONTE CARLO " << std::endl;
    
    std::cout << "prix : " << statsMCCall.price << std::endl;
    
    for (auto& element : statsMCCall.greeks)
    {
        std::cout << element.first << " : " << element.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << std::endl;
    std::cout << " CRR Model " << std::endl;

    std::cout << "Prix : " << statsCRRCall.price <<std::endl;

    return 0;
}