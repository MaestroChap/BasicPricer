#define _USE_MATH_DEFINES
#include "MonteCarlo.hpp"
#include <random>
#include <algorithm>
#include <numeric>

namespace MonteCarlo
{
	MonteCarlo::MonteCarlo() : m_S0(0), m_K(0), m_r(0), m_T(0), m_sigma(0), m_samples(0)
	{

	}

	MonteCarlo::~MonteCarlo()
	{

	}


	// Fonctions utilisées pour le calcul 

	double pdf(double x)
	{
		return (1 / (std::sqrt(2 * M_PI))) * std::exp(-0.5 * x * x);
	}

	double loi_normale_standard()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::normal_distribution<double> distribution(0, 1);

		double a = distribution(gen);

		return a;
	}

	//Classe Call Europeen 

	CallEuro::CallEuro(double prixDeBase, double strike, double taux, double temps, double sigma, int N)
	{
		setS(prixDeBase);
		setK(strike);
		setR(taux);
		setT(temps);
		setSigma(sigma);
		setSample(N);
	}

	CallEuro::~CallEuro()
	{

	}
	
	//Setters

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

	void CallEuro::setSample(int N)
	{

		std::vector<double> samples;

		for (int i = 0; i < N; i++)
		{
			samples.push_back(loi_normale_standard());
		}
		m_samples = samples;

	}




	double CallEuro::optionPrice() 
	{
		std::vector<double> A(m_samples.size(), 0);
		std::vector<double> B(m_samples.size(), 0);

		double payoff{ 0 };
		double S_T{ 0 };

		for (int i = 0; i < m_samples.size(); i++)
		{
			S_T = m_S0 * std::exp((m_r - pow(m_sigma, 2) * 0.5) * m_T + m_sigma * std::sqrt(m_T) * m_samples[i]); // On utilise la formule explicite de S_T pour ne pas avoir à resimuler un MB
			payoff = std::max(std::exp(-m_r * m_T) * (S_T - m_K), 0.);
			A[i] = payoff;

		}

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

		return std::accumulate(A.begin(), A.end(), 0.)/ m_samples.size();

	}

	std::unordered_map<std::string, double> CallEuro::optionGrecques(double h)
	{

		std::unordered_map<std::string, double> grecques;
		double S_T{ 0 };

		double price = optionPrice();

		m_S0 += h;

		double priceBump = optionPrice();
		double delta = (priceBump - price) / h;

		grecques["delta"] = delta;

		m_S0 -= h;
		m_r += h;

		priceBump = optionPrice();

		double rho = (priceBump - price) / h;

		grecques["rho"] = rho;

		m_r -= h;
		m_sigma += h;

		priceBump = optionPrice();

		double vega = (priceBump - price) / h;

		grecques["vega"] = vega;


		m_sigma -= h;
		m_S0 += h;

		priceBump = optionPrice();
		m_S0 -= 2 * h;

		double priceDump = optionPrice();
		double gamma = (priceBump + priceDump - 2 * price) / pow(h, 2);

		grecques["gamma"] = gamma;

		m_S0 += h;
		m_T += h;

		priceBump = optionPrice();
		double theta = (priceBump - price) / h;

		grecques["theta"] = theta;

		return grecques;
	}


	// PUT EUROPEEN 

	PutEuro::PutEuro(double prixDeBase, double strike, double taux, double temps, double sigma, int N)
	{
		setS(prixDeBase);
		setK(strike);
		setR(taux);
		setT(temps);
		setSigma(sigma);
		setSample(N);
	}

	PutEuro::~PutEuro()
	{

	}

	//Setters

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

	void PutEuro::setSample(int N)
	{

		std::vector<double> samples;

		for (int i = 0; i < N; i++)
		{
			samples.push_back(loi_normale_standard());
		}
		m_samples = samples;

	}




	double PutEuro::optionPrice()
	{
		std::vector<double> A(m_samples.size(), 0);

		double payoff{ 0 };
		double S_T{ 0 };

		for (int i = 0; i < m_samples.size(); i++)
		{
			S_T = m_S0 * std::exp((m_r - pow(m_sigma, 2) * 0.5) * m_T + m_sigma * std::sqrt(m_T) * m_samples[i]); // On utilise la formule explicite de S_T pour ne pas avoir à resimuler un MB
			payoff = std::max(std::exp(-m_r * m_T) * (m_K - S_T), 0.);
			A[i] = payoff;

		}


		return std::accumulate(A.begin(), A.end(), 0.) / m_samples.size();

	}

	std::unordered_map<std::string, double> PutEuro::optionGrecques(double h)
	{

		std::unordered_map<std::string, double> grecques;
		double S_T{ 0 };

		double price = optionPrice();

		m_S0 += h;

		double priceBump = optionPrice();
		double delta = (priceBump - price) / h;

		grecques["delta"] = delta;

		m_S0 -= h;
		m_r += h;

		priceBump = optionPrice();

		double rho = (priceBump - price) / h;

		grecques["rho"] = rho;

		m_r -= h;
		m_sigma += h;

		priceBump = optionPrice();

		double vega = (priceBump - price) / h;

		grecques["vega"] = vega;


		m_sigma -= h;
		m_S0 += h;

		priceBump = optionPrice();
		m_S0 -= 2 * h;

		double priceDump = optionPrice();
		double gamma = (priceBump + priceDump - 2 * price) / pow(h, 2);

		grecques["gamma"] = gamma;

		m_S0 += h;
		m_T += h;

		priceBump = optionPrice();
		double theta = (priceBump - price) / h;

		grecques["theta"] = theta;

		return grecques;
	}


}