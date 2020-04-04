#pragma once
#include <cstdint>
#include "glm/glm.hpp"
#include <vector>

struct Position {
	float x, y, z;
};

struct Vertex
{
	float x;
	float y;
	float z;

	float u;
	float v;

	float r;
	float g;
	float b;
	float a;

	float nx;
	float ny;
	float nz;

	float data;
	


	
};
struct kmenashelperformat
{
	Vertex kmeanspoint;
	std::vector<Vertex> assignedpoints;
	std::vector < std::vector<Vertex>> verticesoutput;
	std::vector<Vertex > vectorVertexoutput;
	
};