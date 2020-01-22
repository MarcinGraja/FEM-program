#pragma once
#include <vector>
struct integrationPoint
{
	double xi;
	double eta;
	double weight;
	integrationPoint(double xi, double eta, double weight)
	{
		this->xi = xi;
		this->eta = eta;
		this->weight = weight;
	}
};
class data
{
public:
	double H;
	double W;
	int nodeCountH;
	int nodeCountW;
	double k;
	double c;
	double rho;
	double alpha;
	double T;
	std::vector<integrationPoint> integrationPoints;
	void initializeData()
	{
		H = 1.5;
		W = 0.5;
		nodeCountH = 5;
		nodeCountW = 4;
		k = 30;
		c = 700;
		rho = 7800;
		alpha = 25;
		T = 1200;
		double p1 = 1.0 / sqrt(3);
		double p2 = -p1;
		integrationPoints.push_back(integrationPoint(p1, p1, 1.0));
		integrationPoints.push_back(integrationPoint(p1, p2, 1.0));
		integrationPoints.push_back(integrationPoint(p2, p2, 1.0));
		integrationPoints.push_back(integrationPoint(p2, p1, 1.0));
	}
};

