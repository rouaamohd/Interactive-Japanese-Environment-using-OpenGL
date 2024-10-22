//-----------------------------------------------------------------------------
// project.cpp
// 
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include "GL/glew.h"	// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

// my custom headers
#include "texture.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Mesh.h"

// Global Variables
const char* APP_TITLE = "CMPS373 Computer Graphics Course Project";
const int gWindowWidth = 1000;
const int gWindowHeight = 800;
GLFWwindow* gWindow = NULL;
bool gWireframe = false;
const std::string texture1FileName = "res/images/dialogue.png";

//experimenting with transformation
// transformation
bool transDirection = true;
float offset = 0.0f;
float increment = 0.001f;
float maxOffset = 0.9f;
// rotation
float curAngle = 0.0f;
// scaling
bool sizeDirection = true;
float maxSize = 0.8f;
float minSize = 0.1f;
float curSize = 0.3f;
// sky textures
int atSky = 0;
// dialogue
bool dialogue;
// transparent objects mode
bool Transparent = false;
//light intensity
float ambientIntensity = 0.7f;
bool sunset = false;

//setting up camera
glm::vec3 initialCameraPosition = glm::vec3(5.0f, 2.0f, -40.0f);
glm::vec3 cameraFacingDirection = glm::vec3(0.0f, 0.0f, -1.0f);
FPSCamera fpsCamera(initialCameraPosition, cameraFacingDirection);
glm::vec3 boatPos = glm::vec3(5.0f, 0.0f, -40.0f);

const double ZOOM_SENSITIVITY = -3.0f;
const double MOVE_SPEED = 5.0;
const double MOUSE_SENSITIVITY = 0.1f;

// Function prototypes
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();
bool checkCollision(float posX, float posZ);
bool checkDomeCollision(float posX, float posZ);
void moveBoat(const glm::vec3& offestpos);
void checkDialogue();

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
	if (!initOpenGL())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	//call shaders
	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("res/shaders/lighting_phong.vert", "res/shaders/lighting_phong.frag");
	// for the dialogue box
	ShaderProgram shaderProgram2;
	shaderProgram2.loadShaders("res/shaders/dialogue.vert", "res/shaders/dialogue.frag");
	// for the sun
	ShaderProgram lightShader;
	lightShader.loadShaders("res/shaders/bulb.vert", "res/shaders/bulb.frag");

	//Load meshes
	const int numModels = 11; //no of models
	Mesh mesh[numModels];
	Texture texture[numModels];

	mesh[0].loadOBJ("res/models/environment.obj");
	mesh[1].loadOBJ("res/models/plants.obj");
	mesh[2].loadOBJ("res/models/dojo.obj");
	mesh[3].loadOBJ("res/models/sea.obj");
	mesh[4].loadOBJ("res/models/panda.obj");
	mesh[5].loadOBJ("res/models/sakura.obj");
	mesh[6].loadOBJ("res/models/door.obj");
	mesh[7].loadOBJ("res/models/boat.obj");
	mesh[8].loadOBJ("res/models/mountains.obj");
	mesh[9].loadOBJ("res/models/lantern.obj");
	mesh[10].loadOBJ("res/models/bridge.obj");

	//load mesh for light
	Mesh sunMesh;
	sunMesh.loadOBJ("res/models/bulb.obj");
	Texture sunText;
	sunText.loadTexture("res/models/bulb.png", true);
	//Sun Position
	glm::vec3 sunPos = glm::vec3(-10.0f, 30.0f, 40.0f); //light


	// sky texture
	Texture sky[4];
	sky[3].loadTexture("res/models/sky_b.png", true);
	sky[0].loadTexture("res/models/sky_c.png", true);
	sky[1].loadTexture("res/models/sky_d.png", true);
	sky[2].loadTexture("res/models/sky_e.jpg", true);
	Texture skyText = sky[atSky];
	// dojo texture
	Texture dojo[4];
	dojo[3].loadTexture("res/models/dojo_b.jpg", true);
	dojo[0].loadTexture("res/models/dojo_c.jpg", true);
	dojo[1].loadTexture("res/models/dojo_d.jpg", true);
	dojo[2].loadTexture("res/models/dojo_e.jpg", true);
	Texture dojoText = dojo[atSky];
	// water texture
	Texture water[4];
	water[3].loadTexture("res/models/Sea_Texture_Example.jpeg", true);
	water[0].loadTexture("res/models/water_c.jpg", true);
	water[1].loadTexture("res/models/water_d.jpg", true);
	water[2].loadTexture("res/models/water_e.jpg", true);
	Texture waterText = water[atSky];
	// water texture
	Texture plants[4];
	plants[3].loadTexture("res/models/plants_b.jpg", true);
	plants[0].loadTexture("res/models/plants_c.jpg", true);
	plants[1].loadTexture("res/models/plants_d.jpg", true);
	plants[2].loadTexture("res/models/plants_e.jpg", true);
	Texture plantsText = water[atSky];

	//*****************
	// General Texture
	texture[4].loadTexture("res/models/panda.png", true);
	texture[5].loadTexture("res/models/sakura.png", true);
	texture[6].loadTexture("res/models/door.png", true);
	texture[7].loadTexture("res/models/boat.png", true);
	texture[8].loadTexture("res/models/mountains.png", true);
	texture[9].loadTexture("res/models/lantern.png", true);
	texture[10].loadTexture("res/models/bridge.png", true);



	// Model Position
	glm::vec3 modelPos[] = {
		glm::vec3(0.0f, 0.0f, 0.0f), //env
		glm::vec3(0.0f, 0.0f, 0.0f), //plants
		glm::vec3(0.0f, 0.0f, 0.0f), //dojo
		glm::vec3(0.0f, 0.0f, 0.0f), //floor
		glm::vec3(10.0f, 0.0f, -27.3f), //panda
		glm::vec3(8.0f, 0.0f, -20.0f), //flower
		glm::vec3(6.0f, 0.0f, -25.0f), //door
		glm::vec3(6.0f, 0.0f, 25.0f), //boat
		glm::vec3(8.0f, -2.0f, 35.0f), //mountains
		glm::vec3(7.0f, 0.0f, -9.0f), //lantern
		glm::vec3(16.0f, 0.0f, 20.0f),	//bridge


	};

	// Model Scale
	glm::vec3 modelScale[] = {
		glm::vec3(1.0f, 1.0f, 1.0f), //env
		glm::vec3(1.5f, 1.5f, 1.5f), //plants
		glm::vec3(1.5f, 1.5f, 1.5f), //dojo
		glm::vec3(1.0f, 1.0f, 1.0f), //floor
		glm::vec3(3.0f, 3.0f, 3.0f), //panda
		glm::vec3(0.2f, 0.2f, 0.2f), //flower
		glm::vec3(1.0f, 1.0f, 1.0f), //door
		glm::vec3(1.0f, 1.0f, 1.0f), //boat
		glm::vec3(1.0f, 1.2f, 0.7f), //mountains
		glm::vec3(35.0f, 35.0f, 35.0f), //lantern
		glm::vec3(60.0f, 60.0f, 60.0f),	//bridge


	};

	// set up a dialogue rectangle
	//  Set up an array of vertices for a quad (2 triangls) with an index buffer data
	//   (What is a vertex?)
	GLfloat vertices[] = {
		// positions			// texture
		 0.50f,  -0.40f, 0.0f,   1.0f, 1.0f,   // top right
		 0.50f, -0.85f, 0.0f,    1.0f, 0.0f,   // bottom right
		-0.50f, -0.85f, 0.0f,   0.0f, 0.0f,   // bottom left
		-0.50f,  -0.40f, 0.0f,   0.0f, 1.0f    // top left 
	};

	GLuint indices[] = {
		0, 1, 2,  // First Triangle
		0, 2, 3   // Second Triangle
	};


	// 2. Set up buffers on the GPU
	GLuint vbo, ibo, vao;

	glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU

	glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
	glBindVertexArray(vao);					// Make it the current one

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);			// Enable the first attribute or attribute "0"
	// texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set up index buffer
	glGenBuffers(1, &ibo);	// Create buffer space on the GPU for the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	// Texture
	Texture texture1;
	texture1.loadTexture(texture1FileName, true);

	// ******************************

	// delta time
	double lastTime = glfwGetTime();
	float cubeAngle = 0.0f;

	// ******************************
	// Rendering loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);
		double currentTime = glfwGetTime();
		double deltTime = currentTime - lastTime;
		checkDialogue();

		// Poll for and process events
		glfwPollEvents();
		update(deltTime);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the quad (two triangles)
		shaderProgram2.use();
		// setup texture
		texture1.bind(0);
		glUniform1i(glGetUniformLocation(shaderProgram2.getProgram(), "texSampler1"), 0);
		glBindVertexArray(vao);
		if (dialogue == true) {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);


		shaderProgram.use();

		if (Transparent == true)
			shaderProgram.setUniform("transparancy", glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
		else
			shaderProgram.setUniform("transparancy", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// animation
		if (transDirection)
			offset += increment;
		else
			offset -= increment;
		if (abs(offset) >= maxOffset)
			transDirection = !transDirection;

		//rotation
		curAngle += 0.005f;
		if (curAngle >= 360)
			curAngle -= 360;

		//scaling
		if (sizeDirection)
			curSize += 0.001f;
		else
			curSize -= 0.001f;

		if (curSize >= maxSize || curSize <= minSize)
			sizeDirection = !sizeDirection;

		// using delt rendering time
		cubeAngle += (float)deltTime * 50.0f;
		if (cubeAngle >= 360) cubeAngle = 0.0f;

		float sakuraMoveAmplitude = 0.5f; // Max movement range on x-axis
		float sakuraMoveSpeed = 2.0f; // Speed of oscillation
		float sakuraOffset = sakuraMoveAmplitude * sin(sakuraMoveSpeed * currentTime);


		glm::mat4 model(1.0), view(1.0), projection(1.0);

		// create the view matrix
		//view = glm::lookAt(camPos, targetPos, up);
		view = fpsCamera.getViewMatrix();
		// create the projection matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / gWindowHeight, 0.1f, 100.0f); //near cant be 0


		glm::vec3 viewPos;
		viewPos.x = fpsCamera.getPosition().x;
		viewPos.y = fpsCamera.getPosition().y;
		viewPos.z = fpsCamera.getPosition().z;

		glm::vec3 lightPos(0.0f, 2.0f, 5.0f);
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

		// the light
		//if (sunset == true) {
		//	glm::vec3 lightColor(1.0f, 1.0f, 0.8f);
		//	glm::vec3 lightPos(0.0f, 5.0f, 30.0f);
		//}

		// move the light in x direction
		cubeAngle += (float)deltTime * 50.0f;


		// pass the uniforms t othe shaders
		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);
		shaderProgram.setUniform("viewPos", viewPos);
		shaderProgram.setUniform("lightPos", lightPos);
		shaderProgram.setUniform("lightColor", lightColor);
		shaderProgram.setUniform("ambientIntensity", ambientIntensity);

		// Render the models
		for (int i = 0; i < numModels; i++) {

			glActiveTexture(GL_TEXTURE0);
			model = glm::translate(glm::mat4(1.0), modelPos[i]) *
				glm::scale(glm::mat4(1.0), modelScale[i]);

			if (i == 2 || i == 4) //rotate panda and dojo
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			if (i == 8) //rotate panda and dojo
				model = glm::rotate(model, glm::radians(-160.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			shaderProgram.setUniform("model", model);
			shaderProgram.setUniform("vertColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			// change background on B
			if (i == 0) {
				// Apply rotation around Y-axis
				model = glm::rotate(model, glm::radians(curAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				shaderProgram.setUniform("model", model);
				skyText = sky[atSky];
				skyText.bind(0);
			}
			else if (i == 1) {
				plantsText = plants[atSky];
				plantsText.bind(0);
			}
			else if (i == 2) {
				dojoText = dojo[atSky];
				dojoText.bind(0);
			}
			else if (i == 3) {
				model = glm::rotate(model, glm::radians(-curAngle), glm::vec3(0.0f, 1.0f, 0.0f));
				shaderProgram.setUniform("model", model);
				waterText = water[atSky];
				waterText.bind(0);
			}
			else if (i == 5) { //sakura flowers
				// color sakura flowers based on theme
				if (atSky == 1)
					shaderProgram.setUniform("vertColor", glm::vec4(0.96f, 0.95, 1.5f, 1.0f));
				else if (atSky == 2)
					shaderProgram.setUniform("vertColor", glm::vec4(0.54f, 0.1, 0.2f, 1.0f));
				else if (atSky == 3)
					shaderProgram.setUniform("vertColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));


				model = glm::translate(glm::mat4(1.0), glm::vec3((-10.0f + sakuraOffset), 0.0f, -10.0f)) * glm::scale(glm::mat4(1.0), modelScale[i]);
				shaderProgram.setUniform("model", model);
				texture[i].bind(0);
				mesh[i].draw();
				model = glm::translate(glm::mat4(1.0), glm::vec3((11.0f - sakuraOffset), 0.0f, -10.0f)) * glm::scale(glm::mat4(1.0), modelScale[i]);
				shaderProgram.setUniform("model", model);
				mesh[i].draw();
				model = glm::translate(glm::mat4(1.0), glm::vec3((10.0f + sakuraOffset), 0.0f, -27.3f)) * glm::scale(glm::mat4(1.0), glm::vec3(0.30f, 0.30f, 0.30f));
				shaderProgram.setUniform("model", model);
				mesh[i].draw();
				model = glm::translate(glm::mat4(1.0), glm::vec3((3.0f - sakuraOffset), 0.0f, -7.6f)) * glm::scale(glm::mat4(1.0), modelScale[i]);
				shaderProgram.setUniform("model", model);
			}
			else {
				texture[i].bind(0);
				if (i == 4) { // adjust color of panda
					if (atSky == 0)
						shaderProgram.setUniform("vertColor", glm::vec4(0.85f, 0.8f, 0.87f, 1.0f));
					else if (atSky == 1)
						shaderProgram.setUniform("vertColor", glm::vec4(0.70f, 0.85f, 0.85f, 1.0f));
					else if (atSky == 2)
						shaderProgram.setUniform("vertColor", glm::vec4(0.55f, 0.5f, 0.5f, 1.0f));
					else if (atSky == 3)
						shaderProgram.setUniform("vertColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
				}
				else if (i == 8) { // adjust color of mountains
					if (atSky == 0)
						shaderProgram.setUniform("vertColor", glm::vec4(1.0f, 1.0f, 0.8f, 1.0f));
					else if (atSky == 1)
						shaderProgram.setUniform("vertColor", glm::vec4(0.754f, 0.751f, 0.83f, 1.0f));
					else if (atSky == 2)
						shaderProgram.setUniform("vertColor", glm::vec4(0.1f, 0.25f, 0.43f, 1.0f));
					else if (atSky == 3)
						shaderProgram.setUniform("vertColor", glm::vec4(0.65f, 0.80f, 0.80f, 1.0f));

				}
				else if (i == 6 || i == 9 || i == 10) { // adjust color of door, lanterb, bridge
					if (atSky == 0)
						shaderProgram.setUniform("vertColor", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
					else if (atSky == 1)
						shaderProgram.setUniform("vertColor", glm::vec4(0.6f, 0.85f, 0.85f, 1.0f));
					else if (atSky == 2)
						shaderProgram.setUniform("vertColor", glm::vec4(0.5f, 0.3f, 0.3f, 1.0f));
					else if (atSky == 3)
						shaderProgram.setUniform("vertColor", glm::vec4(0.6f, 0.6f, 0.7f, 1.0f));

					// add an extra lantern
					if (i == 9) {
						mesh[i].draw();
						model = glm::translate(glm::mat4(1.0), glm::vec3(-8.0f, 0.0f, -6.0f)) * glm::scale(glm::mat4(1.0), modelScale[i]);
						shaderProgram.setUniform("model", model);
					}

				}
				else if (i == 7) {
					model = glm::translate(glm::mat4(1.0), boatPos) * glm::scale(glm::mat4(1.0), modelScale[i]);
					shaderProgram.setUniform("model", model);
					shaderProgram.setUniform("vertColor", glm::vec4(0.90f, 0.85f, 0.85f, 1.0f));

				}
				else
					shaderProgram.setUniform("vertColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			mesh[i].draw();
			texture[i].unbind(0);
		}


		//GLint vertexColorLocation = glGetUniformLocation(shaderProgram.getProgram(), "vertColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenColor, 0.0f, 0.0f);
		glBindVertexArray(0);


		// Render the light/bulb
		//model = glm::translate(glm::mat4(1.0), lightPos);
		//model = glm::translate(glm::mat4(1.0), lightPos) *
		//glm::scale(glm::mat4(1.0), glm::vec3(1.0, 1.0, 1.0));
		//lightShader.use();
		//lightShader.setUniform("projection", projection);
		//lightShader.setUniform("view", view);
		//lightShader.setUniform("model", model);
		//lightShader.setUniform("lightColor", lightColor);


		//bulb.bind(0);
		//lightMesh.draw();
		//bulb.unbind(0);

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(glm::mat4(1.0), sunPos);
		lightModel = glm::scale(lightModel, glm::vec3(9.0f, 9.0f, 9.0f));

		lightShader.use();
		lightShader.setUniform("projection", projection);
		lightShader.setUniform("view", view);
		lightShader.setUniform("model", lightModel);
		lightShader.setUniform("lightColor", lightColor);
		sunText.bind(0);
		sunMesh.draw();
		sunText.unbind(0);

		// Swap front and back buffers
		glfwSwapBuffers(gWindow);
		lastTime = currentTime;
	}

	// Clean up

	glfwTerminate();
	return 0;
}
//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL()
{
	// Intialize GLFW 
	// GLFW is configured.  Must be called before calling any GLFW functions
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	// Set the required callback functions
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);


	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glClearColor(0.0f, 0.0f, 0.10f, 1.0f);
	// Define the viewport dimensions
	glViewport(0, 0, gWindowWidth, gWindowHeight);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}
//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// change bg color when B is presesd
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		if (atSky == 3) {
			atSky = 0;
		}
		else {
			atSky++;
		}
	}

	// enable/disable transperancy when T is pressed
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		if (Transparent == false) {
			Transparent = true;
		}
		else {
			Transparent = false;
		}
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		ambientIntensity += 0.1f;  // Increase ambient light intensity
		if (ambientIntensity > 1.0f) ambientIntensity = 1.0f; // Clamp to maximum
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		ambientIntensity -= 0.1f;  // Decrease ambient light intensity
		if (ambientIntensity < 0.0f) ambientIntensity = 0.0f; // Clamp to minimum
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		if (sunset == false) {
			sunset = true;
		}
		else {
			sunset = false;
		}
	}

}
//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// new method on mouse scroll
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY) {
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;
	fov = glm::clamp(fov, 1.0, 120.0);
	fpsCamera.setFOV((float)fov);
}
// update
void update(double elapsedTime) {

	//camera orientation
	double mouseX, mouseY;

	//get current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	//rotate camera using delta in mouse differences
	fpsCamera.rotate(float(gWindowWidth / 2.0f - mouseX) * MOUSE_SENSITIVITY, // mapped to yaw and pitch
		float(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	//camera movement forward and backward
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS && checkCollision(fpsCamera.getPosition().x, fpsCamera.getPosition().z) == false) {
		//define front boundaries of objects
		fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
		moveBoat((float)MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
	}
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS && checkDomeCollision(fpsCamera.getPosition().x, fpsCamera.getPosition().z) == false) {
		fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());
		moveBoat((float)MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

	}

	//left right
	if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS) {
		fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
		moveBoat((float)MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());

	}
	else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS && checkCollision(fpsCamera.getPosition().x, fpsCamera.getPosition().z) == false) {
		fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
		moveBoat((float)MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());

	}

	//up down
	//if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
	//	fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
	//else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
	//	fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
}
//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);	// decimal places
		outs << std::fixed
			<< APP_TITLE << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
	}

	frameCount++;
}

//printf("%6.4lf", mPosition.z);
bool checkCollision(float posX, float posZ) {
	if ((posX < 9.5 && posX > -9.5) && (posZ > -11.5 && posZ < 8.0))
		return true;
	if (posZ > 19.0f)
		return true;
	else if ((posZ > -30.0 && posZ < -25.0f) && (posX > 9 && posX < 11.0))
		return true;
	else
		return false;
}

bool checkDomeCollision(float posX, float posZ) {
	if (posZ < -44.0f)
		return true;
	else
		return false;
}

void moveBoat(const glm::vec3& offestpos)
{
	boatPos.x += offestpos.x;
	boatPos.z += offestpos.z;
}

void checkDialogue() {
	float x = fpsCamera.getPosition().x;
	float z = fpsCamera.getPosition().z;
	if ((x > 4.0 && x < 11.0) && (z < -27.0 && z >= -36.0)) {
		dialogue = true;
	}
	else
		dialogue = false;
}