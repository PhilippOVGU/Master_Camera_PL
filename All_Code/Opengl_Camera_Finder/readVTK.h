#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "defines.h"


using namespace std;

int count_timesteps(string Filename) {
	ifstream stream_str;
	stream_str.open(Filename);
	string line_str;
	int num_timesteps = 0;
	while (!stream_str.eof()) {
		stream_str >> line_str;
		if (line_str.find("wss_mag_t") != std::string::npos) {
			//cout << "found!" << endl;
			num_timesteps++;
		}
	}
	cout << "Number of timesteps: " << num_timesteps << endl;
	
	/*string combined = to_string(num_timesteps);
	cout << "wss_mag_t_" + combined << endl;*/
	return num_timesteps;
	stream_str.close();
}

vector<vector<Vertex>> read_all_wss_mag(string Filename, vector<Vertex> vertices) {
	vector < vector<Vertex>> vertices_matrix;
	int timesteps = count_timesteps(Filename);
	ifstream wss_mag_str;
	cout << "reading wss_mag" << endl;
	string line_str;
	string::size_type sz;
	// varable declaration

	int start_wss_meg_section = 0;
	bool end_wss_meg_section = false;
	int zahler = 0;
	int vertexcounter = 0;
	string wss_mag_t;
	int counter_timesteps = 0;
	float startColor[3] = {0.9411,0.231,0.1254 };
	float endColor[3] = { 1,0.9294,0.62745 };
	float percentage;
	
	
	wss_mag_t = "wss_mag_t_" + to_string(counter_timesteps);

	wss_mag_str.open(Filename);

	while (!wss_mag_str.eof()) {
		wss_mag_str >> line_str;

		if (line_str == wss_mag_t)
		{
			start_wss_meg_section = 1;
			zahler = 0;
		}
		if (start_wss_meg_section == 1 && zahler == 5)
		{
			start_wss_meg_section = 2;
		}
		if (start_wss_meg_section == 2 && line_str == "SCALARS")
		{
			end_wss_meg_section = true;
		}
		if (start_wss_meg_section == 2 && !end_wss_meg_section)
		{
			//cout << "bin in lese bedingung!" << endl;
			//vertices[vertexcounter].r = -10 * stof(line_str, &sz) + 5;
			//vertices[vertexcounter].g = 0;
			//vertices[vertexcounter].b = 6 * stof(line_str, &sz);
			percentage = stof(line_str, &sz)/0.4f;
			vertices[vertexcounter].r = (startColor[0] * (1 - percentage)) + (endColor[0] * percentage);
			vertices[vertexcounter].g = (startColor[1] * (1 - percentage)) + (endColor[1] * percentage);
			vertices[vertexcounter].b = (startColor[2] * (1 - percentage)) + (endColor[2] * percentage);
			vertices[vertexcounter].data = stof(line_str, &sz);
			vertexcounter++;
			if (vertexcounter==vertices.size())
			{	// variable reset between vertices
				counter_timesteps++;
				wss_mag_t = "wss_mag_t_" + to_string(counter_timesteps);
				start_wss_meg_section = 0;
				end_wss_meg_section = false;
				zahler = 0;
				vertexcounter = 0;
				vertices_matrix.push_back(vertices);
			}
		}
		zahler++;
	}
	wss_mag_str.close();

	


	//}
	return vertices_matrix;

}

vector<Vertex> read_wss_mag2(string Filename, vector<Vertex> vertices) {
	ifstream Nromals_str;
	Nromals_str.open(Filename);
	string line_str;
	string::size_type sz;
	int start_wss_meg_section = 0;
	bool end_wss_meg_section = false;
	int zahler = 0;
	int vertexcounter = 0;
	float maximum = 0;
	while (!Nromals_str.eof()) {
		Nromals_str >> line_str;

		if (line_str == "wss_mag_t_1")
		{
			start_wss_meg_section = 1;
			zahler = 0;
		}
		if (start_wss_meg_section == 1 && zahler == 5)
		{
			start_wss_meg_section = 2;
		}
		if (start_wss_meg_section == 2 && line_str == "SCALARS")
		{
			end_wss_meg_section = true;
		}
		if (start_wss_meg_section == 2 && !end_wss_meg_section)
		{
			if (maximum < stof(line_str, &sz))
			{
				maximum = stof(line_str, &sz);
			}
			vertices[vertexcounter].r = -10 * stof(line_str, &sz) + 5;
			vertices[vertexcounter].g = 0;
			vertices[vertexcounter].b = 6 * stof(line_str, &sz);
			//stof(line_str, &sz)
			vertexcounter++;
		}
		zahler++;
	}
	//cout << "Maximum ist: " << maximum << endl;
	return vertices;
}

vector<Vertex> read_wss_mag(string Filename, vector<Vertex> vertices) {
	ifstream Nromals_str;
	Nromals_str.open(Filename);
	string line_str;
	string::size_type sz;
	int start_wss_meg_section = 0;
	bool end_wss_meg_section = false;
	int zahler = 0;
	int vertexcounter = 0;
	float maximum = 0;
	while (!Nromals_str.eof()) {
		Nromals_str >> line_str;

		if (line_str=="wss_mag_t_0")
		{
			start_wss_meg_section = 1;
			zahler = 0;
		}
		if (start_wss_meg_section==1 && zahler==5)
		{
			start_wss_meg_section = 2;
		}
		if (start_wss_meg_section==2&&line_str=="SCALARS")
		{
			end_wss_meg_section = true;
		}
		if (start_wss_meg_section==2&& !end_wss_meg_section)
		{
			if (maximum< stof(line_str, &sz))
			{
				maximum = stof(line_str, &sz);
			}
			vertices[vertexcounter].r = -10*stof(line_str, &sz)+5;
			vertices[vertexcounter].g = 0;
			vertices[vertexcounter].b = 6* stof(line_str, &sz);
			//stof(line_str, &sz)
			vertexcounter++;
		}
		zahler++;
	}
	cout << "Maximum ist: " << maximum << endl;
	return vertices;
}

vector<Vertex> readNormls(string Filename,vector<Vertex> vertices) {
	
	ifstream Nromals_str;
	Nromals_str.open(Filename);
	string line_str;
	string::size_type sz;
	int zahler = 0;
	int cordicounter = 1;
	int vertexcounter = 0;
	float cordix, cordiy, cordiz;
	int startNormalSection = 0;
	bool endNormalSection = false;
	while (!Nromals_str.eof())
	{
		Nromals_str >> line_str;
		if (line_str== "POINT_DATA")
		{
			startNormalSection = 1; //übergangszustand --> start gefunden jetzt zählen bis eigentliche daten anfangen
			zahler = 0;
		}
		if (startNormalSection==1&&zahler==8)
		{
			startNormalSection = 2;
			endNormalSection = false;
		}
	
		if (startNormalSection==2&& line_str== "SCALARS")
		{
			endNormalSection = true;
		}



		if (startNormalSection == 2 && !endNormalSection ) // conditions met start reading secton 
		{
			int test = 0;
			switch (cordicounter)
			{
			case 1:
				cordix = stof(line_str, &sz);
				cordicounter++;

				break;
			case 2:
				cordiy = stof(line_str, &sz);

				cordicounter++;
				break;
			case 3:
				cordiz = stof(line_str, &sz);
				

				cordicounter = 1;
				// pushing values in vertices vector;
				Position normalVector;

				vertices[vertexcounter].nx = cordix;
				vertices[vertexcounter].ny = cordiy;
				vertices[vertexcounter].nz = cordiz;
				vertexcounter++;
				
				break;
			default:
				cerr << "Error with Cordicounter somehow" << endl;
				break;
			}

		}

		zahler++;
	}

	return vertices;
}


vector<vector<Vertex>> create_time_line(vector<double> time_linedata, vector<Vertex> vertices,int numtimesteps) {
	vector < vector<Vertex>> vertices_matrix;
	//find max
	double max = 0;
	vector<Vertex> vertices_copy;
	for (size_t i = 0; i < time_linedata.size(); i++)
	{
		if (max<time_linedata[i])
		{
			max = time_linedata[i];
		}
	}
	cout << "maximum: " << max << endl;
	double timestep = max / numtimesteps;

	for (int i = 0; i < numtimesteps; i++)
	{
		vertices_copy = vertices;
		for (int j = 0; j < vertices.size(); j++)
		{
			if (time_linedata[j]<(0+i*timestep) || time_linedata[j]>(timestep+i*timestep)){

				//vertices_copy[j].r = 0;
				//vertices_copy[j].g = 0;
				//vertices_copy[j].b = 0;
				vertices_copy[j].a = 0;
			}
		}
		vertices_matrix.push_back(vertices_copy);

	}
	cout << "maximum: " << max << endl;
	return vertices_matrix;

}
vector<double> readTime(string Filename) {
	vector<double> time;
	ifstream time_str;


	time_str.open(Filename);
	string line_str;
	string::size_type sz;
	int zahler = 0;
	int StartIndexSection = 0; //0=false 1= übergangszustand 2=true
	bool EndIndexSection = false;

	while (!time_str.eof())
	{

		time_str >> line_str;

		if (line_str == "SCALARS" && StartIndexSection == 2)
		{
			EndIndexSection = true;
		}

		

		if (StartIndexSection == 2 && !EndIndexSection)// All conditions med --> here starts actuall reading of section with line information
		{
			time.push_back(atof(line_str.c_str()));
		}

		if (line_str == "time")
		{
			StartIndexSection = 1;
			zahler = 0;
		}
		else if (StartIndexSection == 1 && zahler == 4)
		{
			StartIndexSection = 2;
		}
		zahler++;
		//cout << line_str << "  zahler: " << zahler << endl;
	}

	//int test2 = 5;
	time_str.close();
	return time;
}
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
					indices.push_back(stoi(line_str, &sz));
				}
				else if (i == (lineLength - 1))
				{
					indices.push_back(stoi(line_str, &sz));
					indices.push_back(stoi(line_str, &sz));
					indices.push_back(stoi(line_str, &sz)); //for some reasone i need 3 for lines_adjacancy instead of 2 
					
				}
				else
				{
					indices.push_back(stoi(line_str, &sz));
					//indices.push_back(stoi(line_str, &sz));
				}

			}

			indices.push_back(INT_MAX);

		}
		
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
		
	}
	
	Vertex_str.close();
	return vertices;
}