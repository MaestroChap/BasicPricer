#include "interface.hpp"

std::string GreekKeyToStr(GreekKey key)
{
	if (key == GreekKey::Price)
		return "Price";
	else if (key == GreekKey::Delta)
		return "Delta";
	else if (key == GreekKey::Vega)
		return "Vega";
	else if (key == GreekKey::Rho)
		return "Rho";
	else if (key == GreekKey::Gamma)
		return "Gamma";
	else if (key == GreekKey::Theta)
		return "Theta";
	else
		return "Unknown key in greek container";
}
double Price(std::unique_ptr<EuropeanOption>& instr, const std::unique_ptr<QuantModels>& mod)
{
	return mod->europeanOptionPrice(instr);
}

GreekContainer PriceAndGreeks(std::unique_ptr<EuropeanOption>& instr, const std::unique_ptr<QuantModels>& mod)
{
	return mod->europeanOptionGreeks(instr);
}

void display(const GreekContainer& cont)
{
	for (auto it = cont.begin(); it != cont.end(); it++)
	{
		std::cout << GreekKeyToStr(it->first) << " : " << it->second << std::endl;
	}
}
