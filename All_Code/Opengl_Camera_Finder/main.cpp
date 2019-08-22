#include <iostream>
#include <Windows.h>
#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"opengl32.lib")
#include "defines.h"
//#include "Vertex_buffer.h"
#include "Vertex_buffer2.h"
#include "shader.h"
#include "index_buffer.h"
#include <cmath>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "camera.h"
#include "fps_cemera.h"
#include "stb_image.h"
#include "glm/glm.hpp" //mathe für bewegungen
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include "readVTK.h"

using namespace std;

void APIENTRY opengelDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* massage, const void* useParam) {
	std::cout << "[OpenGl error]: " << massage << std::endl;
};

int main(int argc, char** argv) {
	//error catching for console useage
	if (argc<3)
	{
		cout << "Missing Arguments in console --> Usage: Opengl_camera_finder.exe nameofvtkfile mesh/lines" << endl;
		return 1;
	}
	//reading in filename and if data is pathlines or mesh
	string Filename_aorta = argv[1];
	bool isLinedata = false;
	//cout << "argv[2] :" << argv[2] << endl;
	string checkargv2 = argv[2];
	if (checkargv2 =="lines")
	{
		isLinedata = true;
	}
	else {
		isLinedata = false;
	}
	//cout << "islinedata: " << isLinedata << endl;
	//string Filename_aorta = "Aorta_mesh.vtk";
	//string Filename_aorta = "prisma.vtk";
	//string Filename_aorta = "cube.vtk";
	//string Filename_aorta = "Aorta_pathlines.vtk";
	//bool isLinedata = true;


	SDL_Window* window;
	SDL_Init(SDL_INIT_EVERYTHING);
	
	// defining frame buffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	// debug massage
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	//change mouse display type
	SDL_SetRelativeMouseMode(SDL_TRUE);

	window = SDL_CreateWindow("Opengl_Aorta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	GLenum err = glewInit(); // initaliziere glew für neue open gl funktionen
	if (err != GLEW_OK)
	{
		cout << "Error: " << glewGetErrorString << endl;

	}
	cout << "Opengl version: " << glGetString(GL_VERSION) << endl;

	//debug massage:
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(opengelDebugCallback, 0);


	

	
	std::vector<Vertex> vertices;
	int num_elements = 0;
	
	std::vector<Position> normals;

	uint64_t numVertices = 0;
	std::vector<uint32_t> indices;
	


		uint64_t numIndices = 0;

	// loading aorta
	cout << "Loading in data....(this may take a few seconds)" << endl;
	vertices = readVertices(Filename_aorta);
	if (isLinedata)
	{
		indices = readIndices_Line(Filename_aorta);
	}
	else
	{
		vertices = readNormls(Filename_aorta,vertices);
		indices = readIndices_Vertex(Filename_aorta);
	}
	
	
	numIndices = indices.size();
	numVertices = vertices.size();

	//cout << "numVerticec: " << numVertices << " numIndices :" << numIndices << endl;
	IndexBuffer indexBuffer(indices.data(), numIndices, sizeof(indices[0]));
	VertexBuffer vertexBuffer(vertices.data(), numVertices);
	

	// loading texture
	int texturWidth = 0;
	int texturHeight = 0;
	int bitsPerPixel = 0;
	stbi_set_flip_vertically_on_load(true);
	auto textureBuffer = stbi_load("logo.png",&texturWidth,&texturHeight,&bitsPerPixel,4);
	GLuint textureId;
	//eigenschaften texture
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texturWidth, texturHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind


	if (textureBuffer) {
		stbi_image_free(textureBuffer);
	}


	Shader shader("basic.vs", "basic.fs");
	shader.bind();

	
	
	// dynamische veränderung der farbe
	int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
	if (!colorUniformLocation != -1) {
		glUniform4f(colorUniformLocation, 1.0f, 0.0f, 1.0f, 1.0f);
	}
	
	
	// texutr übergabe
	int textureUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_texture");
	if (!textureUniformLocation != -1) {
		glUniform1i(textureUniformLocation, 0);
	}
	
	
	FPSCamera camera(90.0f, 800.0f, 600.0f);
	camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
	camera.update();
	//rotationsmatrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.5f));
	
	bool ortho = false;
	//old projection limits 
	//glm::mat4 orthoprojection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -10.0f, 100.0f);
	//glm::mat4 persective = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	
	glm::mat4 orthoprojection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -50.0f, 500.0f);
	glm::mat4 persective = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 500.0f);
	glm::mat4 projection = persective;

	
	glm::mat4 modelViewProj = projection * model;
	int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");
	int modelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_modelView");
	int invModelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_invModelView");

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));

	float time = 0.0f;
	float delta = 0.02f;
	// Lässt fenster erscheinen bis event eintritt
	bool buttonW = false;
	bool buttonS = false;
	bool buttonA = false;
	bool buttonD = false;
	float cameraSpeed = 6.0f;
		
	glEnable(GL_CULL_FACE); //enables culling(hide not shown triangle)
	glEnable(GL_DEPTH_TEST); // Tiefentest --> vorgrund überdeckt hintergrund

	while (true) {
		
		

		bool close = false;
		SDL_Event event;
		while (SDL_PollEvent(&event)) { // checke ob event auftritt
			if (event.type == SDL_QUIT) // wenn event auftritt und quit event ist ander close und break den loob so das programm zuende laufen kann
			{
				close = true;
			}
			else if(event.type== SDL_KEYDOWN){
				
				if (event.key.keysym.sym == SDLK_p) {
					
						if (ortho)
						{
							projection = persective;
						}
						else
						{
							projection = orthoprojection;
						}
					}
					ortho = !ortho;
					
					
					switch (event.key.keysym.sym)
					{
					case SDLK_w:
						buttonW = true;
						break;

					case SDLK_s:
						buttonS = true;
						break;

					case SDLK_a:
						buttonA = true;
						break;

					case SDLK_d:
						buttonD = true;
						break;
					case SDLK_ESCAPE:
						SDL_SetRelativeMouseMode(SDL_FALSE);
						break;
						
					default:
						break;
					}
			}else if (event.type==SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
					{
				case SDLK_w:
					buttonW = false;
					break;

				case SDLK_s:
					buttonS = false;
					break;

				case SDLK_a:
					buttonA = false;
					break;

				case SDLK_d:
					buttonD = false;
					break;
				default:
					break;
					}
			}
			else if (event.type==SDL_MOUSEMOTION)
			{
				if (SDL_GetRelativeMouseMode())
				{
					camera.onMouseMoved(event.motion.xrel, event.motion.yrel);
				}
				
			}
			else if (event.type==SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
			}
			
		}

		if (close)
		{
			break;
		}






		// eigntliche änderung der farbe dynamisch
		time = time + delta;
		if (!colorUniformLocation != -1) {
			glUniform4f(colorUniformLocation, sinf(time)*sinf(time), 0.1f, 0.1f, 1.0f);
		}

		if (buttonW)
		{
			
			camera.moveFront(-delta* cameraSpeed);
			camera.update();
		}
		if (buttonS)
		{
			camera.moveFront(delta* cameraSpeed);
			camera.update();
		}
		if (buttonA)
		{
			camera.moveSidways(-delta* cameraSpeed);
			camera.update();
		}
		if (buttonD)
		{
			camera.moveSidways(delta* cameraSpeed); 
			camera.update();
		}



		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		//rotation:
		model = glm::rotate(model, 0.25f * delta, glm::vec3(0.125f, 0.25f, 0));
		modelViewProj = camera.getViewProj()* model;
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe modus

		// lighting matrixes
		glm::mat4 modelView = camera.getView()*model;
		glm::mat4 invModelView = glm::transpose(glm::inverse(modelView));
		//texture stuff
		vertexBuffer.bind();
		vertexBuffer.bind();
		glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProj[0][0]); //übergibt die prjection matrix an den shader
		glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, &modelView[0][0]); //übergabe der view matrix an den shader
		glUniformMatrix4fv(invModelViewLocation, 1, GL_FALSE, &invModelView[0][0]); //übergabe der view matrix an den shader

		indexBuffer.bind();
		glActiveTexture(GL_TEXTURE0);
	
		if (isLinedata)
		{
			glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
		}
		
		
		vertexBuffer.unBind();

		SDL_GL_SwapWindow(window);




		
		
		
		
		
		
		
		

	



	




	
	
	
	
	
	
	
	}
	
	glDeleteTextures(1, &textureId); // löschen der texture aus speicher --> nicht zwingend nötig da alles gelöscht wird wenn programm beendet wird

	//system("pause");
	return 0;
}

