#pragma once

// This file contains basic mathematical operations needed for pricing

namespace Maths
{
	double pdf_normal(double x, double avg,double stddev);
	double pdf_normal_standard(double x);
	double draw_sample_normal_standard();
	double inverseNormalCDF(double probability);
}

