#include "interface.hpp"

PricingAnalytics stats(std::unique_ptr<Option>& option, const std::unique_ptr<QuantModels>& mod)
{
	return mod->setPricingAnalytics(option);
}