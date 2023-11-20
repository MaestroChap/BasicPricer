#pragma once

#include "typedefs.hpp"

class Instrument
{
public:
	Instrument(double S0);
	Instrument(const Instrument& instr);
	virtual ~Instrument() = 0;

	double getS0() { return m_S0; }
	double getPrice() { return m_Price; }
	GreekContainer getGreek() { return m_Greeks; }

	void setS0(double S0) { m_S0 = S0; m_Price = UNDEFINITE_TOKEN; }
	void setPrice(double price) { m_Price = price; }
	void setGreek(double greekValue, GreekKey key) { m_Greeks[key] = greekValue; }

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
	EuropeanOption(const EuropeanOption& instr);
	virtual EuropeanOption* DeepClone() const = 0;
	virtual double getOptionSign() const = 0;

	double getStrike() const { return m_strike; }
	double getVolatility() const { return m_volatility; }
	double getMaturity() const { return m_maturity; }
	double getRiskFreeRate() const { return m_riskFreeRate; }

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
	EuropeanCall(double S0, double strike, double volatility, double maturity, double riskFreeRate) : EuropeanOption(S0, strike, volatility, maturity, riskFreeRate) {};
	EuropeanCall(const EuropeanCall& instr) : EuropeanOption(instr) {};
	virtual EuropeanCall* DeepClone() const override { return new EuropeanCall(*this); }
	virtual double getOptionSign() const override { return 1.; }
};

class EuropeanPut : public EuropeanOption
{
public:
	EuropeanPut(double S0, double strike, double volatility, double maturity, double riskFreeRate) : EuropeanOption(S0, strike, volatility, maturity, riskFreeRate) {};
	EuropeanPut(const EuropeanCall& instr) : EuropeanOption(instr) {};
	virtual EuropeanPut* DeepClone() const override { return new EuropeanPut(*this); }
	virtual double getOptionSign() const override { return 1.; }
};

class AmericanOption : public Instrument
{
public:
	AmericanOption(double S0, double strike, double volatility, double maturity, double riskFreeRate);
	AmericanOption(const AmericanOption& instr);
	virtual double getOptionSign() const = 0;

	double getStrike() const { return m_strike; }
	double getVolatility() const { return m_volatility; }
	double getMaturity() const { return m_maturity; }
	double getRiskFreeRate() const { return m_riskFreeRate; }

	void setStrike(double strike) { m_strike = strike; m_Price = UNDEFINITE_TOKEN; }
	void setVolatility(double volatility) { m_volatility = volatility; m_Price = UNDEFINITE_TOKEN; }
	void setMaturity(double maturity) { m_maturity = maturity; m_Price = UNDEFINITE_TOKEN; }
	void setRiskFreeRate(double riskFreeRate) { m_riskFreeRate = riskFreeRate; m_Price = UNDEFINITE_TOKEN; }

protected:
	double m_strike;
	double m_volatility;
	double m_maturity;
	double m_riskFreeRate;
};

class AmericanCall : public AmericanOption
{
public:
	AmericanCall(double S0, double strike, double volatility, double maturity, double riskFreeRate) : AmericanOption(S0, strike, volatility, maturity, riskFreeRate) {};
	AmericanCall(const AmericanCall& instr) : AmericanOption(instr) {};
	virtual double getOptionSign() const { return 1.; };
};

class AmericanPut : public AmericanOption
{
public:
	AmericanPut(double S0, double strike, double volatility, double maturity, double riskFreeRate) : AmericanOption(S0, strike, volatility, maturity, riskFreeRate) {};
	AmericanPut(const AmericanPut& instr) : AmericanOption(instr) {};
	virtual double getOptionSign() const { return -1.; }
};