#pragma once

#include <iostream>
#include <unordered_map>
#include "Instruments.hpp"
#include "typedefs.hpp"


struct PricingAnalytics
{
	double price;
	//std::unordered_map<GreekKey, double> greeks;
	std::pair<double, double> intervalValues;
	double intervalPrecision; // 95% in general 
};

class QuantModels
{
public:
	QuantModels( double spaceStep) : m_SpaceStep(spaceStep) {};
	virtual struct PricingAnalytics europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr) = 0;
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key) = 0;
	virtual GreekContainer europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr) = 0;

	virtual double americanOptionPrice(std::unique_ptr<AmericanOption>& instr) = 0;
	virtual double americanOptionPrice(std::unique_ptr<AmericanOption>& instr, GreekKey key) = 0;
	virtual GreekContainer americanOptionGreeks(std::unique_ptr<AmericanOption>& instr) = 0;
protected:
	double m_SpaceStep; // for greeks computation
};

class BlackScholes : public QuantModels
{
public:
	BlackScholes(double spaceStep) : QuantModels(spaceStep) {};
	virtual struct PricingAnalytics europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr) override;
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key) override;
	virtual GreekContainer europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr) override;

	virtual double americanOptionPrice(std::unique_ptr<AmericanOption>& instr) override;
	virtual double americanOptionPrice(std::unique_ptr<AmericanOption>& instr, GreekKey key) override;
	virtual GreekContainer americanOptionGreeks(std::unique_ptr<AmericanOption>& instr) override;
};

class MonteCarlo : public QuantModels
{
public:
	MonteCarlo(Unt timeSamples, double spaceStep);
	void setSamples();
protected:
	Unt m_TimeSamples;
	std::vector<double> m_Samples; // MonteCarlo N(0, 1) samples used
};

class BasicMonteCarlo : public MonteCarlo
{
public:
	BasicMonteCarlo(Unt timeSamples, double spaceStep);
	virtual struct PricingAnalytics europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr) override;
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key) override; //Todo
	virtual GreekContainer europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr) override; //Todo

	virtual double americanOptionPrice(std::unique_ptr<AmericanOption>& instr) override; //Todo
	virtual double americanOptionPrice(std::unique_ptr<AmericanOption>& instr, GreekKey key) override; //Todo
	virtual GreekContainer americanOptionGreeks(std::unique_ptr<AmericanOption>& instr) override { GreekContainer a; return a; } //Todo
};

class CRR : public QuantModels
{
public:
	CRR(Unt step, double spaceStep) : QuantModels(spaceStep), m_step(step) {};
	virtual struct PricingAnalytics europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr) override;
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key) override; //Todo
	virtual GreekContainer europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr) override;//Todo

	virtual double americanOptionPrice(std::unique_ptr<AmericanOption>& instr) override;
	virtual double americanOptionPrice(std::unique_ptr<AmericanOption>& instr, GreekKey key) override;//Todo
	virtual GreekContainer americanOptionGreeks(std::unique_ptr<AmericanOption>& instr) override { GreekContainer a; return a; }//Todo
protected:
	Unt m_step;
};