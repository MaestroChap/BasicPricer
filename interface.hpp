#pragma once

#include "QuantModels.hpp"
#include "typedefs.hpp"
#include "Instruments.hpp"

PricingAnalytics PriceAndGreeks(std::unique_ptr<Option>& option, const std::unique_ptr<QuantModels>& mod);
void Print(PricingAnalytics& pricingAnalytics);
void PrintOptionParameters(std::unique_ptr<Option>& option);
std::string GetOptionTypeName(const std::unique_ptr<Option>& option);