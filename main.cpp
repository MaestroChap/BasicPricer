#include <iostream>
#include "interface.hpp"
#include "Maths.hpp"
#include "Stock.hpp"


int main()
{
    int step = 50;
    int nbMonteCarloSteps = 100000;
    double spaceStep = 0.001;

    double S0 = 50;
    double strike = 50;
    double volatility = 0.40;
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

    PricingAnalytics statsCRRCall = stats(EuropeanCall1, crr_model); // Pricing CRR
    PricingAnalytics statsCRRPut = stats(AmericanPut1, crr_model);

    PricingAnalytics statsBSCall = stats(EuropeanCall1, blackScholes_model); // Pricing Black Scholes
    PricingAnalytics statsBSPut = stats(AmericanPut1, blackScholes_model);

    PricingAnalytics statsMCCall = stats(EuropeanCall1, monteCarlo_model); // Pricing Monte Carlo
    PricingAnalytics statsMCPut = stats(AmericanPut1, monteCarlo_model);

    std::cout << "Caracteristiques du Call europeen : " << std::endl;
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
    std::cout << "prix : " << statsBSPut.price << std::endl;

    for (auto& element : statsBSPut.greeks)
    {
        std::cout << element.first << " : " << element.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << std::endl;

    std::cout << "MONTE CARLO " << std::endl;
    std::cout << "prix : " << statsMCPut.price << std::endl;

    for (auto& element : statsMCPut.greeks)
    {
        std::cout << element.first << " : " << element.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << std::endl;

    std::cout << " CRR Model " << std::endl;
    std::cout << "Prix : " << statsCRRPut.price << std::endl;

    for (auto& element : statsCRRPut.greeks)
    {
        std::cout << element.first << " : " << element.second << std::endl;
    }

    return 0;
}

    /*
   int step = 50;
   int nbMonteCarloSteps = 100000;
   double spaceStep = 0.001;

   double S0 = 50;
   double strike = 50;
   double volatility = 0.40;
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

   PricingAnalytics statsCRRCall = stats(EuropeanCall1, crr_model); // Pricing CRR
   PricingAnalytics statsCRRPut = stats(AmericanPut1, crr_model);

   PricingAnalytics statsBSCall = stats(EuropeanCall1, blackScholes_model); // Pricing Black Scholes
   PricingAnalytics statsBSPut = stats(AmericanPut1, blackScholes_model);

   PricingAnalytics statsMCCall = stats(EuropeanCall1, monteCarlo_model); // Pricing Monte Carlo
   PricingAnalytics statsMCPut = stats(AmericanPut1, monteCarlo_model);

   std::cout << "Caracteristiques du Call europeen : " << std::endl;
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
   std::cout << "prix : " << statsBSPut.price << std::endl;

   for (auto& element : statsBSPut.greeks)
   {
       std::cout << element.first << " : " << element.second << std::endl;
   }
   std::cout << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << std::endl;

   std::cout << "MONTE CARLO " << std::endl;
   std::cout << "prix : " << statsMCPut.price << std::endl;

   for (auto& element : statsMCPut.greeks)
   {
       std::cout << element.first << " : " << element.second << std::endl;
   }
   std::cout << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << std::endl;

   std::cout << " CRR Model " << std::endl;
   std::cout << "Prix : " << statsCRRPut.price <<std::endl;

   for (auto& element : statsCRRPut.greeks)
   {
       std::cout << element.first << " : " << element.second << std::endl;
   }
   */