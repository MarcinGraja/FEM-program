#pragma once
#include <vector>
#include <string>
struct integrationPoint
{
	double x;
	double weight;
	integrationPoint(double x, double weight)
	{
		this->x = x;
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
	double ambientTemperature;
	double simulationTime;
	double timeStep;
	double initialTemp;
	std::vector<integrationPoint> integrationPoints;
	void initializeData()
	{
		initialTemp = 100;
		simulationTime = 500;
		timeStep = 50;
		ambientTemperature = 1200;
		alpha = 300;
		H = 1.5;
		W = 0.5;
		nodeCountH = 5;
		nodeCountW = 4;
		k = 25;
		c = 700;
		rho = 7800;
		
		
		double p1 = 1.0 / sqrt(3);
		double p2 = -p1;
		integrationPoints.push_back(integrationPoint(p1, 1.0));
		integrationPoints.push_back(integrationPoint(p2, 1.0));
	}
};

