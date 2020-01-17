#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "defines.h"
#include <Eigen/Dense>
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