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

//European Options constructors

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
	m_Greeks = instr.m_Greeks;
}

// American Option constructors

AmericanOption::AmericanOption(double S0, double strike, double volatility, double maturity, double riskFreeRate) :
	Instrument(S0), m_strike(strike), m_volatility(volatility), m_maturity(maturity), m_riskFreeRate(riskFreeRate)
{

}

AmericanOption::AmericanOption(const AmericanOption& instr) : Instrument(instr)
{
	m_maturity = instr.m_maturity;
	m_riskFreeRate = instr.m_riskFreeRate;
	m_strike = instr.m_strike;
	m_volatility = instr.m_volatility;
	m_Greeks = instr.m_Greeks;
}