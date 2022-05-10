#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include "../headers/Scene.hpp"
#include "../headers/CameraObject.hpp"

float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationZ = 0.0f;
float rotationNoStop = 0.0f;
float speedRotation = 2.5f;

float speedBall = 9.0f;
float speedJump = 6.5f;

vec3 actualBallPosition, actualBallRotation;

PhysicsObject *physicsBallObject;
GameObject *textureBallObject;

bool wireframe_mode = false;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	// Wireframe mode (touche W)
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		wireframe_mode = !wireframe_mode;
	}

	// Jump
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		// cout << "Jump" << endl;
		// cout << physicsBallObject->onSurface << " " << deltaTime << endl;
		// Attention ! doit dépendre de la normale de la surface sur laquelle on est

		if (physicsBallObject->onSurface)
		{
			physicsBallObject->transform->setLocalTranslation(physicsBallObject->transform->getLocalTranslation() + vec3(0.0f, 0.0f, 0.01f));
			physicsBallObject->velocity.z = 0.0f;
			physicsBallObject->velocity += vec3(0.0f, 0.0f, speedJump);
			physicsBallObject->onSurface = false;

			// physicsBallObject->acceleration += vec3(0.0f, 0.0f, speedJump) * deltaTime;
		}
		// physicsBallObject->velocity += vec3(0.0f, 0.0f, speedJump) * deltaTime;
	}

	// Change to Material 1
	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
	{
		cout << "Change to Material 1" << endl;
	}
	// Change to Material 2
	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
	{
		cout << "Change to Material 2" << endl;
	}
	// Change to Material 3
	if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
	{
		cout << "Change to Material 3" << endl;
	}
}
class InputManager
{

public:
	GLFWwindow *window;
	Scene *scene;
	CameraObject *cam;

public:
	InputManager(GLFWwindow *window, CameraObject *cam, Scene *scene)
	{
		this->window = window;
		this->cam = cam;
		this->scene = scene;

		// physicsBallObject = this->scene->PhysicsObjectList[1];
		// textureBallObject = this->scene->objects[2];
		physicsBallObject = this->scene->getPlayer();
		// cout << physicsBallObject->id << endl;
		textureBallObject = physicsBallObject->rotationObject;
	}

	~InputManager()
	{
		delete this;
	}


	void updateBallPositionAndRotation()
	{
		actualBallPosition = physicsBallObject->transform->getLocalTranslation();
		// vec3 actualBallPosition = this->scene->PhysicsObjectList[0]->transform->getLocalTranslation();
		actualBallRotation = textureBallObject->transform->getLocalRotation();
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void processInput(float deltaTime)
	{
		// CAMERA camera = this->scene->cameras[0];
		// std::cout << "camera position 0 : " << camera.position[2] << std::endl;

		GLFWwindow *window = this->window;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glm::vec3 camera_right = glm::vec3(1.0f, 0.0f, 0.0f);

		// if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		//   		wireframe_mode = !wireframe_mode;

		// rotationX
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			rotationX += 1;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			rotationX -= 1;

		// rotationY
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			rotationY += 1;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			rotationY -= 1;

		// rotationZ
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			rotationZ += 1;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			rotationZ -= 1;

		// Camera zoom in and out
		float cameraSpeed = 25 * deltaTime;

		Transform *camTransform = this->cam->transform;
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			// this->scene->cameras[0].position -= cameraSpeed * this->scene->cameras[0].target;
			camTransform->translation -= cameraSpeed * this->cam->target;
		camTransform = this->cam->transform;
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			camTransform->translation += cameraSpeed * this->cam[0].target;
		camTransform = this->cam->transform;
		// glfwSetScrollCallback(window, scroll_callback);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camTransform->translation += glm::normalize(glm::cross(this->cam->up, this->cam->target)) * cameraSpeed;
		camTransform = this->cam->transform;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camTransform->translation -= glm::normalize(glm::cross(this->cam->up, this->cam->target)) * cameraSpeed;
		camTransform = this->cam->transform;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camTransform->translation += glm::normalize(glm::cross(camera_right, this->cam->target)) * cameraSpeed;
		camTransform = this->cam->transform;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camTransform->translation -= glm::normalize(glm::cross(camera_right, this->cam->target)) * cameraSpeed;
		camTransform = this->cam->transform;

		updateBallPositionAndRotation();
		if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		{
			physicsBallObject->velocity += vec3(-speedBall, 0.0f, 0.0f) * deltaTime;
			textureBallObject->transform->setLocalRotation(actualBallRotation - vec3(0.0f, speedRotation, 0.0f));
			// float radiusPlayer = textureBallObject->mesh->size * textureBallObject->transform->getLocalScale().x;
			// float radius = radiusPlayer / (textureBallObject->mesh->size * textureBallObject->transform->getLocalScale().x);
			// textureBallObject->transform->setLocalRotation(actualBallRotation - vec3(0, -(physicsBallObject->velocity.x) * deltaTime * 150 * radius, 0));
		}
		updateBallPositionAndRotation();
		if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		{
			physicsBallObject->velocity += vec3(0.0f, -speedBall, 0.0f) * deltaTime;
			textureBallObject->transform->setLocalRotation(vec3(actualBallRotation.x + speedRotation, 0.0f, 0.0f));
			/*float radiusPlayer = textureBallObject->mesh->size * textureBallObject->transform->getLocalScale().x;
			float radius = radiusPlayer / (textureBallObject->mesh->size * textureBallObject->transform->getLocalScale().x);*/
			// textureBallObject->transform->setLocalRotation(vec3(actualBallRotation - vec3(physicsBallObject->velocity.y * deltaTime * 150 * radius, 0, 0)));
		}
		updateBallPositionAndRotation();
		if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		{
			physicsBallObject->velocity += vec3(speedBall, 0.0f, 0.0f) * deltaTime;
			textureBallObject->transform->setLocalRotation(actualBallRotation - vec3(0.0f, -speedRotation, 0.0f));
			// float radiusPlayer = textureBallObject->mesh->size * textureBallObject->transform->getLocalScale().x;
			// float radius = radiusPlayer / (textureBallObject->mesh->size * textureBallObject->transform->getLocalScale().x);
			// textureBallObject->transform->setLocalRotation(actualBallRotation - vec3(0, -(physicsBallObject->velocity.x) * deltaTime * 150 * radius, 0));
		}
		updateBallPositionAndRotation();
		if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		{
			physicsBallObject->velocity += vec3(0.0f, speedBall, 0.0f) * deltaTime;
			textureBallObject->transform->setLocalRotation(vec3(actualBallRotation.x - speedRotation, 0.0f, 0.0f));
			// float radiusPlayer = textureBallObject->mesh->size * textureBallObject->transform->getLocalScale().x;
			// float radius = radiusPlayer / (textureBallObject->mesh->size * textureBallObject->transform->getLocalScale().x);
			// textureBallObject->transform->setLocalRotation(vec3(actualBallRotation - vec3(physicsBallObject->velocity.y * deltaTime * 150 * radius, 0, 0)));
		}
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow *window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
};