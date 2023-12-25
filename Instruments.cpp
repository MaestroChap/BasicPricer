#include "Instruments.hpp"

// Instrument constructors & destructors

Instrument::Instrument(double S0) : m_S0(S0)
{
	m_Price = UNDEFINITE_TOKEN;
	m_Greeks = GreekContainer();
}

Instrument::Instrument(const Instrument& instr)
{
	m_S0 = instr.m_S0;
	m_Price = instr.m_Price;
	m_Greeks = instr.m_Greeks;
}

Instrument::~Instrument()
{
}

Option::Option(double S0, double strike, double volatility, double maturity, double riskFreeRate) :
	Instrument(S0), m_strike(strike), m_volatility(volatility), m_maturity(maturity), m_riskFreeRate(riskFreeRate)
{

}

Option::Option(const Option& option) : Instrument(option)
{
	m_volatility = option.m_volatility;
	m_maturity = option.m_maturity;
	m_riskFreeRate = option.m_riskFreeRate;
}

//European Options constructors

EuropeanOption::EuropeanOption(double S0, double strike, double volatility, double maturity, double riskFreeRate) :
	Option(S0,strike,volatility,maturity,riskFreeRate)
{

}

EuropeanOption::EuropeanOption(const EuropeanOption& option) : Option(option)
{
	m_maturity = option.m_maturity;
	m_riskFreeRate = option.m_riskFreeRate;
	m_strike = option.m_strike;
	m_volatility = option.m_volatility;
	m_Greeks = option.m_Greeks;
}

// American Option constructors

AmericanOption::AmericanOption(double S0, double strike, double volatility, double maturity, double riskFreeRate) :
	Option(S0, strike, volatility, maturity, riskFreeRate)
{

}

AmericanOption::AmericanOption(const AmericanOption& option) : Option(option)
{
	m_maturity = option.m_maturity;
	m_riskFreeRate = option.m_riskFreeRate;
	m_strike = option.m_strike;
	m_volatility = option.m_volatility;
	m_Greeks = option.m_Greeks;
}