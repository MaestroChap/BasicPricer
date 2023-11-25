#pragma once

#include "QuantModels.hpp"
#include "typedefs.hpp"
#include "Instruments.hpp"

PricingAnalytics stats(std::unique_ptr<Option>& option, const std::unique_ptr<QuantModels>& mod);