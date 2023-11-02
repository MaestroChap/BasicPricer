#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

#define UNDEFINITE_TOKEN -999.0

typedef unsigned int Unt;

enum class GreekKey
{
	Price = 0,
	Delta,
	Vega,
	Rho,
	Gamma,
	Theta
};

typedef std::unordered_map<GreekKey, double> GreekContainer;