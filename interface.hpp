#pragma once

#include "QuantModels.hpp"
#include "typedefs.hpp"
#include "Instruments.hpp"


double Price(std::unique_ptr<EuropeanOption>& instr, const std::unique_ptr<QuantModels>& mod);
GreekContainer PriceAndGreeks(std::unique_ptr<EuropeanOption>& instr, const std::unique_ptr<QuantModels>& mod);
void display(const GreekContainer& cont);
