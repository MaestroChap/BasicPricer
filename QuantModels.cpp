#include "QuantModels.hpp"
#include "Maths.hpp"
#include <algorithm>
#include <numeric>


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

double BasicMonteCarlo::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr)
{/*
	if (instr->hasBeenPriced())
	{
		return instr->getPrice();
	}
	*/
	std::vector<double> A(m_Samples.size(), 0);
	std::vector<double> B(m_Samples.size(), 0);
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

	/*
	for (int j = 0; j < m_samples.size(); j++)
	{
		B[j] = pow(A[j] - somme, 2);
	}

	double var = sqrt(std::accumulate(B.begin(), B.end(), 0.) / m_Samples.size()-1);

	std::cout << "On a un intervalle de confiance de 95% du prix qui est : " << "[" << price - (var * 1.96) / sqrt(m_samples.size()) << "," << price + (var * 1.96) / sqrt(m_Samples.size()) << "]" << std::endl;
	*/
	return price;
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
	return europeanOptionPrice(bumpedInstr);
}

GreekContainer BasicMonteCarlo::europeanOptionGreeks(std::unique_ptr<EuropeanOption>& instr)
{
	// todo add a check on the greeks like in pricing mode, and change the setter to reinitialize the greeks each time a parameter is modified
	GreekContainer res;
	double price = europeanOptionPrice(instr);
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

double BlackScholes::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr)
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
	return price;
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
	double price{ this->europeanOptionPrice(instr) };

	GreekContainer res;
	double d1 = 1 / (vol * std::sqrt(mat)) * (std::log(S_0 / K) + (rfr + std::pow(vol, 2) * 0.5) * mat);
	double d2 = d1 - vol * sqrt(mat);
	d1 *= instr->getOptionSign();
	d2 *= instr->getOptionSign();

	double cdf1 = (1.0 + std::erf(d1 / std::sqrt(2.0))) / 2.0;
	double cdf2 = (1.0 + std::erf(d2 / std::sqrt(2.0))) / 2.0;


	double delta = instr->getOptionSign()*cdf1;
	double rho = instr->getOptionSign()*K * mat * std::exp(-rfr * mat) * cdf2;
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
		instr->setGreek(element.second,	element.first);

	return res;
}