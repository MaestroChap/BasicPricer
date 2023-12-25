#include "BrownianMotion1D.hpp"
#include <iostream>
#include "interface.hpp"
#include "Maths.hpp"
#include "Stock.hpp"


int main()
{
    int step = 500;
    int nbMonteCarloSteps = 100000;
    double spaceStep = 0.0001;

    double S0 = 50;
    double strike = 50;
    double volatility = 0.4;
    double riskFreeRate = 0.1;
    double maturity = 0.4167;

    std::unique_ptr<QuantModels> monteCarlo_model(new BasicMonteCarlo(nbMonteCarloSteps, spaceStep)); // Monte Carlo
    std::unique_ptr<QuantModels> blackScholes_model(new BlackScholes(spaceStep)); // Black Scholes
    std::unique_ptr<QuantModels> crr_model(new CRR(step, spaceStep)); // Binomial Tree

    std::unique_ptr<Option> EuropeanCall1(new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate)); // European Call
    std::unique_ptr<Option> EuropeanCall2(new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<Option> EuropeanCall3(new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate));

    std::unique_ptr<Option> EuropeanPut1(new EuropeanPut(S0, strike, volatility, maturity, riskFreeRate)); // European Put

    std::unique_ptr<Option> AmericanCall1(new AmericanCall(S0, strike, volatility, maturity, riskFreeRate)); // American call
    std::unique_ptr<Option> AmericanPut1(new AmericanPut(S0, strike, volatility, maturity, riskFreeRate)); // American Put

    PricingAnalytics statsCRRCall = PriceAndGreeks(AmericanCall1, crr_model); // Pricing CRR
    PricingAnalytics statsCRRPut = PriceAndGreeks(AmericanPut1, crr_model);

    PricingAnalytics statsBSCall = PriceAndGreeks(AmericanCall1, blackScholes_model); // Pricing Black Scholes
    PricingAnalytics statsBSPut = PriceAndGreeks(AmericanPut1, blackScholes_model);

    PricingAnalytics statsMCCall = PriceAndGreeks(AmericanCall1, monteCarlo_model); // Pricing Monte Carlo
    PricingAnalytics statsMCPut = PriceAndGreeks(AmericanPut1, monteCarlo_model);

    PrintOptionParameters(AmericanPut1);

    std::cout << "--------------" << std::endl;

    std::cout << "BLACK SCHOLES " << std::endl;
    
    Print(statsBSPut);

    std::cout << "-----------" << std::endl;

    std::cout << "MONTE CARLO " << std::endl;

    Print(statsMCPut);

    std::cout << "-----------" << std::endl;

    std::cout << " CRR Model " << std::endl;

    Print(statsCRRPut);


    return 0;
}