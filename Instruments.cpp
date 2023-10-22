#include "Instruments.hpp"

Instrument::Instrument(double S0) : m_S0(S0)
{
	m_Price = UNDEFINITE_TOKEN;
	m_Greeks = GreekContainer();
}

Instrument::Instrument(const Instrument& instr)
{
	m_S0 = instr.m_S0;
	m_Price = instr.m_Price;
	m_Greeks = GreekContainer(); // todo deep copy of the greeks
}


void Instrument::setGreek(double greekValue, GreekKey key)
{
	m_Greeks[key] = greekValue;
}

EuropeanOption::EuropeanOption(double S0, double strike, double volatility, double maturity, double riskFreeRate) :
	Instrument(S0), m_strike(strike), m_volatility(volatility), m_maturity(maturity), m_riskFreeRate(riskFreeRate)
{
}

EuropeanOption::EuropeanOption(const EuropeanOption& instr) : Instrument(instr)
{
	m_maturity = instr.m_maturity;
	m_riskFreeRate = instr.m_riskFreeRate;
	m_strike = instr.m_strike;
	m_volatility = instr.m_volatility;
}

EuropeanCall::EuropeanCall(double S0, double strike, double volatility, double maturity, double riskFreeRate) : EuropeanOption(S0, strike, volatility, maturity, riskFreeRate)
{
}

double EuropeanCall::getOptionSign() const
{
	// S-K
	return 1.;
}

EuropeanPut::EuropeanPut(double S0, double strike, double volatility, double maturity, double riskFreeRate) : EuropeanOption(S0, strike, volatility, maturity, riskFreeRate)
{
}

double EuropeanPut::getOptionSign() const
{
	// K-S
	return -1.;
}
