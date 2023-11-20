#pragma once

#include "QuantModels.hpp"
#include "typedefs.hpp"
#include "Instruments.hpp"


double PriceEuropean(std::unique_ptr<EuropeanOption>& instr, const std::unique_ptr<QuantModels>& mod);
double PriceAmerican(std::unique_ptr<AmericanOption>& instr, const std::unique_ptr<QuantModels>& mod);
std::pair<double, double> ConfidenceIntervalEuropeanOption(std::unique_ptr<EuropeanOption>& instr, const std::unique_ptr<QuantModels>& mod);
//double Price(std::unique_ptr<Instrument>& instr, const std::unique_ptr<QuantModels>& mod);
GreekContainer PriceAndGreeksEuropean(std::unique_ptr<EuropeanOption>& instr, const std::unique_ptr<QuantModels>& mod);
void display(const GreekContainer& cont);