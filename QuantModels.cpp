#include "QuantModels.hpp"
#include "Maths.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <utility>


PricingAnalytics BlackScholes::setPricingAnalytics(std::unique_ptr<Option>& option)
{
	PricingAnalytics p;
	GreekContainer res;
	EuropeanOption* euroOption = dynamic_cast<EuropeanOption*>(option.get());

	if (euroOption)
	{
		// Compute the price
		double S_T{ 0 };
		double S_0{ option->getS0() };
		double vol{ option->getVolatility() };
		double rfr{ option->getRiskFreeRate() };
		double mat{ option->getMaturity() };
		double K{ option->getStrike() };

		double d1 = 1 / (vol * std::sqrt(mat)) * (std::log(S_0 / K) + (rfr + std::pow(vol, 2) * 0.5) * mat);
		double d2 = d1 - vol * sqrt(mat);
		d1 *= option->getOptionSign();
		d2 *= option->getOptionSign();

		double cdf1 = (1.0 + std::erf(d1 / std::sqrt(2.0))) / 2.0;
		double cdf2 = (1.0 + std::erf(d2 / std::sqrt(2.0))) / 2.0;

		double price = option->getOptionSign() * (S_0 * cdf1 - K * std::exp(-rfr * mat) * cdf2);
		option->setPrice(price);
		p.price = price;

		// Compute the greeks
		double delta = option->getOptionSign() * cdf1;
		double rho = option->getOptionSign() * K * mat * std::exp(-rfr * mat) * cdf2;
		double vega = S_0 * Maths::pdf_normal_standard(d1) * std::sqrt(mat);
		double gamma = Maths::pdf_normal_standard(d1) / (S_0 * vol * std::sqrt(mat));
		double theta = S_0 * vol * Maths::pdf_normal_standard(d1) / (2 * std::sqrt(mat)) + option->getOptionSign() * rfr * K * std::exp(-rfr * mat) * cdf2;

		res["Delta"] = delta;
		res["Rho"] = rho;
		res["Vega"] = vega;
		res["Theta"] = theta;
		res["Gamma"] = gamma;

		for (auto& element : res)
			option->setGreek(element.first, element.second);
		p.greeks = res;

		return p;
	}
	else
	{
		double price = UNDEFINITE_TOKEN;
		option->setPrice( UNDEFINITE_TOKEN );
		p.price = price;

		res["Delta"] = UNDEFINITE_TOKEN;
		res["Vega"] = UNDEFINITE_TOKEN;
		res["Theta"] = UNDEFINITE_TOKEN;
		res["Rho"] = UNDEFINITE_TOKEN;
		res["Gamma"] = UNDEFINITE_TOKEN;
		p.greeks = res;

		return p;
	}
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


std::pair<double,std::pair<double,double>> BasicMonteCarlo::pricing(std::unique_ptr<Option>& option)
{
	std::pair<double, std::pair<double, double>> pair_gen;
	std::pair<double, double> IntervalValues;
	EuropeanOption* euroOption = dynamic_cast<EuropeanOption*>(option.get());

	if (euroOption)
	{
		std::vector<double> A(m_Samples.size(), 0);
		std::vector<double> B(m_Samples.size(), 0);
		std::pair<double, double> intervalValues{ 0., 0. };

		double payoff{ 0 };
		double S_T{ 0 };
		double S_0{ option->getS0() };
		double vol{ option->getVolatility() };
		double rfr{ option->getRiskFreeRate() };
		double mat{ option->getMaturity() };
		double K{ option->getStrike() };

		for (Unt i = 0; i < m_Samples.size(); i++)
		{
			S_T = S_0 * std::exp((rfr - pow(vol, 2) * 0.5) * mat + vol * std::sqrt(mat) * m_Samples[i]); // Explicit S_T formula
			payoff = std::max(std::exp(-rfr * mat) * option->getOptionSign() * (S_T - K), 0.);
			A[i] = payoff;
		}
		double price{ std::accumulate(A.begin(), A.end(), 0.) / m_Samples.size() };
		option->setPrice(price);
		pair_gen.first = price;
		
		for (Unt i = 0; i < m_Samples.size(); i++)
		{
			B[i] = pow(A[i] - price, 2);
		}

		double std = sqrt(std::accumulate(B.begin(), B.end(), 0.) / m_Samples.size() - 1);

		intervalValues.first = price - (std * Maths::inverseNormalCDF((1 - (1 - 0.95) / 2.))) / sqrt(m_Samples.size());
		intervalValues.second = price + (std * Maths::inverseNormalCDF((1 - (1 - 0.95) / 2.))) / sqrt(m_Samples.size());

		pair_gen.second = intervalValues;

		return pair_gen;
	}
	else
	{
		return pair_gen;
	}
}

double BasicMonteCarlo::pricingForGreeks(std::unique_ptr<Option>& option, std::string key)
{
	// this function prices the product after bumping the value corresponding to the key by h

	std::unique_ptr<Option> bumpedInstr(option->DeepClone());
	if (key == "Delta")
		bumpedInstr->setS0(bumpedInstr->getS0() + m_SpaceStep);
	else if (key == "Vega")
		bumpedInstr->setVolatility(bumpedInstr->getVolatility() + m_SpaceStep);
	else if (key == "Theta")
		bumpedInstr->setMaturity(bumpedInstr->getMaturity() + m_SpaceStep);
	else if (key == "Rho")
		bumpedInstr->setRiskFreeRate(bumpedInstr->getRiskFreeRate() + m_SpaceStep);
	else if (key == "Gamma")
		bumpedInstr->setS0(bumpedInstr->getS0() + m_SpaceStep);
	return pricing(bumpedInstr).first;
}

GreekContainer BasicMonteCarlo::greeks(std::unique_ptr<Option>& option)
{
	GreekContainer res;
	std::unique_ptr<Option> bumpedInstr(option->DeepClone());

	double price = pricing(option).first;
	double priceBumpDelta = pricingForGreeks(option, "Delta");
	double priceBumpVega = pricingForGreeks(option, "Vega");
	double priceBumpTheta = pricingForGreeks(option, "Theta");
	double priceBumpRho = pricingForGreeks(option, "Rho");
	double priceBumpGamma = pricingForGreeks(option, "Gamma");

	m_SpaceStep = -m_SpaceStep;
	double priceDumpGamma = pricingForGreeks(option, "Gamma"); //False gamma
	m_SpaceStep = -m_SpaceStep;

	double delta = (priceBumpDelta - price) / m_SpaceStep;
	double vega = (priceBumpVega - price) / m_SpaceStep;
	double theta = (priceBumpTheta - price) / m_SpaceStep;
	double rho = (priceBumpRho - price) / m_SpaceStep;
	double gamma = (priceBumpGamma + priceDumpGamma - 2 * price) / pow(m_SpaceStep, 2);

	res["Delta"] = delta;
	res["Vega"] = vega;
	res["Theta"] = theta;
	res["Rho"] = rho;
	res["Gamma"] = gamma;

	for (auto& element : res)
		option->setGreek(element.first, element.second);
	return res;
}

PricingAnalytics BasicMonteCarlo::setPricingAnalytics(std::unique_ptr<Option>& option)
{
	PricingAnalytics pricingAnalytics;

	pricingAnalytics.price = pricing(option).first;
	pricingAnalytics.greeks = greeks(option);

	return pricingAnalytics;
}

//Cox Ross Rubinstein model ----------------------

std::pair<double, GreekContainer> CRR::pricing(std::unique_ptr<Option>& option)
{
	std::pair<double, GreekContainer> pair_gen;
	GreekContainer greeks;
	EuropeanOption* euroOption = dynamic_cast<EuropeanOption*>(option.get());
	if (euroOption)
	{

		double S_0{ option->getS0() };
		double K{ option->getStrike() };
		double vol{ option->getVolatility() };
		double rfr{ option->getRiskFreeRate() };
		double mat{ option->getMaturity() };

		double u = exp(vol * std::sqrt(mat / m_step));
		double d = exp(-vol * std::sqrt(mat / m_step));
		double p = (exp(rfr * (mat / m_step)) - d) / (u - d);

		double price = 0;
		double delta = 0;
		double gamma = 0;
		double f11 = 0;
		double f10 = 0;
		double f22 = 0;
		double f21 = 0;
		double f20 = 0;

		std::vector<double> U;
		for (int i = 0; i < m_step + 1; i++)
		{
			U.push_back(std::max(option->getOptionSign() * (S_0 * pow(u, m_step - i) * pow(d, i) - K), 0.));
		}
		for (int i = m_step - 1; i > -1; i--)
		{
			if (i == 0)
			{
				f11 = U[0];
				f10 = U[1];
			}
			if (i == 1)
			{
				f22 = U[0];
				f21 = U[1];
				f20 = U[2];
			}
			for (int j = 0; j < i + 1; j++)
			{
				U[j] = option->getOptionSign() * exp(-rfr * mat / m_step) * (p * U[j] + (1 - p) * U[j + 1]);
			}
			U.pop_back();
		}
		price = U[0];
		delta = (f11 - f10) / (S_0 * u - S_0 * d);
		gamma = ((f22 - f21) / (S_0 * pow(u, 2) - S_0) - ((f21 - f20) / (S_0 - S_0 * pow(d, 2)))) * (1 / (0.5 * (S_0 * pow(u, 2) - S_0 * pow(d, 2))));

		option->setPrice(price);

		pair_gen.first = price;

		greeks["Delta"] = delta;
		greeks["Gamma"] = gamma;
		//dic["Theta"] = theta;
		pair_gen.second = greeks;

		return pair_gen;
	}
	else
	{
		std::pair<double, GreekContainer> pair_gen;
		GreekContainer greeks;
		double S_0{ option->getS0() };
		double K{ option->getStrike() };
		double vol{ option->getVolatility() };
		double rfr{ option->getRiskFreeRate() };
		double mat{ option->getMaturity() };

		double u = exp(vol * std::sqrt(mat / m_step));
		double d = exp(-vol * std::sqrt(mat / m_step));
		double p = (exp(rfr * (mat / m_step)) - d) / (u - d);

		//Grecques

		double price = 0;
		double delta = 0;
		double gamma = 0;
		double f11 = 0;
		double f10 = 0;
		double f22 = 0;
		double f21 = 0;
		double f20 = 0;

		std::vector<double> U;
		for (int i = 0; i < m_step + 1; i++)
		{
			U.push_back(std::max(option->getOptionSign() * (S_0 * pow(u, m_step - i) * pow(d, i) - K), 0.));
		}
		
		for (int i = m_step - 1; i > -1; i--)
		{
			if (i == 0)
			{
				f11 = U[0];
				f10 = U[1];
			}
			if (i == 1)
			{
				f22 = U[0];
				f21 = U[1];
				f20 = U[2];
			}
			for (int j = 0; j < i + 1; j++)
			{
				U[j] = std::max(std::max(option->getOptionSign() * (S_0 * pow(u, i - j) * pow(d, j) - K), 0.), exp(-rfr * mat / m_step) * (p * U[j] + (1 - p) * U[j + 1]));
			}
			U.pop_back();
		}
		price = U[0];
		delta = (f11 - f10) / (S_0 * u - S_0 * d);
		gamma = ((f22 - f21) / (S_0 * pow(u, 2) - S_0) - ((f21 - f20) / (S_0 - S_0 * pow(d, 2)))) * (1 / (0.5 * (S_0 * pow(u, 2) - S_0 * pow(d, 2))));

		option->setPrice(price);

		pair_gen.first = price;

		greeks["Delta"] = delta;
		greeks["Gamma"] = gamma;
		//dic["Theta"] = theta;
		pair_gen.second = greeks;
		return pair_gen;
	}
}

GreekContainer CRR::greeks(std::unique_ptr<Option>& option) //To do
{
	GreekContainer a;
	return a;
}

PricingAnalytics CRR::setPricingAnalytics(std::unique_ptr<Option>& option)
{
	PricingAnalytics pricingAnalytics;
	std::pair<double, GreekContainer> res;
	res = pricing(option);
	pricingAnalytics.price = res.first;
	for (auto& element : greeks(option))
	{
		res.second[element.first] = element.second;
	}
	pricingAnalytics.greeks = res.second;

	return pricingAnalytics;
}