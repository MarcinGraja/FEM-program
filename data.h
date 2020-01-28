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
		H = 0.1;
		W = 0.1;
		nodeCountH = 4;
		nodeCountW = 4;
		c = 700;
		k = 25;
		rho = 7800;
		
		
		double p1 = 1.0 / sqrt(3);
		double p2 = -p1;
		integrationPoints.push_back(integrationPoint(p1, 1.0));
		integrationPoints.push_back(integrationPoint(p2, 1.0));
	}
	void initializeData2()
	{
		initialTemp = 100;
		simulationTime = 100;
		timeStep = 1;
		ambientTemperature = 1200;
		alpha = 300;
		H = 0.1;
		W = 0.1;
		nodeCountH = 31;
		nodeCountW = 31;
		c = 700;
		k = 25;
		rho = 7800;
	}
};

