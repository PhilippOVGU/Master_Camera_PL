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
	//if (argc<3)
	//{
	//	cout << "Missing Arguments in console --> Usage: Opengl_camera_finder.exe nameofvtkfile mesh/lines" << endl;
	//	return 1;
	//}
	////reading in filename and if data is pathlines or mesh
	//string Filename_aorta = argv[1];
	//bool isLinedata = false;
	////cout << "argv[2] :" << argv[2] << endl;
	//string checkargv2 = argv[2];
	//if (checkargv2 =="lines")
	//{
	//	isLinedata = true;
	//}
	//else {
	//	isLinedata = false;
	//}
	//cout << "islinedata: " << isLinedata << endl;
	string Filename_aorta = "Aorta_mesh.vtk";
	//string Filename_aorta = "cube.vtk";
	//string Filename_pathlines = "cube_pathlines.vtk";
	string Filename_pathlines = "Aorta_pathlines.vtk";
	//string Filename_aorta = "Aorta_pathlines.vtk";
	//string Filename_aorta = "cube_pathlines.vtk";
	bool isLinedata = false;
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
	std::vector<Vertex> vertices_2;
	vector<vector<Vertex>> vertices_matrix;
	int num_elements = 0;
	
	std::vector<Position> normals;

	uint64_t numVertices = 0;
	std::vector<uint32_t> indices;
	std::vector<uint32_t> indices_2;
	


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
		//vertices_2 = vertices;
		//vertices=read_wss_mag(Filename_aorta, vertices);
		//vertices_2 = read_wss_mag2(Filename_aorta, vertices_2);
		indices = readIndices_Vertex(Filename_aorta);
		
		//vertices_matrix = read_all_wss_mag(Filename_aorta, vertices);
		vertices_2 = readVertices(Filename_pathlines);
		indices_2 = readIndices_Line(Filename_pathlines);
		cout << "done with loading" << endl;
	}

	numIndices = indices.size();
	numVertices = vertices.size();
	uint64_t numIndices_2 = indices_2.size();
	uint64_t numVertices_2= vertices_2.size();

	//cout << "numVerticec: " << numVertices << " numIndices :" << numIndices << endl;
	IndexBuffer indexBuffer(indices.data(), numIndices, sizeof(indices[0]));
	VertexBuffer vertexBuffer(vertices.data(), numVertices);
	
	
	IndexBuffer indexBuffer_2(indices_2.data(), numIndices_2, sizeof(indices_2[0]));
	VertexBuffer vertexBuffer_2(vertices_2.data(), numVertices_2);
	
	

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


	Shader shader("basic_vs.glsl", "basic_fs.glsl");
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
	bool buttonQ = false;
	bool buttonE = false;
	float cameraSpeed = 6.0f;
		
	glEnable(GL_CULL_FACE); //enables culling(hide not shown triangle)
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST); // Tiefentest --> vorgrund überdeckt hintergrund
	


	// varables and calculations for changing surface depended on time
	int maxtime = 40; //länge der annimation
	int surfacecounter = 0;
	int start_timeframe = 0;
	int end_timeframe = 0;
	bool changdata = true;
	int numtimesteps = count_timesteps(Filename_aorta);
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
					case SDLK_q:
						buttonQ = true;
						break;
					case SDLK_e:
						buttonE = true;
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
				case SDLK_q:
					buttonQ = false;
					break;
				case SDLK_e:
					buttonE = false;
					break;
				default:
					break;
					}
			}
			// mouse camera movment
			/*else if (event.type==SDL_MOUSEMOTION)
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
			}*/
			
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
		if (buttonQ)
		{
			camera.moveFront(-delta * cameraSpeed);
			camera.update();
		}
		if (buttonE)
		{
			camera.moveFront(delta* cameraSpeed);
			camera.update();
		}
		if (buttonW)
		{
			model = glm::rotate(model, 0.5f * delta, glm::vec3(0.5f, 0.0f, 0));
			//camera.moveFront(-delta* cameraSpeed);
			camera.update();
		}
		if (buttonS)
		{
			model = glm::rotate(model, 0.5f * delta, glm::vec3(-0.5f, 0.0f, 0));
			//camera.moveFront(delta* cameraSpeed);
			camera.update();
		}
		if (buttonA)
		{
			model = glm::rotate(model, 0.5f * delta, glm::vec3(0, 0.5f, 0));
			//camera.moveSidways(-delta* cameraSpeed);
			camera.update();
		}
		if (buttonD)
		{
			model = glm::rotate(model, 0.5f * delta, glm::vec3(0, -0.5f, 0));
			//camera.moveSidways(delta* cameraSpeed); 
			camera.update();
		}



		// Change surface time dependend
		//if (!isLinedata)
		//{


		//	if (surfacecounter < numtimesteps + 1) //numtimesteps+1 because if(time >start_timeframe..) is false at the start (at t=0) and as a result surfacecounter skipps 0
		//	{

		//		if (time >= (start_timeframe * surfacecounter) && time < ((end_timeframe + maxtime / numtimesteps) * surfacecounter))
		//		{
		//			if (changdata)
		//			{
		//				cout << surfacecounter << endl;
		//				vertexBuffer.newData(vertices_matrix[surfacecounter - 1].data(), numVertices); // surfacecounter-1 because if(time >start_timeframe..) is false at the start (t=0) and surfacecounter skipps 0
		//				changdata = false;
		//			}
		//		}
		//		else
		//		{
		//			surfacecounter++;
		//			changdata = true;
		//		}
		//	}
		//}
		
		
		
		

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		//rotation:
		//model = glm::rotate(model, 0.25f * delta, glm::vec3(0.125f, 0.25f, 0));
		modelViewProj = camera.getViewProj()* model;
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe modus

		// lighting matrixes
		glm::mat4 modelView = camera.getView()*model;
		glm::mat4 invModelView = glm::transpose(glm::inverse(modelView));
		//texture stuff
		

		
		glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProj[0][0]); //übergibt die prjection matrix an den shader
		glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, &modelView[0][0]); //übergabe der view matrix an den shader
		glUniformMatrix4fv(invModelViewLocation, 1, GL_FALSE, &invModelView[0][0]); //übergabe der view matrix an den shader

		
		glActiveTexture(GL_TEXTURE0);
	
		if (isLinedata)
		{
			vertexBuffer.bind();
			indexBuffer.bind();
			glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
			vertexBuffer.unBind();
			indexBuffer.unBind();
		}
		else
		{
			vertexBuffer.bind();
			indexBuffer.bind();
			glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
			vertexBuffer.unBind();
			indexBuffer.unBind();


			vertexBuffer_2.bind();
			indexBuffer_2.bind();
			glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
			vertexBuffer_2.unBind();
			indexBuffer_2.unBind();
		}
		
		
		

		SDL_GL_SwapWindow(window);



		//cout << time << endl;
		
		
		
		
		
		
	
		

	



	




	
	
	
	
	
	
	
	}
	
	glDeleteTextures(1, &textureId); // löschen der texture aus speicher --> nicht zwingend nötig da alles gelöscht wird wenn programm beendet wird

	//system("pause");
	return 0;
}

