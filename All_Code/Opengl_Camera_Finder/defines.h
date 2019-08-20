#pragma once
#include <cstdint>
#include "glm/glm.hpp"

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

	
};
