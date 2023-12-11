#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

#define UNDEFINITE_TOKEN -999.0

typedef unsigned int Unt;

typedef std::unordered_map<std::string, double> GreekContainer;

struct PricingAnalytics
{
	double price;
	GreekContainer greeks;
	//std::pair<double, double> intervalValues;
	//double intervalPrecision; // 95% in general;
};

using PricingAnalytics = struct PricingAnalytics;

struct OptionParameters
{
	double S0;
	double strike;
	double volatility;
	double maturity;
	double riskFreeRate;
};