#include "QuantModels.hpp"
#include "Maths.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <utility>

//Black Scholes Model
std::pair<double, double> intervalValues{ 0., 0. };


struct PricingAnalytics BlackScholes::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr)
{
	double S_T{ 0 };
	double S_0{ instr->getS0() };
	double vol{ instr->getVolatility() };
	double rfr{ instr->getRiskFreeRate() };
	double mat{ instr->getMaturity() };
	double K{ instr->getStrike() };

	double d1 = 1 / (vol * std::sqrt(mat)) * (std::log(S_0 / K) + (rfr + std::pow(vol, 2) * 0.5) * mat);
	double d2 = d1 - vol * sqrt(mat);
	d1 *= instr->getOptionSign();
	d2 *= instr->getOptionSign();

	double cdf1 = (1.0 + std::erf(d1 / std::sqrt(2.0))) / 2.0;
	double cdf2 = (1.0 + std::erf(d2 / std::sqrt(2.0))) / 2.0;

	double price = instr->getOptionSign() * (S_0 * cdf1 - K * std::exp(-rfr * mat) * cdf2);
	instr->setPrice(price);

	std::pair<double, double> intervalValues{ 0., 0. };

	struct PricingAnalytics ConfidenceInterval { price, intervalValues, 0.95 };

	return ConfidenceInterval;
}

double BlackScholes::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key)
{
	// todo
	return 0.0;
}


GreekContainer BlackScholes::europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr)
{
	double S_T{ 0 };
	double S_0{ instr->getS0() };
	double vol{ instr->getVolatility() };
	double rfr{ instr->getRiskFreeRate() };
	double mat{ instr->getMaturity() };
	double K{ instr->getStrike() };
	double price{ this->europeanOptionPrice(instr).price };

	GreekContainer res;
	double d1 = 1 / (vol * std::sqrt(mat)) * (std::log(S_0 / K) + (rfr + std::pow(vol, 2) * 0.5) * mat);
	double d2 = d1 - vol * sqrt(mat);
	d1 *= instr->getOptionSign();
	d2 *= instr->getOptionSign();

	double cdf1 = (1.0 + std::erf(d1 / std::sqrt(2.0))) / 2.0;
	double cdf2 = (1.0 + std::erf(d2 / std::sqrt(2.0))) / 2.0;


	double delta = instr->getOptionSign() * cdf1;
	double rho = instr->getOptionSign() * K * mat * std::exp(-rfr * mat) * cdf2;
	double vega = S_0 * Maths::pdf_normal_standard(d1) * std::sqrt(mat);
	double gamma = Maths::pdf_normal_standard(d1) / (S_0 * vol * std::sqrt(mat));
	double theta = S_0 * vol * Maths::pdf_normal_standard(d1) / (2 * std::sqrt(mat)) + instr->getOptionSign() * rfr * K * std::exp(-rfr * mat) * cdf2;

	res[GreekKey::Price] = price;
	res[GreekKey::Delta] = delta;
	res[GreekKey::Vega] = vega;
	res[GreekKey::Theta] = theta;
	res[GreekKey::Rho] = rho;
	res[GreekKey::Gamma] = gamma;

	for (auto& element : res)
		instr->setGreek(element.second, element.first);

	return res;
}

double BlackScholes::americanOptionPrice(std::unique_ptr<AmericanOption>& instr)
{
	instr->setPrice(UNDEFINITE_TOKEN);
	return instr->getPrice();
}

double BlackScholes::americanOptionPrice(std::unique_ptr<AmericanOption>& instr, GreekKey key)
{
	instr->setGreek(UNDEFINITE_TOKEN,key);
	instr->setPrice(UNDEFINITE_TOKEN);
	return instr->getPrice();
}

GreekContainer BlackScholes::americanOptionGreeks(std::unique_ptr<AmericanOption>& instr)
{
	instr->setGreek(UNDEFINITE_TOKEN, GreekKey::Delta);
	instr->setGreek(UNDEFINITE_TOKEN, GreekKey::Vega);
	instr->setGreek(UNDEFINITE_TOKEN, GreekKey::Theta);
	instr->setGreek(UNDEFINITE_TOKEN, GreekKey::Rho);
	instr->setGreek(UNDEFINITE_TOKEN, GreekKey::Gamma);

	return instr->getGreek();
}

//Monte Carlo

MonteCarlo::MonteCarlo(const Unt timeSamples, const double spaceStep) : m_TimeSamples(timeSamples), QuantModels(spaceStep)
{
	m_Samples = std::vector<double>(m_TimeSamples,0);
	setSamples();
}

void MonteCarlo::setSamples()
{
	for (Unt i = 0; i < m_TimeSamples; i++)
	{
		m_Samples[i] = Maths::draw_sample_normal_standard();
	}
}

BasicMonteCarlo::BasicMonteCarlo(Unt timeSamples, double spaceStep) : MonteCarlo(timeSamples, spaceStep)
{
}

struct PricingAnalytics BasicMonteCarlo::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr)
{
	std::vector<double> A(m_Samples.size(), 0);
	std::vector<double> B(m_Samples.size(), 0);
	std::pair<double, double> intervalValues{ 0., 0. };

	double payoff{ 0 };
	double S_T{ 0 };
	double S_0{ instr->getS0() };
	double vol{ instr->getVolatility() };
	double rfr{ instr->getRiskFreeRate() };
	double mat{ instr->getMaturity() };
	double K{ instr->getStrike() };

	for (Unt i = 0; i < m_Samples.size(); i++)
	{
		S_T = S_0 * std::exp((rfr - pow(vol, 2) * 0.5) * mat + vol * std::sqrt(mat) * m_Samples[i]); // Explicit S_T formula
		payoff = std::max(std::exp(-rfr * mat) * instr->getOptionSign() * (S_T - K), 0.);
		A[i] = payoff;
	}
	double price{ std::accumulate(A.begin(), A.end(), 0.) / m_Samples.size() };
	instr->setPrice(price);

	for (Unt i = 0; i < m_Samples.size(); i++)
	{
		B[i] = pow(A[i] - price, 2);
	}

	double std = sqrt(std::accumulate(B.begin(), B.end(), 0.) / m_Samples.size() - 1);

	intervalValues.first = price - (std * Maths::inverseNormalCDF((1 - (1 - 0.95) / 2.)))/ sqrt(m_Samples.size());
	intervalValues.second = price + (std * Maths::inverseNormalCDF((1 - (1 - 0.95) / 2.))) / sqrt(m_Samples.size());

	struct PricingAnalytics ConfidenceInterval { price, intervalValues, 0.95 };

	return ConfidenceInterval;
}

double BasicMonteCarlo::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key) // Change function name ? 
{
	// this function prices the product after bumping the value corresponding to the key by h

	std::unique_ptr<EuropeanOption> bumpedInstr(instr->DeepClone());
	if (key == GreekKey::Delta)
		bumpedInstr->setS0(bumpedInstr->getS0() + m_SpaceStep);
	else if (key == GreekKey::Vega)
		bumpedInstr->setVolatility(bumpedInstr->getVolatility() + m_SpaceStep);
	else if (key == GreekKey::Rho)
		bumpedInstr->setRiskFreeRate(bumpedInstr->getRiskFreeRate() + m_SpaceStep);
	else if (key == GreekKey::Theta)
		bumpedInstr->setMaturity(bumpedInstr->getMaturity() + m_SpaceStep);
	else if (key == GreekKey::Gamma)
		bumpedInstr->setS0(bumpedInstr->getS0() + m_SpaceStep);
	return europeanOptionPrice(bumpedInstr).price;
}

GreekContainer BasicMonteCarlo::europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr)
{
	// todo add a check on the greeks like in pricing mode, and change the setter to reinitialize the greeks each time a parameter is modified
	GreekContainer res;
	double price = europeanOptionPrice(instr).price;
	double priceBumpDelta = europeanOptionPrice(instr, GreekKey::Delta);
	double priceBumpVega = europeanOptionPrice(instr, GreekKey::Vega);
	double priceBumpTheta = europeanOptionPrice(instr, GreekKey::Theta);
	double priceBumpRho = europeanOptionPrice(instr, GreekKey::Rho);
	double priceBumpGamma = europeanOptionPrice(instr, GreekKey::Gamma);

	m_SpaceStep = -m_SpaceStep;
	double priceDumpGamma = europeanOptionPrice(instr, GreekKey::Gamma); //False gamma
	m_SpaceStep = -m_SpaceStep;

	double delta = (priceBumpDelta - price) / m_SpaceStep;
	double vega = (priceBumpVega - price) / m_SpaceStep;
	double theta = (priceBumpTheta - price) / m_SpaceStep;
	double rho = (priceBumpRho - price) / m_SpaceStep;
	double gamma = (priceBumpGamma + priceDumpGamma - 2 * price) / pow(m_SpaceStep, 2);

	res[GreekKey::Price] = price;
	res[GreekKey::Delta] = delta;
	res[GreekKey::Vega] = vega;
	res[GreekKey::Theta] = theta;
	res[GreekKey::Rho] = rho;
	res[GreekKey::Gamma] = gamma;

	for (auto& element : res)
		instr->setGreek(element.second, element.first);
	return res;
}

double BasicMonteCarlo::americanOptionPrice(std::unique_ptr<AmericanOption>& instr)
{
	return 0.;
}

double BasicMonteCarlo::americanOptionPrice(std::unique_ptr<AmericanOption>& instr, GreekKey key)
{
	return 0;
}

//Cox Ross Rubinstein model ----------------------

struct PricingAnalytics CRR::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr)
{
	std::pair<double, double> intervalValues{ 0., 0. };
	double S_0{ instr->getS0() };
	double K{ instr->getStrike() };
	double vol{ instr->getVolatility() };
	double rfr{ instr->getRiskFreeRate() };
	double mat{ instr->getMaturity() };

	double u = exp(vol * std::sqrt(mat / m_step));
	double d = exp(-vol * std::sqrt(mat / m_step));
	double p = (exp(rfr * (mat / m_step)) - d) / (u - d);

	std::vector<double> U;
	for (int i = 0; i < m_step + 1; i++)
	{
		U.push_back(std::max(instr->getOptionSign() * (S_0 * pow(u, m_step - i) * pow(d, i) - K), 0.));
	}
	for (int i = m_step - 1; i > -1; i--)
	{
		for (int j = 0; j < i + 1; j++)
		{
			U[j] = instr->getOptionSign() * exp(-rfr * mat / m_step) * (p * U[j] + (1 - p) * U[j + 1]);
		}
		U.pop_back();
	}
	instr->setPrice(U[0]);

	struct PricingAnalytics ConfidenceInterval { U[0], intervalValues, 0.95 };

	return ConfidenceInterval;
}

double CRR::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key)
{
	std::unique_ptr<EuropeanOption> bumpedInstr(instr->DeepClone());
	if (key == GreekKey::Delta)
		bumpedInstr->setS0(bumpedInstr->getS0() + m_SpaceStep);
	else if (key == GreekKey::Vega)
		bumpedInstr->setVolatility(bumpedInstr->getVolatility() + m_SpaceStep);
	else if (key == GreekKey::Rho)
		bumpedInstr->setRiskFreeRate(bumpedInstr->getRiskFreeRate() + m_SpaceStep);
	else if (key == GreekKey::Theta)
		bumpedInstr->setMaturity(bumpedInstr->getMaturity() + m_SpaceStep);
	else if (key == GreekKey::Gamma)
		bumpedInstr->setS0(bumpedInstr->getS0() + m_SpaceStep);
	return europeanOptionPrice(bumpedInstr).price;
}

GreekContainer CRR::europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr)
{
	GreekContainer a;
	return a;
}

double CRR::americanOptionPrice(std::unique_ptr<AmericanOption>& instr)
{
	std::pair<double, double> intervalValues{ 0., 0. };

	double S_0{ instr->getS0() };
	double K{ instr->getStrike() };
	double vol{ instr->getVolatility() };
	double rfr{ instr->getRiskFreeRate() };
	double mat{ instr->getMaturity() };

	double u = exp(vol * std::sqrt(mat / m_step));
	double d = exp(-vol * std::sqrt(mat / m_step));
	double p = (exp(rfr * (mat / m_step)) - d) / (u - d);

	std::vector<double> U;
	for (int i = 0; i < m_step + 1; i++)
	{
		U.push_back(std::max(instr->getOptionSign() * (S_0 * pow(u, m_step - i) * pow(d, i) - K), 0.));
	}
	for (int i = m_step - 1; i > -1; i--)
	{
		for (int j = 0; j < i + 1; j++)
		{
			U[j] = std::max(std::max(instr->getOptionSign() * (S_0 * pow(u, i - j) * pow(d, j) - K), 0.), exp(-rfr * mat / m_step) * (p * U[j] + (1 - p) * U[j + 1]));
		}
		U.pop_back();
	}
	instr->setPrice(U[0]);

	struct PricingAnalytics ConfidenceInterval { U[0], intervalValues, 0.95 };

	return ConfidenceInterval.price;
}

double CRR::americanOptionPrice(std::unique_ptr<AmericanOption>& instr, GreekKey key)
{
	return 0;
}

