#pragma once

#include <iostream>
#include <unordered_map>
#include "Instruments.hpp"
#include "typedefs.hpp"

class QuantModels
{
public:
	QuantModels( double spaceStep) : m_SpaceStep(spaceStep) {};
	virtual PricingAnalytics setPricingAnalytics(std::unique_ptr<Option>& option) = 0;
protected:
	double m_SpaceStep; // for greeks computation
};

class BlackScholes : public QuantModels
{
public:
	BlackScholes(double spaceStep) : QuantModels(spaceStep) {};
	virtual PricingAnalytics setPricingAnalytics(std::unique_ptr<Option>& option) override;
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
	virtual PricingAnalytics setPricingAnalytics(std::unique_ptr<Option>& option);
	virtual std::pair<double, std::pair<double, double>> pricing(std::unique_ptr<Option>& option);
	virtual double pricingForGreeks(std::unique_ptr<Option>& option, std::string key);
	virtual GreekContainer greeks(std::unique_ptr<Option>& option);

};

class CRR : public QuantModels
{
public:
	CRR(Unt step, double spaceStep) : QuantModels(spaceStep), m_step(step) {};
	virtual PricingAnalytics setPricingAnalytics(std::unique_ptr<Option>& option) override;
	virtual double pricing(std::unique_ptr<Option>& option);
	virtual GreekContainer greeks(std::unique_ptr<Option>& option);

protected:
	Unt m_step;
};