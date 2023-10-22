#pragma once

#include "typedefs.hpp"
class Instrument
{
public:
	Instrument(double S0);
	Instrument(const Instrument& instr); // deep copy
	virtual void setGreek(double greekValue, GreekKey key);
	//todo virtual ~Instrument() = 0; // this renders the class Instrument abstract

	double getS0() { return m_S0; }
	double getPrice() { return m_Price; };
	void setS0(double S0) { m_S0 = S0; m_Price = UNDEFINITE_TOKEN; }
	void setPrice(double price) { m_Price = price; };
	bool hasBeenPriced() { return m_Price != UNDEFINITE_TOKEN; }

protected:
	double m_S0;
	double m_Price;
	GreekContainer m_Greeks;
};

class EuropeanOption : public Instrument
{
public:
	EuropeanOption(double S0, double strike, double volatility, double maturity, double riskFreeRate);
	EuropeanOption(const EuropeanOption& instr); // deep copy
	virtual EuropeanOption* DeepClone() const = 0;

	virtual double getOptionSign() const = 0;

	double getStrike() { return m_strike; }
	double getVolatility() { return m_volatility; }
	double getMaturity() { return m_maturity; }
	double getRiskFreeRate() { return m_riskFreeRate; }

	void setStrike(double strike) { m_strike = strike; m_Price = UNDEFINITE_TOKEN;}
	void setVolatility(double volatility) { m_volatility = volatility; m_Price = UNDEFINITE_TOKEN;}
	void setMaturity(double maturity) { m_maturity = maturity; m_Price = UNDEFINITE_TOKEN;}
	void setRiskFreeRate(double riskFreeRate) { m_riskFreeRate = riskFreeRate; m_Price = UNDEFINITE_TOKEN;}
protected:
	double m_strike;
	double m_volatility;
	double m_maturity;
	double m_riskFreeRate;
};

class EuropeanCall : public EuropeanOption
{
public:
	EuropeanCall(double S0, double strike, double volatility, double maturity, double riskFreeRate);
	EuropeanCall(const EuropeanCall& instr) : EuropeanOption(instr) {};
	virtual EuropeanCall* DeepClone() const override;

	virtual double getOptionSign() const override;
};

class EuropeanPut : public EuropeanOption
{
public:
	EuropeanPut(double S0, double strike, double volatility, double maturity, double riskFreeRate);
	EuropeanPut(const EuropeanCall& instr) : EuropeanOption(instr) {};
	virtual EuropeanPut* DeepClone() const override;

	virtual double getOptionSign() const override;


};