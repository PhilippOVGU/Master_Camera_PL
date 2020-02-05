#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "defines.h"
#include <Eigen/Dense>
#include <random>
#include <chrono>
#include "glm/glm.hpp" //mathe für bewegungen
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
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

vector<float> PCA(vector<Vertex> vertices) {
	Eigen::MatrixXd inputMat;
	
	inputMat.resize(vertices.size(), 3); //rezizing input matrix 
	for (size_t i = 0; i < vertices.size(); i++)
	{
		inputMat(i, 0) = vertices[i].x;
		inputMat(i, 1) = vertices[i].y;
		inputMat(i, 2) = vertices[i].z;

	}
	cout << "input mat" << endl;
	//cout << inputMat << endl;

	Eigen::MatrixXd centered = inputMat.rowwise() - inputMat.colwise().mean();
	Eigen::MatrixXd cov = centered.adjoint() * centered;
	//cout << cov << endl;
	//cout << "maximum is: " << cov(1, 1) << endl;
	Eigen::EigenSolver<Eigen::MatrixXd> es(cov);
	Eigen::VectorXcd EigenvalueVector = es.eigenvalues();
	Eigen::MatrixXcd EigenvectorMatrix = es.eigenvectors();
	//cout << "The eigenvalues of A are:" << endl << EigenvalueVector << endl;
	//cout << "The matrix of eigenvectors, V, is:" <<endl<< EigenvectorMatrix<< endl;
	double max = 0;
	int Secondmax = 0;
	vector<double> SortEigenvalues;
	//sorting eigenvalues form low to high
	for (size_t i = 0; i < 3; i++)
	{
		SortEigenvalues.push_back(EigenvalueVector(i).real());
		
	}
	sort(SortEigenvalues.begin(), SortEigenvalues.end());
	//finding the index of the second highest eigenvalue
	for (size_t i = 0; i < 3; i++)
	{
		if (SortEigenvalues[1]== EigenvalueVector(i).real())
		{
			Secondmax = i;
		}

	}
	//cout << "second maximum eigenvalue " << max << endl;
	//cout << " second maxium varation direction " << EigenvectorMatrix.row(Secondmax) << endl;
	Eigen::VectorXcd EigenvectorVector = EigenvectorMatrix.row(Secondmax);
	//double x = EigenvectorVector(0).real();
	//double y = EigenvectorVector(1).real();
	//double z = EigenvectorVector(2).real();
	vector<float> results;
	results.push_back(EigenvectorVector(0).real());
	results.push_back(EigenvectorVector(1).real());
	results.push_back(EigenvectorVector(2).real());
	cout << "now we get everything as double" << endl;
	//cout << "X value: " << x << " Yvalue: " << y << " Z value" << z << endl;
	return results;
}


float cityblock(Vertex Kmeanspoint, Vertex datapoint) {
	float distance;
	distance = abs(Kmeanspoint.x - datapoint.x) + abs(Kmeanspoint.y - datapoint.y) + abs(Kmeanspoint.z - datapoint.z) + abs(Kmeanspoint.data - datapoint.data);
	//cout << "datadistance: " << abs(Kmeanspoint.data - datapoint.data) << endl;
	return distance;
}

int kmeans(vector < vector<Vertex>> vertices_matrix) {
	int kvalue = 3;
	int numtimesteps = vertices_matrix.size();
	int numdatapoints = vertices_matrix[0].size();
	vector<float> meandata;
	for (size_t i = 0; i < numtimesteps; i++)
	{
		for (size_t j = 0; j < numdatapoints; j++)
		{
			if (i == 0) {
				meandata.push_back(vertices_matrix[i][j].data);

			}
			else {
				meandata[j] = meandata[j] + vertices_matrix[i][j].data;
			}
			if (i==(numtimesteps-1))
			{
				meandata[j] = meandata[j] / numtimesteps;
			}

		}

	}
	
	//Zufällige initialisierung und datenzusammenstellung(koordinaten + meanwerte)
	vector<Vertex> data;
	vector <Vertex> kmeansnow;
	vector<int> permutation;
	float minimum;
	int indexassigment;
	vector<int> assigment;
	float x;
	float y;
	float z;
	float datavalue;
	int equalcounter=0;
	vector<Vertex> Clusterpoints;
	for (size_t i = 0; i < numdatapoints; i++)
	{
		permutation.push_back(i);
		data.push_back(vertices_matrix[0][i]);
		data[i].data = meandata[i];
	}
	// obtain a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(permutation.begin(), permutation.end(), std::default_random_engine(seed));

	for (size_t i = 0; i < kvalue; i++)
	{
		//int randindex = rand() % (kvalue - 1 + 1) + 1;
		int randindex = permutation[i];
		kmeansnow.push_back(vertices_matrix[0][randindex]);
		kmeansnow[i].data = meandata[randindex];
		//cout << randindex << endl;

	}

	vector<Vertex> kmeanslast;
	vector<vector<float>> abstandMatrix;
	vector<float>abstand;
	int loopcounter=0;
	//loopstart
	while (equalcounter!=kvalue)
	{
		kmeanslast = kmeansnow;


		//Abstandsberechnung
		loopcounter = loopcounter + 1;
		if (loopcounter == 1)
		{
			for (size_t k = 0; k < kvalue; k++)
			{
				for (size_t j = 0; j < numdatapoints; j++)
				{
					if (k == 0)
					{
						abstand.push_back(cityblock(kmeanslast[k], data[j]));
					}
					else
					{
						abstand[j] = cityblock(kmeanslast[k], data[j]);
					}

				}
				abstandMatrix.push_back(abstand);
			}

		}
		else
		{
			for (size_t k = 0; k < kvalue; k++)
			{
				for (size_t j = 0; j < numdatapoints; j++)
				{
					abstandMatrix[k][j] = cityblock(kmeanslast[k], data[j]);

				}

			}
		}

		//zugehörigkeit der punkte zu clustern festlegen aufgrund des abstandes

		for (size_t j = 0; j < numdatapoints; j++)
		{
			minimum = 100000;
			for (size_t k = 0; k < kvalue; k++)
			{
				if (abstandMatrix[k][j] < minimum)
				{
					minimum = abstandMatrix[k][j];
					indexassigment = k;
				}
			}
			if (loopcounter == 1)
			{
				assigment.push_back(indexassigment);
			}
			else {
				assigment[j] = indexassigment;
			}

		}
		//berrechnung der neuen klaustermittelpunkte 

		equalcounter = 0;
		for (size_t k = 0; k < kvalue; k++)
		{
			x = 0;
			y = 0;
			z = 0;
			datavalue = 0;
			Clusterpoints.clear();
			for (size_t j = 0; j < numdatapoints; j++)
			{
				if (assigment[j] == k)
				{
					cout << "k teffer: " << k << endl;
					Clusterpoints.push_back(data[j]);
				}
			}
			for (size_t i = 0; i < Clusterpoints.size(); i++)
			{
				x = Clusterpoints[i].x + x;
				y = Clusterpoints[i].y + y;
				z = Clusterpoints[i].z + z;
				datavalue = Clusterpoints[i].data + datavalue;

			}
			kmeansnow[k].x = x / Clusterpoints.size();
			kmeansnow[k].y = y / Clusterpoints.size();
			kmeansnow[k].z = z / Clusterpoints.size();
			kmeansnow[k].data = datavalue / Clusterpoints.size();

			if (kmeansnow[k].x == kmeanslast[k].x)
			{
				if (kmeansnow[k].y == kmeanslast[k].y)
				{
					if (kmeansnow[k].z == kmeanslast[k].z)
					{
						if (kmeansnow[k].data == kmeanslast[k].data)
						{
							equalcounter = equalcounter + 1;
						}
					}
				}
			}


		}

		std::cout << "Equalcounter: " << equalcounter << endl;
	}

	

		
		//loop end
		


	int test = 5;
	test = test + numdatapoints;
	return test;



}

