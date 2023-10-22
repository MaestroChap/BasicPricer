#include "QuantModels.hpp"
#include "Maths.hpp"
#include <algorithm>
#include <numeric>

// Cette partie sert à calculer un intervalle de confiance poour le prix avec alpha= 0.05 et calculer le n optimal pour avoir une longuuer d'intervalle de confiance
/*
double somme = std::accumulate(A.begin(), A.end(), 0.) / m_samples.size();
for (int j = 0; j < m_samples.size(); j++)
{
	B[j] = pow(A[j] - somme, 2);
}

double var = sqrt(std::accumulate(B.begin(), B.end(), 0.) / m_samples.size()-1);

double l = 0.1;
std::cout << "On a un intervalle de confiance de 95% du prix qui est : " << "[" << somme - (var * 1.96) / sqrt(m_samples.size()) << "," << somme + (var * 1.96) / sqrt(m_samples.size()) << "]" << std::endl;
std::cout << std::endl;
std::cout << "Il faut n = " << pow((2 * var * 1.96) / l, 2) << ", pour avoir une erreur d'au plus " << l << std::endl;
*/


QuantModels::QuantModels(const double spaceStep) : m_SpaceStep(spaceStep)
{
}

MonteCarlo::MonteCarlo(const Unt timeSamples, const double spaceStep) : m_TimeSamples(timeSamples), QuantModels(spaceStep)
{
	m_Samples = std::vector<double>();
	setSamples();
}

void MonteCarlo::setSamples()
{
	if (m_Samples.empty())
	{
		for (Unt i = 0; i < m_TimeSamples; i++)
		{
			m_Samples.push_back(Maths::draw_sample_normal_standard());
		}
	}
}

BasicMonteCarlo::BasicMonteCarlo(Unt timeSamples, double spaceStep) : MonteCarlo(timeSamples, spaceStep)
{
}

double BasicMonteCarlo::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr)
{
	if (instr->hasBeenPriced())
	{
		return instr->getPrice();
	}

	std::vector<double> A(m_Samples.size(), 0);
	double payoff{ 0 };
	double S_T{ 0 };
	double S_0{ instr->getS0() };
	double vol{ instr->getVolatility() };
	double rfr{ instr->getRiskFreeRate() };
	double mat{ instr->getMaturity() };
	double K  { instr->getStrike() };

	for (int i = 0; i < m_Samples.size(); i++)
	{
		S_T = S_0 * std::exp((rfr - pow(vol, 2) * 0.5) * mat + vol * std::sqrt(mat) * m_Samples[i]); // On utilise la formule explicite de S_T pour ne pas avoir à resimuler un MB
		payoff = std::max(std::exp(-rfr * mat) * instr->getOptionSign() * (S_T - K), 0.);
		A[i] = payoff;
	}
	double price{ std::accumulate(A.begin(), A.end(), 0.) / m_Samples.size() };
	instr->setPrice(price);
	return price;
}

double BasicMonteCarlo::europeanOptionPrice(std::unique_ptr<EuropeanOption>& instr, GreekKey key)
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
	// todo gamma 
	//else if (key == GreekKey::Gamma)
	//	bumpedInstr->setS0(bumpedInstr->getR + m_SpaceStep);

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

	double delta = (priceBumpDelta - price) / m_SpaceStep;
	double vega = (priceBumpVega - price) / m_SpaceStep;
	double theta = (priceBumpTheta - price) / m_SpaceStep;
	double rho   = (priceBumpRho - price) / m_SpaceStep;

	//todo gamma
	//double gamma = (priceBump + priceDump - 2 * price) / pow(h, 2);

	// todo add result of greeks into the instrument
	res[GreekKey::Price] = price;
	res[GreekKey::Delta] = delta;
	res[GreekKey::Vega] = vega;
	res[GreekKey::Theta] = theta;
	res[GreekKey::Rho] = rho;

	return res;
}

BlackScholes::BlackScholes(double spaceStep) : QuantModels(spaceStep)
{
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

	// todo adapt the equations for a put option
	double delta = cdf1;
	double rho = K * mat * std::exp(-rfr * mat) * cdf2;
	double vega = S_0 * Maths::pdf_normal_standard(d1) * std::sqrt(mat);
	double gamma = Maths::pdf_normal_standard(d1) / (S_0 * vol * std::sqrt(mat));
	double theta = S_0 * vol * Maths::pdf_normal_standard(d1) / (2 * std::sqrt(mat)) + rfr * K * std::exp(-rfr * mat) * cdf2;

	// todo add result of greeks into the instrument
	res[GreekKey::Price] = price;
	res[GreekKey::Delta] = delta;
	res[GreekKey::Vega] = vega;
	res[GreekKey::Theta] = theta;
	res[GreekKey::Rho] = rho;

	return res;
}
	// todo remove once these equations for the put are implemented
	//double delta = -cdf1;
	//double rho = -m_K * m_T * std::exp(-m_r * m_T) * cdf2;
	//double vega = m_K * std::exp(-m_r * m_T) * pdf(d2) * std::sqrt(m_T);
	//double gamma = pdf(d1) / (m_S0 * m_sigma * std::sqrt(m_T));
	//double theta = (m_S0 * m_sigma * pdf(d1)) / (2 * std::sqrt(m_T)) - m_r * m_K * std::exp(-m_r * m_T) * cdf2;


