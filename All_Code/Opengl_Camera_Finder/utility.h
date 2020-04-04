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
	int firstmax = 0;
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
		//if (SortEigenvalues[0]== EigenvalueVector(i).real())
		//{
		//	firstmax = i;
		//}
		if (SortEigenvalues[1]== EigenvalueVector(i).real())
		{
			Secondmax = i;
		}

	}
	//cout << "second maximum eigenvalue " << max << endl;
	//cout << " second maxium varation direction " << EigenvectorMatrix.row(Secondmax) << endl;
	Eigen::VectorXcd EigenvectorVector = EigenvectorMatrix.row(Secondmax);
	//Eigen::VectorXcd EigenvectorVector2 = EigenvectorMatrix.row(firstmax);
	//glm::vec3 eigen1 = glm::vec3(EigenvectorVector(0).real(), EigenvectorVector(1).real(), EigenvectorVector(2).real());
	//glm::vec3 eigen0 = glm::vec3(EigenvectorVector2(0).real(), EigenvectorVector2(1).real(), EigenvectorVector2(2).real());
	//glm::vec3 result = glm::cross(eigen0, eigen1);
	//double x = EigenvectorVector(0).real();
	//double y = EigenvectorVector(1).real();
	//double z = EigenvectorVector(2).real();
	vector<float> results;
	//results.push_back(result.x);
	//results.push_back(result.y);
	//results.push_back(result.z);
	results.push_back(EigenvectorVector(0).real());
	results.push_back(EigenvectorVector(1).real());
	results.push_back(EigenvectorVector(2).real());
	cout << "now we get everything as double" << endl;
	//cout << "X value: " << x << " Yvalue: " << y << " Z value" << z << endl;
	return results;
}


float cityblock(Vertex Kmeanspoint, Vertex datapoint) {
	float distance;
	float test= abs(Kmeanspoint.data - datapoint.data);
	//distance = abs(Kmeanspoint.x - datapoint.x) + abs(Kmeanspoint.y - datapoint.y) + abs(Kmeanspoint.z - datapoint.z) + abs(Kmeanspoint.data - datapoint.data);
	//distance = sqrt(pow(Kmeanspoint.x - datapoint.x,2) + pow(Kmeanspoint.y - datapoint.y,2) + pow(Kmeanspoint.z - datapoint.z,2) + pow(Kmeanspoint.data - datapoint.data,2));
	distance = (pow(Kmeanspoint.x - datapoint.x,2) + pow(Kmeanspoint.y - datapoint.y,2) + pow(Kmeanspoint.z - datapoint.z,2) + pow(Kmeanspoint.data - datapoint.data,2));
	//cout << "datadistance: " << abs(Kmeanspoint.data - datapoint.data) << endl;
	return distance;
}

kmenashelperformat kmeans_pathline(vector<Vertex> vertices_matrix,vector<double> inputdata) {
	int kvalue = 10;
	//int numtimesteps = vertices_matrix.size();
	//int numdatapoints = vertices_matrix[0].size();
	int numdatapoints = vertices_matrix.size();

	
	

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
	int equalcounter = 0;
	float dataweight = 1;
	vector<Vertex> Clusterpoints;
	
	for (size_t i = 0; i < numdatapoints; i++)
	{
		permutation.push_back(i);
		data.push_back(vertices_matrix[i]);
		data[i].data = inputdata[i]* dataweight;
	}
	
	// obtain a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(permutation.begin(), permutation.end(), std::default_random_engine(seed));

	for (size_t i = 0; i < kvalue; i++)
	{
		//int randindex = rand() % (kvalue - 1 + 1) + 1;
		int randindex = permutation[i];
		kmeansnow.push_back(vertices_matrix[randindex]);
		kmeansnow[i].data = inputdata[randindex];
		//cout << randindex << endl;

	}

	vector<Vertex> kmeanslast;
	vector<vector<float>> abstandMatrix;
	vector<float>abstand;
	int loopcounter = 0;
	//loopstart
	while (equalcounter != kvalue)
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
					//cout << "k teffer: " << k << endl;
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
		std::cout << "loopcounter: " << loopcounter << endl;
	}




	//loop end

	int highest_k_data_index;
	float maximumvalue = 0;
	for (size_t i = 0; i < kmeansnow.size(); i++)
	{
		if (kmeansnow[i].data > maximumvalue)
		{
			maximumvalue = kmeansnow[i].data;
			highest_k_data_index = i;
		}
	}

	vector<Vertex> kmeans_points;
	for (size_t i = 0; i < vertices_matrix.size(); i++)
	{
		vertices_matrix[i].x = vertices_matrix[i].x - kmeansnow[highest_k_data_index].x;
		vertices_matrix[i].y = vertices_matrix[i].y - kmeansnow[highest_k_data_index].y;
		vertices_matrix[i].z = vertices_matrix[i].z - kmeansnow[highest_k_data_index].z;
		if (assigment[i] == highest_k_data_index)
		{
			kmeans_points.push_back(vertices_matrix[i]);
		}
	}
	kmenashelperformat output;
	output.kmeanspoint = kmeansnow[highest_k_data_index];
	output.assignedpoints = kmeans_points;
	//output.assignedpoints = kmeans_points;
	output.vectorVertexoutput = vertices_matrix;
	int test = 5;
	test = test + numdatapoints;
	return output;



}

kmenashelperformat kmeans(vector < vector<Vertex>> vertices_matrix) {
	int kvalue = 10;
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
					//cout << "k teffer: " << k << endl;
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

		//std::cout << "Equalcounter: " << equalcounter << endl;
		//std::cout << "loopcounter: " << loopcounter << endl;
	}

	

		
		//loop end
		
	int highest_k_data_index;
	float maximumvalue = 0; // change this if miniumum is whished
	for (size_t i = 0; i < kmeansnow.size(); i++)
	{
		if (kmeansnow[i].data>maximumvalue) // change this if miniumum is whished
		{
			maximumvalue = kmeansnow[i].data; 
			highest_k_data_index = i;
		}
	}

	//continous color change
	float endColor[3] = { 0.9411,0.231,0.1254 };
	float startColor[3] = { 1,0.9294,0.62745 };

	// Discrete color change
	// https://colorbrewer2.org/#type=sequential&scheme=Blues&n=5
	float Color0[3] = { 0.936 ,0.949,0.996 };
	float Color1[3] = { 0.738,0.84,0.9 };
	float Color2[3] = { 0.418,0.68,0.836 };
	float Color3[3] = { 0.191,0.508,0.738 };
	float Color4[3] = { 0.03,0.316,0.609 };

	float maximumdata = 0;
	float average = 0;
	float percentage;
	float percentage_continous;
	
	for (size_t i = 0; i < numdatapoints; i++)
	{
		average = meandata[i] + average;
		if (meandata[i] > maximumdata)
		{
			maximumdata = meandata[i];
		}

	}
	average = average / numdatapoints;
	for (size_t i = 0; i < numtimesteps; i++)
	{
		for (size_t j = 0; j < vertices_matrix[0].size(); j++)
		{
			//vertices_matrix[i][j].x = vertices_matrix[i][j].x - 0.165017396;
			//vertices_matrix[i][j].y = vertices_matrix[i][j].y - 0.17192883;
			//vertices_matrix[i][j].z = vertices_matrix[i][j].z + 0.257190108;
			vertices_matrix[i][j].x = vertices_matrix[i][j].x - kmeansnow[highest_k_data_index].x;
			vertices_matrix[i][j].y = vertices_matrix[i][j].y - kmeansnow[highest_k_data_index].y;
			vertices_matrix[i][j].z = vertices_matrix[i][j].z - kmeansnow[highest_k_data_index].z;
			


			////Continous color change
			//percentage_continous = vertices_matrix[i][j].data / (average * 1.5);
			//vertices_matrix[i][j].r = (startColor[0] * (1 - percentage_continous)) + (endColor[0] * percentage_continous);
			//vertices_matrix[i][j].g = (startColor[1] * (1 - percentage_continous)) + (endColor[1] * percentage_continous);
			//vertices_matrix[i][j].b = (startColor[2] * (1 - percentage_continous)) + (endColor[2] * percentage_continous);

			// Discrete color change
			percentage = meandata[j] / (average * 1.5);
			//discrete color scale
			if (percentage < 0.2)
			{
				vertices_matrix[i][j].r = Color0[0];
				vertices_matrix[i][j].g = Color0[1];
				vertices_matrix[i][j].b = Color0[2];

			}
			if (percentage > 0.2 && percentage < 0.4)
			{
				vertices_matrix[i][j].r = Color1[0];
				vertices_matrix[i][j].g = Color1[1];
				vertices_matrix[i][j].b = Color1[2];
			}
			if (percentage > 0.4 && percentage < 0.6)
			{
				vertices_matrix[i][j].r = Color2[0];
				vertices_matrix[i][j].g = Color2[1];
				vertices_matrix[i][j].b = Color2[2];
			}
			if (percentage > 0.6 && percentage < 0.8)
			{
				vertices_matrix[i][j].r = Color3[0];
				vertices_matrix[i][j].g = Color3[1];
				vertices_matrix[i][j].b = Color3[2];
			}
			if (percentage > 0.8)
			{
				vertices_matrix[i][j].r = Color4[0];
				vertices_matrix[i][j].g = Color4[1];
				vertices_matrix[i][j].b = Color4[2];
			}

			//	énable disables color overwrite
			//if (assigment[j] == highest_k_data_index) {
			//	vertices_matrix[i][j].r = 0;
			//	vertices_matrix[i][j].g = 0;
			//	vertices_matrix[i][j].b = 1;
			//}

		}

	}
	vector<Vertex> kmeans_points;
	for (size_t i = 0; i < vertices_matrix[0].size(); i++)
	{
		if (assigment[i] == highest_k_data_index)
		{
			kmeans_points.push_back(vertices_matrix[0][i]);
		}
	}
	kmenashelperformat output;
	output.kmeanspoint = kmeansnow[highest_k_data_index];
	output.assignedpoints = kmeans_points;
	output.verticesoutput = vertices_matrix;
	//output.assignedpoints = kmeans_points;
	int test = 5;
	test = test + numdatapoints;
	std::cout  << loopcounter << endl;
	return output;



}

vector < vector<Vertex>> discritecolor(vector < vector<Vertex>> vertices_matrix) {
	int kvalue = 50;
	int numtimesteps = vertices_matrix.size();
	int numdatapoints = vertices_matrix[0].size();
	vector<float> meandata;
	// https://colorbrewer2.org/#type=sequential&scheme=Blues&n=5
	float Color0[3] = { 0.936 ,0.949,0.996 };
	float Color1[3] = { 0.738,0.84,0.9 };
	float Color2[3] = { 0.418,0.68,0.836 };
	float Color3[3] = { 0.191,0.508,0.738 };
	float Color4[3] = { 0.03,0.316,0.609 };


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
			if (i == (numtimesteps - 1))
			{
				meandata[j] = meandata[j] / numtimesteps;
			}

		}

	}
	// finding maximum meandata
	float maximumdata = 0;
	float average = 0;
	
	for (size_t i = 0; i < numdatapoints; i++)
	{
		average = meandata[i] + average;
		if (meandata[i]>maximumdata)
		{
			maximumdata = meandata[i];
		}

	}
	average = average / numdatapoints;
	float percentage;
	maximumdata = maximumdata * 0.6;
	for (size_t i = 0; i < numtimesteps; i++)
	{
		for (size_t j = 0; j < numdatapoints; j++)
		{
			percentage = meandata[j] / (average*1.5);
			//discrete color scale
			if (percentage<0.2)
			{
				vertices_matrix[i][j].r = Color0[0];
				vertices_matrix[i][j].g = Color0[1];
				vertices_matrix[i][j].b = Color0[2];

			}
			if (percentage>0.2&&percentage<0.4)
			{
				vertices_matrix[i][j].r = Color1[0];
				vertices_matrix[i][j].g = Color1[1];
				vertices_matrix[i][j].b = Color1[2];
			}
			if (percentage > 0.4 && percentage < 0.6)
			{
				vertices_matrix[i][j].r = Color2[0];
				vertices_matrix[i][j].g = Color2[1];
				vertices_matrix[i][j].b = Color2[2];
			}
			if (percentage>0.6&&percentage<0.8)
			{
				vertices_matrix[i][j].r = Color3[0];
				vertices_matrix[i][j].g = Color3[1];
				vertices_matrix[i][j].b = Color3[2];
			}
			if (percentage>0.8)
			{
				vertices_matrix[i][j].r = Color4[0];
				vertices_matrix[i][j].g = Color4[1];
				vertices_matrix[i][j].b = Color4[2];
			}

		}
	}

	return vertices_matrix;


}
