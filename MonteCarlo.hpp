#ifndef __MONTECARLO__

	#define __MONTECARLO__
	
	#include <iostream>
	#include <unordered_map>

	namespace MonteCarlo
	{
		class MonteCarlo
		{
		public:

			MonteCarlo();

			virtual ~MonteCarlo();

			virtual double optionPrice() = 0;

			//Setters

			virtual void setS(double prixDeBase) = 0;
			virtual void setK(double strike) = 0;
			virtual void setR(double taux) = 0;
			virtual void setSigma(double sigma) = 0;
			virtual void setT(double temps) = 0;

			virtual void setSample(int N) = 0;

			virtual std::unordered_map<std::string, double> optionGrecques(double h) = 0;

		protected:

			double m_S0, m_K, m_r, m_sigma, m_T;
			std::vector<double> m_samples;
		};



		class CallEuro : public MonteCarlo
		{
		public: 

			CallEuro(double prixDeBase, double strike, double taux, double temps, double sigma, int N=100000);

			virtual ~CallEuro();

			virtual double optionPrice();

			//Setters

			virtual void setS(double prixDeBase);
			virtual void setK(double strike);
			virtual void setR(double taux);
			virtual void setSigma(double sigma);
			virtual void setT(double temps);

			virtual void setSample(int N);

			virtual std::unordered_map<std::string, double> optionGrecques(double h);

		protected:

			double m_S0, m_K, m_r, m_sigma, m_T;
			std::vector<double> m_samples;
		};





		class PutEuro : public MonteCarlo
		{
		public:
			PutEuro(double prixDeBase, double strike, double taux, double temps, double sigma, int N = 100000);

			virtual ~PutEuro();

			virtual double optionPrice();

			virtual std::unordered_map<std::string, double> optionGrecques(double h);

			//Setters

			virtual void setS(double prixDeBase);
			virtual void setK(double strike);
			virtual void setR(double taux);
			virtual void setSigma(double sigma);
			virtual void setT(double temps);

			virtual void setSample(int N);


		protected:

			double m_S0, m_K, m_r, m_sigma, m_T;
			std::vector<double> m_samples;

		};


		double loi_normale_standard();
		double pdf(double x);
	}


#endif