#pragma once

#include <iostream>
#include <unordered_map>
#include "Instruments.hpp"
#include "typedefs.hpp"

class QuantModels
{
//friend class EuropeanOption;
public:
	QuantModels( double spaceStep) : m_SpaceStep(spaceStep) {};
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr) = 0;
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key) = 0;
	virtual GreekContainer europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr) = 0;

protected:
	double m_SpaceStep; // for greeks computation
};

class BlackScholes : public QuantModels
{
public:
	BlackScholes(double spaceStep) : QuantModels(spaceStep) {} ;
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr) override;
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key) override;
	virtual GreekContainer europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr) override;
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
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr) override;
	virtual double europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key) override;
	virtual GreekContainer europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr) override;
};