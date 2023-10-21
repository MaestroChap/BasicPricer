#ifndef __BLACKSCHOLES__

	#define __BLACKSCHOLES__

	#include <vector>
	#include <iostream>
	#include <unordered_map>

namespace BlackScholes{

	class BlackScholes
	{

	public:

		BlackScholes();
		virtual ~BlackScholes();

		virtual double optionPrice() = 0;

		//Setters
		virtual void setS(double prixDeBase) = 0;
		virtual void setK(double strike) = 0;
		virtual void setR(double taux) = 0;
		virtual void setSigma(double sigma) = 0;
		virtual void setT(double temps) = 0;

	protected:

		double m_S0, m_K, m_r, m_sigma, m_T;

	};

	class CallEuro : public BlackScholes
	{

	public:

		CallEuro(double prixDeBase, double K, double taux, double temps, double sigma);
		virtual ~CallEuro();
		virtual void setS(double prixDeBase);
		virtual void setK(double strike);
		virtual void setR(double taux);
		virtual void setSigma(double sigma);
		virtual void setT(double temps);
		virtual double optionPrice();
		virtual std::unordered_map<std::string, double> optionGrecques();

	protected:

		double m_K;
		double m_T;
		double m_S0;
		double m_r;
		double m_sigma;
	};


	class PutEuro : public BlackScholes
	{
	public:

		PutEuro(double prixDeBase, double K, double taux, int temps, double sigma);
		virtual ~PutEuro();
		virtual void setS(double prixDeBase);
		virtual void setK(double strike);
		virtual void setR(double taux);
		virtual void setSigma(double sigma);
		virtual void setT(double temps);
		virtual double optionPrice();
		virtual std::unordered_map<std::string, double> optionGrecques();

	protected:

		double m_K,m_T,m_S0, m_R, m_sigma;
	};
}

#endif