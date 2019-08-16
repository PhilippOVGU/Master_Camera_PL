#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "defines.h"

using namespace std;

vector<uint32_t> readIndices_Line(string Filename) {
	vector<uint32_t> indices;
	ifstream Indices_str;

	
	Indices_str.open(Filename);
	string line_str;
	string::size_type sz;
	int zahler = 0;
	int StartIndexSection = 0; //0=false 1= übergangszustand 2=true
	bool EndIndexSection = false;

	while (!Indices_str.eof())
	{

		Indices_str >> line_str;

		if (line_str == "POINT_DATA")
		{
			EndIndexSection = true;
		}

		// All conditions med --> here starts actuall reading of section with line information

		if (StartIndexSection == 2 && !EndIndexSection)
		{

			int lineLength = stoi(line_str, &sz); //first entry in every line is the length of the line
			// creating modified indices for gllines to work (eg 1,2,3,4 --> 1,2,2,3,3,4 end points of every line become start points of next line creating a long line)
			for (size_t i = 0; i < lineLength; i++)
			{
				Indices_str >> line_str;
				if (i == 0)
				{
					indices.push_back(stoi(line_str, &sz));
				}
				else if (i == (lineLength - 1))
				{
					indices.push_back(stoi(line_str, &sz));
				}
				else
				{
					indices.push_back(stoi(line_str, &sz));
					indices.push_back(stoi(line_str, &sz));
				}

			}

			//indices.push_back(stoi(line_str, &sz));

		}
		//cout << line_str << endl;;
		if (line_str == "LINES")
		{
			StartIndexSection = 1;
			zahler = 0;
		}
		else if (StartIndexSection == 1 && zahler == 2)
		{
			StartIndexSection = 2;
		}
		zahler++;
		//cout << line_str << "  zahler: " << zahler << endl;
	}

	//int test2 = 5;
	Indices_str.close();
	return indices;
}

vector<uint32_t> readIndices_Vertex(string filename) {
	vector<uint32_t> indices;
	ifstream Indices_str;

	//Indices_str.open("Indexbuffer_short.txt");
	Indices_str.open(filename);
	string line_str;
	string::size_type sz;
	int zahler = 0;
	int StartIndexSection = 0; //0=false 1= übergangszustand 2=true
	bool EndIndexSection = false;

	while (!Indices_str.eof())
	{

		Indices_str >> line_str;

		if (line_str == "POINT_DATA")
		{
			EndIndexSection = true;
		}

		if (StartIndexSection == 2 && !EndIndexSection && (zahler + 1) % 4 == 0)
		{

			if (stoi(line_str, &sz) != 3)
			{
				cerr << "Error Polygons are no Triangles readIndices and readVertices will fail" << endl;
			}
		}

		if (StartIndexSection == 2 && !EndIndexSection && (zahler + 1) % 4 != 0)
		{
			indices.push_back(stoi(line_str, &sz));

		}
		//cout << line_str << endl;;
		if (line_str == "POLYGONS")
		{
			StartIndexSection = 1;
			zahler = 0;
		}
		else if (StartIndexSection == 1 && zahler == 2)
		{
			StartIndexSection = 2;
		}
		zahler++;
		//cout << line_str << "  zahler: " << zahler << endl;
	}

	//int test2 = 5;
	Indices_str.close();
	//cout << "Anzahl der Indices: " << indices.size() << endl;
	return indices;
}
vector<Vertex> readVertices(string filename) {
	vector<Vertex> vertices;
	ifstream Vertex_str;
	//Vertex_str.open("musterVertices.txt");
	Vertex_str.open(filename);
	string line_str;
	int zahler = 0;
	int cordicounter = 1;
	string::size_type sz;
	double cordix;
	double cordiy;
	double cordiz;
	bool isVertexSectionstart = false;
	bool isVertexSectionEnd = false;
	double maximum = 0.0f;
	while (!Vertex_str.eof())
	{
		Vertex_str >> line_str;
		//cout << line_str << endl;;
		if (line_str == "POLYGONS" || line_str == "LINES")
		{
			isVertexSectionEnd = true;
		}

		if (isVertexSectionstart && !isVertexSectionEnd)
			if (isVertexSectionstart)
			{
				switch (cordicounter)
				{
				case 1:
					cordix = stof(line_str, &sz);
					cordicounter++;
					if (abs(cordix) > maximum)
					{
						maximum = abs(cordix);
					}
					break;
				case 2:
					cordiy = stof(line_str, &sz);
					if (abs(cordiy) > maximum)
					{
						maximum = abs(cordiy);
					}
					cordicounter++;
					break;
				case 3:
					cordiz = stof(line_str, &sz);
					if (abs(cordiz) > maximum)
					{
						maximum = abs(cordiz);
					}
					cordicounter = 1;
					// pushing values in vertices vector;
					Vertex vertex;

					vertex.x = cordix;
					vertex.y = cordiy;
					vertex.z = cordiz;
					//default color white
					vertex.r = 1.0f;
					vertex.g = 1.0f;
					vertex.b = 1.0f;
					vertex.a = 1.0f;
					vertices.push_back(vertex);
					break;
				default:
					cerr << "Error with Cordicounter somehow" << endl;
					break;
				}
				//cout << maximum << endl;
			}

		if (line_str == "double")
		{
			isVertexSectionstart = true;
		}



		zahler++;
		/*
			if (zahler == 9) {
				int test = 5;
			}*/
	}
	for (size_t i = 0; i < vertices.size(); i++)
	{

		vertices[i].x = vertices[i].x / maximum;
		vertices[i].y = vertices[i].y / maximum;
		vertices[i].z = vertices[i].z / maximum;
		//cout << "in for schleif  " << vertices.size() << endl;
	}
	//cout << "Anzahl der Vertices: "<<vertices.size() << endl;
	Vertex_str.close();
	return vertices;
}