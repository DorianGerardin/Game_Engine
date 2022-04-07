#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include "Scene.cpp"

float rotationX = 0.;
float rotationY = 0.;
float rotationZ = 0.;
float rotationNoStop = 0.;
float speedRotation = 1.;

float speedBall = 0.2;

class InputManager {

	public:
		GLFWwindow* window;
		Scene* scene;

	public:

		InputManager(GLFWwindow* window, Scene* scene) {
			this->window = window;
			this->scene = scene;
		}

		~InputManager() {
			delete this;
		}

		/*static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{
		    //float cameraSpeed = 100 * deltaTime;
		    float cameraSpeed = 10;
		    CAMERA camera = static_cast<InputManager *>(scene)->cameras[0];
		    if (yoffset == -1) {
		        camera.position -= cameraSpeed * camera.target;
		    }
		    if (yoffset == 1) { 
		        camera.position += cameraSpeed * camera.target;
		    }
		}*/

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
		void processInput(float deltaTime)
		{
			//CAMERA camera = this->scene->cameras[0];
			//std::cout << "camera position 0 : " << camera.position[2] << std::endl;

			GLFWwindow* window = this->window;
		    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		        glfwSetWindowShouldClose(window, true);

		    glm::vec3 camera_right = glm::vec3(1.0f, 0.0f,  0.0f);

		    //rotationX
		    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) rotationX += 1;
		    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)rotationX -= 1;

		    //rotationY
		    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationY += 1;
		    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationY -= 1;

		    //rotationZ
		    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rotationZ += 1;
		    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rotationZ -= 1;

		    //Camera zoom in and out
		    float cameraSpeed = 25 * deltaTime;

		    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) this->scene->cameras[0].position -= cameraSpeed * this->scene->cameras[0].target;
		    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) this->scene->cameras[0].position += cameraSpeed * this->scene->cameras[0].target;
		    //glfwSetScrollCallback(window, scroll_callback);

		    //TODO add translations
		    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		        this->scene->cameras[0].position += glm::normalize(glm::cross(this->scene->cameras[0].up, this->scene->cameras[0].target)) * cameraSpeed;
		    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		        this->scene->cameras[0].position -= glm::normalize(glm::cross(this->scene->cameras[0].up, this->scene->cameras[0].target)) * cameraSpeed;
		    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		        this->scene->cameras[0].position += glm::normalize(glm::cross(camera_right, this->scene->cameras[0].target)) * cameraSpeed;
		    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		        this->scene->cameras[0].position -= glm::normalize(glm::cross(camera_right, this->scene->cameras[0].target)) * cameraSpeed;
			
			vec3 actualPosition = this->scene->objects[1]->transform->getLocalTranslation();
		    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		    	this->scene->objects[1]->transform->setLocalTranslation(vec3( actualPosition[0], actualPosition[1], actualPosition[2] - speedBall ));
		    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		    	this->scene->objects[1]->transform->setLocalTranslation(vec3( actualPosition[0] - speedBall, actualPosition[1], actualPosition[2] ));
		    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		    	this->scene->objects[1]->transform->setLocalTranslation(vec3( actualPosition[0], actualPosition[1], actualPosition[2] + speedBall ));
		    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		    	this->scene->objects[1]->transform->setLocalTranslation(vec3( actualPosition[0] + speedBall, actualPosition[1], actualPosition[2]));
		    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
		    	this->scene->objects[1]->transform->setLocalTranslation(vec3( actualPosition[0], actualPosition[1] - speedBall, actualPosition[2]));
		    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
		    	this->scene->objects[1]->transform->setLocalTranslation(vec3( actualPosition[0], actualPosition[1] + speedBall, actualPosition[2]));
		}

		// glfw: whenever the window size changed (by OS or user resize) this callback function executes
		// ---------------------------------------------------------------------------------------------
		void framebuffer_size_callback(GLFWwindow* window, int width, int height)
		{
		    // make sure the viewport matches the new window dimensions; note that width and
		    // height will be significantly larger than specified on retina displays.
		    glViewport(0, 0, width, height);
		}
};