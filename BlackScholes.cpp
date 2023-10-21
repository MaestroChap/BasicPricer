#define _USE_MATH_DEFINES
#include "BlackScholes.hpp"



namespace BlackScholes{

	BlackScholes::BlackScholes() : m_S0(0), m_K(0), m_r(0), m_T(0), m_sigma(0)
	{
		
	}
	BlackScholes::~BlackScholes()
	{

	}

	// CALL EUROPEEN 

	CallEuro::CallEuro(double prixDeBase, double K, double taux, double temps, double sigma)
	{
		setS(prixDeBase);
		setK(K);
		setR(taux);
		setT(temps);
		setSigma(sigma);
	}

	CallEuro::~CallEuro()
	{

	}

	void CallEuro::setS(double prixDeBase)
	{
		m_S0 = prixDeBase;
	}

	void CallEuro::setR(double taux)
	{
		m_r = taux;
	}

	void CallEuro::setK(double strike)
	{
		m_K = strike;
	}

	void CallEuro::setT(double temps)
	{
		m_T = temps;
	}

	void CallEuro::setSigma(double sigma)
	{
		m_sigma = sigma;
	}

	

	double pdf(double x)
	{
		return (1 / (std::sqrt(2 * M_PI))) * std::exp(-0.5 * x * x);
	}


	double CallEuro::optionPrice()
	{
		double d1 = 1 / (m_sigma * std::sqrt(m_T)) * (std::log(m_S0 / m_K) + (m_r + std::pow(m_sigma, 2) * 0.5) * m_T);
		double d2 = d1 - m_sigma * sqrt(m_T);

		double cdf1 = (1.0 + std::erf(d1 / std::sqrt(2.0))) / 2.0;
		double cdf2 = (1.0 + std::erf(d2 / std::sqrt(2.0))) / 2.0;

		double prix = m_S0 * cdf1 - m_K * std::exp(-m_r * m_T) * cdf2;

		return prix;
	}
	
	std::unordered_map<std::string, double> CallEuro::optionGrecques()
	{
		std::unordered_map<std::string, double> grecques;
		double d1 = 1 / (m_sigma * std::sqrt(m_T)) * (std::log(m_S0 / m_K) + (m_r + std::pow(m_sigma, 2) * 0.5) * m_T);
		double d2 = d1 - m_sigma * sqrt(m_T);

		double cdf1 = (1.0 + std::erf(d1 / std::sqrt(2.0))) / 2.0;
		double cdf2 = (1.0 + std::erf(d2 / std::sqrt(2.0))) / 2.0;

		double delta = cdf1;
		double rho = m_K * m_T * std::exp(-m_r * m_T) * cdf2;
		double vega = m_S0 * pdf(d1) * std::sqrt(m_T);
		double gamma = pdf(d1) / (m_S0 * m_sigma * std::sqrt(m_T));
		double theta = m_S0 * m_sigma * pdf(d1) / (2 * std::sqrt(m_T)) + m_r * m_K * std::exp(-m_r * m_T) * cdf2;

		grecques["delta"] = delta;
		grecques["rho"] = rho;
		grecques["vega"] = vega;
		grecques["gamma"] = gamma;
		grecques["theta"] = theta;

		return grecques;
		
	}


	// PUT EUROPEEN


	PutEuro::PutEuro(double prixDeBase, double K, double taux, int temps, double sigma)
	{
		setS(prixDeBase);
		setK(K);
		setR(taux);
		setT(temps);
		setSigma(sigma);
	}

	PutEuro::~PutEuro()
	{

	}

	void PutEuro::setS(double prixDeBase)
	{
		m_S0 = prixDeBase;
	}

	void PutEuro::setR(double taux)
	{
		m_r = taux;
	}

	void PutEuro::setK(double strike)
	{
		m_K = strike;
	}

	void PutEuro::setT(double temps)
	{
		m_T = temps;
	}

	void PutEuro::setSigma(double sigma)
	{
		m_sigma = sigma;
	}

	double PutEuro::optionPrice()
	{
		double d1 = 1 / (m_sigma * std::sqrt(m_T)) * (std::log(m_S0 / m_K) + (m_r + std::pow(m_sigma, 2) * 0.5) * m_T);
		double d2 = d1 - m_sigma * sqrt(m_T);

		double cdf1 = (1.0 + std::erf(-d1 / std::sqrt(2.0))) / 2.0;
		double cdf2 = (1.0 + std::erf(-d2 / std::sqrt(2.0))) / 2.0;

		return -m_S0 * cdf1 + m_K * std::exp(-m_r * m_T) * cdf2;

	}

	std::unordered_map<std::string, double> PutEuro::optionGrecques()
	{
		std::unordered_map<std::string, double> grecques;

		double d1 = 1 / (m_sigma * std::sqrt(m_T)) * (std::log(m_S0 / m_K) + (m_r + std::pow(m_sigma, 2) * 0.5) * m_T);
		double d2 = d1 - m_sigma * sqrt(m_T);

		double cdf1 = (1.0 + std::erf(-d1 / std::sqrt(2.0))) / 2.0;
		double cdf2 = (1.0 + std::erf(-d2 / std::sqrt(2.0))) / 2.0;

		double delta = -cdf1;
		double rho = - m_K * m_T * std::exp(-m_r * m_T) * cdf2;
		double vega = m_K * std::exp(-m_r*m_T)*pdf(d2) * std::sqrt(m_T);
		double gamma = pdf(d1) / (m_S0 * m_sigma * std::sqrt(m_T));
		double theta = (m_S0 * m_sigma * pdf(d1)) / (2 * std::sqrt(m_T)) - m_r * m_K * std::exp(-m_r * m_T) * cdf2;

		grecques["delta"] = delta;
		grecques["rho"] = rho;
		grecques["vega"] = vega;
		grecques["gamma"] = gamma;
		grecques["theta"] = theta;

		return grecques;


	}



}