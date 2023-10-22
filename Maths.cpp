#define _USE_MATH_DEFINES

#include <random>
#include <algorithm>
#include <numeric>

namespace Maths
{
	double pdf_normal(double x, double avg, double stddev)
	{
		return (1 / (stddev * std::sqrt(2 * M_PI))) * std::exp(-0.5 * std::pow((x - avg) / stddev, 2));
	}

	double pdf_normal_standard(double x)
	{
		// standard normal distribution is normal distribution with average of 0 and standard deviation of 1
		return pdf_normal(x, 0., 1.);
	}

	double draw_sample_normal_standard()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::normal_distribution<double> distribution(0, 1);

		double a = distribution(gen);

		return a;
	}
}
