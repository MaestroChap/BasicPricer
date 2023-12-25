#include "interface.hpp"
#include <typeinfo>

PricingAnalytics PriceAndGreeks(std::unique_ptr<Option>& option, const std::unique_ptr<QuantModels>& mod)
{
	return mod->setPricingAnalytics(option);
}

void Print(PricingAnalytics& pricingAnalytics)
{
    std::cout << "prix : " << pricingAnalytics.price << std::endl;

    for (auto& element : pricingAnalytics.greeks)
    {
        std::cout << element.first << " : " << element.second << std::endl;
    }
    std::cout << std::endl;
 }

void PrintOptionParameters(std::unique_ptr<Option>& option)
{
    std::string name = GetOptionTypeName(option);
    std::cout << "Parameters or the " << name << std::endl;
    std::cout << std::endl;
    std::cout << "S0: " << option->getS0() << std::endl;
    std::cout << "Strike : " << option->getStrike() << std::endl;
    std::cout << "Volatilite : " << option->getVolatility() << std::endl;
    std::cout << "RiskFreeRate : " << option->getRiskFreeRate() << std::endl;
    std::cout << "Maturity : " << option->getMaturity() << std::endl;
    std::cout << std::endl;
}

std::string GetOptionTypeName(const std::unique_ptr<Option>& option)
{
    const std::type_info& optionTypeInfo = typeid(*option);

    if (optionTypeInfo == typeid(EuropeanCall)) {
        return "European Call Option";
    }
    else if (optionTypeInfo == typeid(EuropeanPut)) {
        return "European Put Option";
    }
    else if (optionTypeInfo == typeid(AmericanCall)) {
        return "American Call Option";
    }
    else if (optionTypeInfo == typeid(AmericanPut)) {
        return "American Put Option";
    }
    else {
        return "Unknown Option Type";
    }
}

