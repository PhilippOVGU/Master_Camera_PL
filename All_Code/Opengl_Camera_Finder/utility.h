#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "defines.h"
using namespace std;


double findMax(vector <double> time_linedata) {
	double max = 0;
	for (size_t i = 0; i < time_linedata.size(); i++)
	{
		if (time_linedata[i]>max)
		{
			max = time_linedata[i];
		}
	}
	return max;
}
vector <double> upperBound(int numSteps,double Maximum) {
	vector<double> upperBound;
	double currentBound;
	for (size_t i = 0; i < numSteps; i++)
	{
		currentBound = Maximum / numSteps * i+ Maximum / numSteps;
		upperBound.push_back(currentBound);

	}
	upperBound.push_back(currentBound);
	return upperBound;
}

vector <double> lowerBound(int numSteps, double Maximum) {
	vector<double> lowerBound;
	double currentBound;
	for (size_t i = 0; i < numSteps; i++)
	{
		currentBound = Maximum / numSteps * i;
		lowerBound.push_back(currentBound);

	}
	lowerBound.push_back(currentBound);
	return lowerBound;
}

vector<Vertex> swapVertex(double lowerlimit,double upperlimit,vector<double> time,vector<Vertex> vertices) {
	for (size_t i = 0; i < vertices.size(); i++)
	{
		if (time[i]<lowerlimit || time[i]>upperlimit)
		{
			vertices[i].a = 0;
		}
	}
	return vertices;
}

