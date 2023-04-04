#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm/glm.hpp" //core glm functionality
#include "glm/glm/gtc/matrix_transform.hpp" //glm extension for generating common transformation matrices
#include "glm/glm/gtc/matrix_inverse.hpp" //glm extension for computing inverse matrices
#include "glm/glm/gtc/type_ptr.hpp" //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"

#include <iostream>

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;

// shader uniform locations
GLuint modelLoc;
GLuint viewLoc;
GLuint projectionLoc;
GLuint normalMatrixLoc;
GLuint lightDirLoc;
GLuint lightColorLoc;

// camera
gps::Camera myCamera(
    glm::vec3(2.5732f, -0.39875f, 2.0299f),
    glm::vec3(3.0901f, -0.43882f, 2.2508f),
    glm::vec3(0.0f, 1.0f, 0.0f));

gps::Camera cameraUrm(
    glm::vec3(3.0901f, -0.43882f, 2.2508f),
    glm::vec3(3.3114f, -0.40267f, -0.40267f),
    glm::vec3(0.0f, 1.0f, 0.0f));


int moves = 0;
bool presentationEn = false;
bool lightEn = false;
GLfloat lightAngle;

glm::mat4 lightRotation;

glm::vec3 lightPos;
glm::vec3 lightPosTransformed;

GLfloat cameraSpeed = 0.1f;
GLboolean pressedKeys[1024];

// models
gps::Model3D teapot;
GLfloat angle;

gps::Model3D balerina;
GLfloat angleBalerina = 0.0f;
float deltaBalerina = 0.1f;

gps::Model3D avion;
GLfloat angleAvion = 0.0f;
float deltaAvion = 0.1f;

gps::Model3D fata;
GLfloat angleFata = 0.0f;
float deltaFata = 0.1f;

gps::Model3D stanga;
GLfloat angleStanga = 0.0f;
float deltaStanga = 0.1f;

gps::Model3D dreapta;
GLfloat angleDreapta = 0.0f;
float deltaDreapta = 0.1f;


gps::Model3D bec;

// shaders
gps::Shader myBasicShader;

gps::Shader lightingShader;

bool fogEnable = false;
float fogDensity = 0.0f;


GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
	//TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

      // presentation animation
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            presentationEn = !presentationEn;
            moves = 0;
        }


	if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        } else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    //TODO
}

void processMovement() {
	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		//update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

    if (pressedKeys[GLFW_KEY_UP]) {
        myCamera.move(gps::MOVE_UP, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_DOWN]) {
        myCamera.move(gps::MOVE_DOWN, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_Q]) {
        angle -= 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        // update normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    }

    if (pressedKeys[GLFW_KEY_E]) {
        angle += 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        // update normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    }

    if (pressedKeys[GLFW_KEY_F]) {
        fogEnable = true;
    }

    if (pressedKeys[GLFW_KEY_G]) {
        fogEnable = false;
    }

    if (pressedKeys[GLFW_KEY_1]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (pressedKeys[GLFW_KEY_2]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (pressedKeys[GLFW_KEY_3]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    if (pressedKeys[GLFW_KEY_4]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);
    }

    // presentation animation
    if (pressedKeys[GLFW_KEY_P]) {
        
        presentationEn = true;
        printf("from P %s\n", presentationEn ? "true" : "false");
        moves = 0;
    }

    if (pressedKeys[GLFW_KEY_O]) {

        presentationEn = false;
        printf("from O %s\n", presentationEn ? "true" : "false");
        moves = 0;
    }
    
    if (pressedKeys[GLFW_KEY_N]) {
        lightColor += glm::vec3(0.2f, 0.2f, 0.2f);
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    }

    if (pressedKeys[GLFW_KEY_M]) {
        lightColor -= glm::vec3(0.2f, 0.2f, 0.2f);
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    }

    // rotate around y 
    if (pressedKeys[GLFW_KEY_Y]) {
        myCamera.rotate(0.0f, cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_U]) {
        myCamera.rotate(0.0f, -cameraSpeed);
    }

    // rotate around x 
    if (pressedKeys[GLFW_KEY_X]) {
        myCamera.rotate(cameraSpeed, 0.0f);
    }

    if (pressedKeys[GLFW_KEY_C]) {
        myCamera.rotate(-cameraSpeed, 0.0f);
    }

}

void initOpenGLWindow() {
    myWindow.Create(1600, 700, "Mountain Village");
}

void setWindowCallbacks() {
	glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels() {
    teapot.LoadModel("textures/scena.obj");
    balerina.LoadModel("textures/balerina.obj");
    avion.LoadModel("textures/avion.obj");
    fata.LoadModel("textures/fata.obj");
    stanga.LoadModel("textures/stanga.obj");
    dreapta.LoadModel("textures/dreapta.obj");
}

void initShaders() {
	myBasicShader.loadShader("shaders/basic.vert", "shaders/basic.frag");
    lightingShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
}


void initUniforms() {
	myBasicShader.useShaderProgram();

    // create model matrix for teapot
    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

	// get view matrix for current camera
	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
	// send view matrix to shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

	// create projection matrix
	projection = glm::perspective(glm::radians(45.0f), (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,  0.1f, 20.0f);
	projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
	// send projection matrix to shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));	

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
	// send light dir to shader
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));
    lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    lightColor = glm::vec3(0.5f, 0.5f, 0.5f); //white light
	lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");

	// send light color to shader
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
}

void renderTeapot(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    teapot.Draw(shader);

    lightingShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void renderBalerina(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    if (deltaBalerina < 0.020f) {
        deltaBalerina += 0.001f;
    }
    else {
        deltaBalerina = 0.0f;
    }

    // deltaBalerina += 0.001f;
    angleBalerina += deltaBalerina;


    glm::mat4 model1 = glm::translate(glm::vec3(0, 0, deltaBalerina));

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
    
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model1));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    balerina.Draw(shader);
}

void renderAvion(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    if (deltaAvion < 2.0f) {
        deltaAvion += 0.01f;
    }
    else {
        deltaAvion = 0.0f;
    }

    angleAvion += deltaAvion;


    glm::mat4 model1 = glm::translate(glm::vec3(deltaAvion*1.4f, 0, deltaAvion));

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model1));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    avion.Draw(shader);
}

void renderFata(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    if (deltaFata < 2.0f) {
        deltaFata += 0.01f;
    }
    else {
        deltaFata = 0.0f;
    }

    angleFata += deltaFata;

    glm::mat4 model1 = glm::translate(glm::vec3(deltaFata * 1.4f, 0, deltaFata));

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model1));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    fata.Draw(shader);
}

void renderStanga(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    if (deltaStanga < 2.0f) {
        deltaStanga += 0.01f;
    }
    else {
        deltaStanga = 0.0f;
    }

    angleStanga += deltaStanga;

    glm::mat4 model1 = glm::translate(glm::vec3(deltaStanga * 1.4f, 0, deltaStanga));

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model1));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    stanga.Draw(shader);
}

void renderDreapta(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    if (deltaDreapta < 2.0f) {
        deltaDreapta += 0.01f;
    }
    else {
        deltaDreapta = 0.0f;
    }

    angleDreapta += deltaDreapta;

    glm::mat4 model1 = glm::translate(glm::vec3(deltaDreapta * 1.4f, 0, deltaDreapta));

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model1));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    dreapta.Draw(shader);
}

void fog() {
    myBasicShader.useShaderProgram();

    if (fogEnable == true)
        fogDensity = 0.09f;
    else
        fogDensity = 0.0f;

    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "fogDensity"), fogDensity);
}

// presentation animation
int currLoop = 0, endLoop = 300;
float t = 1.0f / endLoop;
bool finishMove = false;

void pointsBetween() {

    if (currLoop < endLoop) {
        finishMove = false;
        glm::vec3 cameraPos = myCamera.getPosition();
        glm::vec3 cameraUrmPos = cameraUrm.getPosition();
        glm::vec3 cameraTar = myCamera.getTarget();
        glm::vec3 cameraUrmTar = cameraUrm.getTarget();

        glm::vec3 newcameraPos = (1 - t) * cameraPos + t * cameraUrmPos;
        glm::vec3 newcameraTar = (1 - t) * cameraTar + t * cameraUrmTar;

        myCamera.setPosition(newcameraPos);
        myCamera.setTarget(newcameraTar);
        currLoop++;
        return;
    }
    else {
        finishMove = true;
        currLoop = 0;
    }
}

void animation() {

    if (presentationEn == true) {

        switch (moves) {
        case 0:
            cameraUrm = gps::Camera(
                glm::vec3(3.0901f, -0.43882f, 2.2508f),
                glm::vec3(3.3114f, -0.40267f, -0.40267f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 1:
            cameraUrm = gps::Camera(
                glm::vec3(3.3114f, -0.40267f, -0.40267f),
                glm::vec3(3.2927f, -0.52161f, -2.0448f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 2:
            cameraUrm = gps::Camera(
                glm::vec3(3.2927f, -0.52161f, -2.0448f),
                glm::vec3(-0.54231f, -0.52161f, -2.7479f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 3:
            cameraUrm = gps::Camera(
                glm::vec3(-0.54231f, -0.52161f, -2.7479f),
                glm::vec3(-2.1444f, -0.52296f, -1.0851f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case 4:
            cameraUrm = gps::Camera(
                glm::vec3(-2.1444f, -0.52296f, -1.0851f),
                glm::vec3(-2.8697f, -0.52938f, 1.6558f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 5:
            cameraUrm = gps::Camera(
                glm::vec3(-2.8697f, -0.52938f, 1.6558f),
                glm::vec3(-2.7085f, -0.52388f, 2.649f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 6:
            cameraUrm = gps::Camera(
                glm::vec3(-2.7085f, -0.52388f, 2.649f),
                glm::vec3(-1.2975f, -0.53357f, 3.7211f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 7:
            cameraUrm = gps::Camera(
                glm::vec3(-1.2975f, -0.53357f, 3.7211f),
                glm::vec3(0.99932f, -0.53357f, 4.7551f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 8:
            cameraUrm = gps::Camera(
                glm::vec3(0.99932f, -0.53357f, 4.7551f),
                glm::vec3(1.7481f, -0.53161f, 5.9908f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 9:
            cameraUrm = gps::Camera(
                glm::vec3(1.7481f, -0.53161f, 5.9908f),
                glm::vec3(2.6446f, -0.6617f, 6.1071f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        default: return;
        }
        pointsBetween();
        if (finishMove == true)
            moves++;

        // disable presentation animation when finished
        if (moves > 9)
            presentationEn == false;
    }
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    myBasicShader.useShaderProgram();

    // presentation animation
    animation();

    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    lightPosTransformed = glm::mat3(lightRotation) * lightPos;
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightPosTransformed));

    lightingShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    
    bec.Draw(lightingShader);

	//render the scene
    fog();

	// render the teapot
	renderTeapot(myBasicShader);
    renderBalerina(myBasicShader);
    renderAvion(myBasicShader);
    renderFata(myBasicShader);
    renderStanga(myBasicShader);
    renderDreapta(myBasicShader);

}

void cleanup() {
    myWindow.Delete();
    //cleanup code for your own data
}

int main(int argc, const char * argv[]) {

    try {
        initOpenGLWindow();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
	initModels();
	initShaders();
	initUniforms();
    setWindowCallbacks();

	glCheckError();   

	// application loop
	while (!glfwWindowShouldClose(myWindow.getWindow())) {

        animation();

        processMovement();
	    renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());

		glCheckError();
	}

	cleanup();

    return EXIT_SUCCESS;
}
