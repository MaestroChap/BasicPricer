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
	m_Greeks = instr.m_Greeks;
}


void Instrument::setGreek(double greekValue, GreekKey key)
{
	m_Greeks[key] = greekValue;
}

//EUROPEAN OPTION


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

EuropeanCall* EuropeanCall::DeepClone() const
{
	return new EuropeanCall(*this); // this calls the copy constructor of EuropeanCall which is a deep copy
}

double EuropeanCall::getOptionSign() const
{
	// S-K
	return 1.;
}

EuropeanPut::EuropeanPut(double S0, double strike, double volatility, double maturity, double riskFreeRate) : EuropeanOption(S0, strike, volatility, maturity, riskFreeRate)
{
}

EuropeanPut* EuropeanPut::DeepClone() const
{
	return new EuropeanPut(*this); // this calls the copy constructor of EuropeanCall which is a deep copy
}

double EuropeanPut::getOptionSign() const
{
	// K-S
	return -1.;
}


// AMERICAN OPTIONS 

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
}


AmericanCall::AmericanCall(double S0, double strike, double volatility, double maturity, double riskFreeRate) : AmericanOption(S0, strike, volatility, maturity, riskFreeRate)
{

}

AmericanCall::AmericanCall(const AmericanCall& instr) : AmericanOption(instr)
{
	
}

AmericanPut::AmericanPut(double S0, double strike, double volatility, double maturity, double riskFreeRate) : AmericanOption(S0, strike, volatility, maturity, riskFreeRate)
{

}

AmericanPut::AmericanPut(const AmericanPut& instr) : AmericanOption(instr)
{

}

double AmericanCall::getOptionSign() const
{
	return 1.;
}

double AmericanPut::getOptionSign() const
{
	return -1.;
}