// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../common/stb_image.h"
//#include "tools/Object.cpp"
#include "src/InputManager.cpp"

// void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

/*******************************************************************************/

int main(void)
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Equilibrium - GLFW", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Dark blue background
    glClearColor(0.73f, 0.95f, 0.98f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    // glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // --------------------------------------------------------------------------------------------
    // |                                      TEXTURES                                            |
    // --------------------------------------------------------------------------------------------

    GLuint earth_texture = loadBMP_custom("textures/earthTexture.bmp");
    GLuint sun_texture = loadBMP_custom("textures/sunTexture.bmp");
    GLuint rock_texture = loadBMP_custom("textures/rock.bmp");
    GLuint parquet_texture = loadBMP_custom("textures/parquet.bmp");

    GLuint TextureIDRock = glGetUniformLocation(programID, "hmapSampler");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rock_texture);
    glUniform1i(TextureIDRock, 0);

    // GLuint TextureIDHmap = glGetUniformLocation(programID,"hmapSampler");
    GLuint TextureID = glGetUniformLocation(programID, "texSampler");

    // Get a handle for our "LightPosition" uniform
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    GLint modelID = glGetUniformLocation(programID, "modelMatrix");

    GLint useHeightMapID = glGetUniformLocation(programID, "useHeightMap");

    GLint hasTexture = glGetUniformLocation(programID, "hasTexture");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    float earthGravity = 9.81;
    // --------------------------------------------------------------------------------------------
    // |                                         SCENE                                            |
    // --------------------------------------------------------------------------------------------

    Scene *scene = new Scene();
    /*InputManager *inputManager = new InputManager(window, scene);*/
    // glfwSetKeyCallback(window, inputManager->key_callback);
    // CAMERA camera = inputManager->scene->cameras[0];

    unique_ptr<LightObject> light_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light = light_uniquePtr.get();

    // unique_ptr<GameObject> plane_uniquePtr = make_unique<GameObject>("GO_Plan", "objects/plane_surface.off", modelID, programID);
    unique_ptr<GameObject> plane_uniquePtr = make_unique<GameObject>("GO_Plan", "objects/plane_surface_relief.off", modelID, programID);
    GameObject *terrain = plane_uniquePtr.get();
    unique_ptr<GameObject> EarthRotation_uniquePtr = make_unique<GameObject>("PO_EarthRotation", SPHERE, 50., modelID, programID);
    GameObject *EarthRotation = EarthRotation_uniquePtr.get();

    unique_ptr<GameObject> Earth_uniquePtr = make_unique<GameObject>("GO_Earth", SPHERE, 50., modelID, programID);
    GameObject *Earth = Earth_uniquePtr.get();

    // CameraObject *cam = scene->cameras2[0];
    // vec3 position = vec3(1.75f, 1.f, 4.5f);
    vec3 target = vec3(0.0f, 0.0f, -1.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    unique_ptr<CameraObject> defaultCamera_ptr = make_unique<CameraObject>("CO_Camera", target, up);
    CameraObject *cam = defaultCamera_ptr.get();
    cam->ToDraw(false);

    unique_ptr<PhysicsObject> Sun_uniquePtr = make_unique<PhysicsObject>("PO_SunFall", SPHERE, 50., modelID, programID, 0.00001f, -0.3f);
    PhysicsObject *Sun = Sun_uniquePtr.get();

    // terrain->applyTexture(rock_texture, TextureID);
    EarthRotation->applyTexture(earth_texture, TextureID);
    Earth->ToDraw(false);
    EarthRotation->applyTexture(earth_texture, TextureID);
    Sun->applyTexture(sun_texture, TextureID);

    terrain->transform->setLocalScale(vec3(4.0f, 4.0f, 4.0f));
    terrain->transform->setLocalRotation(vec3(-90.0f, 0.0f, 0.0f));

    Earth->transform->setLocalScale(vec3(0.05f, 0.05f, 0.05f));
    // Earth->transform->setLocalTranslation(vec3(.5, .5, 0));

    Sun->transform->setLocalScale(vec3(0.1f, 0.1f, 0.1f));
    Sun->transform->setLocalTranslation(vec3(2.0f, 2.0f, -2.0f));

    cam->transform->setLocalTranslation(vec3(0, -20, 10));
    cam->transform->setLocalRotation(vec3(70, 0, 0));

    terrain->addChild(move(Earth_uniquePtr));
    Earth->addChild(move(EarthRotation_uniquePtr));
    Earth->addChild(move(defaultCamera_ptr));

    scene->addLight(light);
    scene->addCamera2(cam);
    scene->addObject(terrain);
    scene->addObject(Earth);
    scene->addObject(EarthRotation);
    scene->addPhysicsObject(Sun);

    InputManager *inputManager = new InputManager(window, cam, scene);

    do
    {

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (inputManager->wireframe_mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // input
        // -----
        inputManager->processInput(deltaTime);
        // CAMERA camera = inputManager->scene->cameras[0];

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        //-------------------------------------------------------------------------------------------------
        GLint cameraPosID = glGetUniformLocation(programID, "cameraPos");
        glUniformMatrix3fv(cameraPosID, 1, GL_FALSE, &cam->transform->getLocalTranslation().x);

        scene->Step(deltaTime);
        vec3 actualEarthPosition = Earth->transform->getLocalTranslation();
        Earth->transform->setLocalTranslation(vec3(actualEarthPosition.x, actualEarthPosition.y, Earth->heightInTriangle(terrain->mesh) + 1.0 * Earth->transform->getLocalScale().x));

        vec3 actualCamRotation = cam->transform->getLocalRotation();
        cam->transform->setLocalRotation(vec3(actualCamRotation.x, actualCamRotation.y, actualCamRotation.z));

        terrain->updateSelfAndChild();
        Earth->updateSelfAndChild();
        Sun->updateSelfAndChild();

        //-------------------------------------------------------------------------------------------------

        // cout << "cam world position : " << glm::to_string(cam->transform->getWorldTranslation()) << endl;

        // glm::mat4 viewMatrix = glm::lookAt(cam->transform->getLocalTranslation(), cam->transform->getLocalTranslation() + cam->target, cam->up);
        // glm::mat4 viewMatrix = glm::lookAt(cam->transform->getWorldTranslation(), cam->transform->getWorldTranslation() + cam->target, cam->up);
        glm::mat4 viewMatrix = inverse(cam->transform->getModelMatrix());

        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationX), glm::vec3(1, 0, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationY), glm::vec3(0, 1, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationZ), glm::vec3(0, 0, 1));

        glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.f);

        GLint viewID = glGetUniformLocation(programID, "viewMatrix");
        GLint projectionID = glGetUniformLocation(programID, "projectionMatrix");

        // glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);

        scene->draw();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
