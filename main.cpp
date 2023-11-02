#include <iostream>
#include "interface.hpp"

int main()
{
    int step = 500; //1000 is enough
    int nbMonteCarloSteps = 100000;
    double spaceStep = 0.001;

    double S0 = 50;
    double strike = 52;
    double volatility = 0.30;
    double riskFreeRate = 0.05;
    double maturity = 0.5;

    std::unique_ptr<QuantModels> modelMC(new BasicMonteCarlo(nbMonteCarloSteps, spaceStep)); // Monte Carlo

    std::unique_ptr<QuantModels> modelTree(new Tree(step, spaceStep)); // Tree

    std::unique_ptr<QuantModels> modelBS(new BlackScholes(spaceStep)); // Black Scholes

    std::unique_ptr<EuropeanOption> callOpt(new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<EuropeanOption> putOpt(new EuropeanPut(S0, strike, volatility, maturity, riskFreeRate));

    std::unique_ptr<AmericanOption> callOpt1(new AmericanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<AmericanOption> putOpt1(new AmericanPut(S0, strike, volatility, maturity, riskFreeRate));

    
    double priceMC = PriceEuropean(callOpt, modelMC);
    double priceBS = PriceEuropean(callOpt, modelBS);
    double priceTree = PriceEuropean(callOpt, modelTree);

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
    std::cout << "Monte Carlo model : " << priceMC << " | " << nbMonteCarloSteps << " steps" << std::endl;
    std::cout << "Tree model : " << priceTree << " | " << step << " steps" << std::endl;
    
    return 0;
}