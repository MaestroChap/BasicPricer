#include <iostream>
#include "interface.hpp"

int main()
{
    int nbMonteCarloStepsAmerican = 1000;
    int nbMonteCarloStepsEuropean = 100000;
    double spaceStep = 0.001;

    double S0 = 50;
    double strike = 52;
    double volatility = 0.30;
    double riskFreeRate = 0.05;
    double maturity = 0.5;

    std::unique_ptr<QuantModels> europeanModelMC(new BasicMonteCarlo(nbMonteCarloStepsEuropean, spaceStep));
    std::unique_ptr<QuantModels> americanModelMC(new BasicMonteCarlo(nbMonteCarloStepsAmerican, spaceStep));
    std::unique_ptr<QuantModels> modelBS(new BlackScholes(spaceStep));

    std::unique_ptr<EuropeanOption> callOpt(new EuropeanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<EuropeanOption> putOpt(new EuropeanPut(S0, strike, volatility, maturity, riskFreeRate));

    std::unique_ptr<AmericanOption> callOpt1(new AmericanCall(S0, strike, volatility, maturity, riskFreeRate));
    std::unique_ptr<AmericanOption> putOpt1(new AmericanPut(S0, strike, volatility, maturity, riskFreeRate));

    double priceAmerican = Price1(putOpt1, americanModelMC); //Todo change the BS model and create and tree model

    std::cout << "Prix des put" << std::endl;
    std::cout << "Put americain : " << Price1(putOpt1, americanModelMC) << std::endl;
    std::cout << "Put europeen : " << Price(putOpt, europeanModelMC)  << std::endl;
    std::cout << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << std::endl;
    std::cout << "Prix des call" << std::endl;
    std::cout << "call americain : " << Price1(callOpt1, americanModelMC) << std::endl;
    std::cout << "call europeen : " << Price(callOpt, europeanModelMC) << std::endl;

    

    /*
    std::cout << "CALL" << std::endl;
    std::cout << "Caracteristiques du put europeen via MC" << std::endl;
    display(callMC);

    std::cout << "----------------" << std::endl;
    std::cout << "Caracteristiques du put europeen via BS" << std::endl;
    display(callBS);

    std::cout << std::endl;
    std::cout << "PUT" << std::endl;
    std::cout << "Caracteristiques du put europeen via MC" << std::endl;
    display(putMC);

    std::cout << "----------------" << std::endl;
    std::cout << "Caracteristiques du put europeen via BS" << std::endl;
    display(putBS);
    */
    return 0;
}