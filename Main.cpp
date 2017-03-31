//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include <time.h>
#include <SOIL.h>

using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
clock_t time0, time1;
bool WIREFRAME = false;
int screenWithd, screenHeight;
float timer5;
GLint locScale;
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



int main() {
	
	//initGLFW
	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	//comprobar que GLFW esta activo
	if (!glfwInit())
		std::exit(EXIT_FAILURE);

	//crear la ventana
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}


	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);

																 //que funcion se llama cuando se detecta una pulsaci�n de tecla en la ventana x
	glfwSetKeyCallback(window, key_callback);


	//cargamos los shader
	//GLuint programID = LoadShaders("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	Shader NewShader("./src/SimpleVertexShaderV2.VERTEXSHADER", "./src/SimpleFragmentShader.fragmentshader");
	// Definir el buffer de vertices
	GLfloat squareVertex[]{
		-0.5f, -0.75f, 0.0f,
		0.5f, -0.75f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};

	GLfloat VertexBufferObject[] = {
		0.5f,  0.5f, 0.0f,  // Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left 
	};

	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};

	

	// Definir el EBO
	GLuint IndexBufferObject[]{
		3,0,2,
		0,1,2 };

	// Crear los VBO, VAO y EBO

	GLuint VBO;
	GLuint EBO;

	GLuint VAO;
	//reservar memoria para el VAO, VBO y EBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VAO);

	//Declarar el VBO y el EBO

	//Enlazar el buffer con openGL
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Alocamos memoria suficiente para almacenar 4 grupos de 3 floats (segundo parámetro)
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, VertexBufferObject, GL_STATIC_DRAW);
	//Alocamos ahora el EBO()
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, IndexBufferObject, GL_STATIC_DRAW);
	//Establecer las propiedades de los vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);

	//liberar el buffer	
	//liberar el buffer de vertices
	glBindVertexArray(0);

	GLint variableShader = glGetUniformLocation(NewShader.Program, "vertexOffset");
	GLint variableShader2 = glGetUniformLocation(NewShader.Program, "IndexInUse");
	//Bucle de dibujado (VENTANA)

	
	
	//Timer
	/*time1 = clock();
	float delta = static_cast<float>(static_cast<double>(time1 - time0) / static_cast<double>(CLOCKS_PER_SEC));
	if (delta > 0.00005f)
	{
		time0 = clock();
		timer5 += delta;
		if (timer5>5.0f) timer5 = 0.0f;
	}*/
	bool decrease = false;
	while (!glfwWindowShouldClose(window)) {
		
		time1 = clock();
		
		float delta = static_cast<float>(static_cast<double>(time1 - time0) / static_cast<double>(CLOCKS_PER_SEC));
		if (delta > 0.00005f)
		{
			time0 = clock();
			if (decrease == true) {
				timer5 -= delta;
			}
			else timer5 += delta;

			if (timer5 >= 2.0f) {
				decrease = true;
			}
			else if (timer5 <= 0.0f) {
				decrease = false;
			}
		}

		glViewport(0, 0, screenWithd, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.2f, 0.6f, 1.0f, 1.f);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10, 10, -10.f, 10.f, -1.0f, 10.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Establecer el shader

		NewShader.Use();

		float vertexOffset = abs(glfwGetTime()) * sin(1);

		//glUniform1fv(variableShader, vertexOffset);
		
		//glUniform1i(variableShader2, );
		glUniform1f(variableShader, timer5 / 5.0f);

		if (WIREFRAME == true) {

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//pintar el VAO
		glBindVertexArray(VAO);
		// bind index buffer if you want to render indexed data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// indexed draw call
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);


		// Swap the screen buffers
		glfwSwapBuffers(window);
		//Event poll
		glfwPollEvents();
	}

	// liberar la memoria de los VAO, EBO y VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Terminate GLFW, clearing any resources allocated by GLFW.

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
}


void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Cuando pulsamos la tecla ESC se cierra la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Cuando apretamos la tecla W se cambia a modo Wireframe
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		WIREFRAME = !WIREFRAME;
}