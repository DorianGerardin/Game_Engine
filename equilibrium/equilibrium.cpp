// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <limits> //infinity

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

    Material crystal;
    crystal.albedo = "textures/crystal/albedo.bmp";
    crystal.normal    = "textures/crystal/normal.bmp";
    crystal.metallic  = "textures/crystal/metallic.bmp";
    crystal.roughness = "textures/crystal/roughness.bmp";
    crystal.ao        = "textures/crystal/ao.bmp";

    Material abstract;
    abstract.albedo = "textures/abstract1/albedo.bmp";
    abstract.normal    = "textures/abstract1/normal.bmp";
    abstract.metallic  = "textures/abstract1/metallic.bmp";
    abstract.roughness = "textures/abstract1/roughness.bmp";
    abstract.ao        = "textures/abstract1/ao.bmp";

    Material fabric;
    fabric.albedo = "textures/fabric1/albedo.bmp";
    fabric.normal    = "textures/fabric1/normal.bmp";
    fabric.metallic  = "textures/fabric1/metallic.bmp";
    fabric.roughness = "textures/fabric1/roughness.bmp";
    fabric.ao        = "textures/fabric1/ao.bmp";

    Material ice;
    ice.albedo = "textures/ice/albedo.bmp";
    ice.normal    = "textures/ice/normal.bmp";
    ice.metallic  = "textures/ice/metallic.bmp";
    ice.roughness = "textures/ice/roughness.bmp";
    ice.ao        = "textures/ice/ao.bmp";

    GLuint earth_texture = loadBMP_custom("textures/earthTexture.bmp");
    GLuint moon_texture = loadBMP_custom("textures/moonTexture.bmp");
    GLuint sun_texture = loadBMP_custom("textures/sunTexture.bmp");
    GLuint rock_texture = loadBMP_custom("textures/rock.bmp");
    GLuint parquet_texture = loadBMP_custom("textures/parquet.bmp");
    GLuint smiley_texture = loadBMP_custom("textures/smiley.bmp");
    GLuint dice_texture = loadBMP_custom("textures/dice.bmp");
    GLuint skybox_texture = loadBMP_custom("textures/skybox.bmp");

    GLuint TextureIDRock = glGetUniformLocation(programID, "hmapSampler");
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, rock_texture);
    // glUniform1i(TextureIDRock, 0);

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
    float earthGravity = 9.81f;
    // float gravity = earthGravity;
    float gravity = 2.5f;

    // --------------------------------------------------------------------------------------------
    // |                                         SCENE                                            |
    // --------------------------------------------------------------------------------------------

    Scene *scene = new Scene();
    /*InputManager *inputManager = new InputManager(window, scene);*/
    // CAMERA camera = inputManager->scene->cameras[0];

    // --------------------
    // Déclaration des Objets
    unique_ptr<LightObject> light_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light = light_uniquePtr.get();

    unique_ptr<GameObject> skybox_uniquePtr = make_unique<GameObject>("GO_SkyBox", SPHERE, 200.0f, modelID, programID);
    GameObject *skybox = skybox_uniquePtr.get();

    // CameraObject *cam = scene->cameras2[0];
    // vec3 position = vec3(1.75f, 1.f, 4.5f);
    vec3 target = vec3(0.0f, 0.0f, -1.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    unique_ptr<CameraObject> defaultCamera_ptr = make_unique<CameraObject>("CO_Camera", target, up);
    CameraObject *cam = defaultCamera_ptr.get();
    cam->ToDraw(false);

    unique_ptr<PhysicsObject> plane_uniquePtr = make_unique<PhysicsObject>("PO_Plan", PLANE, 1.0f, modelID, programID, 1.0f, 0.0f, true);
    PhysicsObject *terrain1 = plane_uniquePtr.get();
    // unique_ptr<PhysicsObject> plane_uniquePtr = make_unique<PhysicsObject>("PO_Plan", "objects/plane_surface.off", modelID, programID, 0.0f, 0.0f);
    // unique_ptr<PhysicsObject> plane_uniquePtr = make_unique<PhysicsObject>("PO_Plan", "objects/plane_surface_relief.off", modelID, programID, 0.0f, 0.0f);
    unique_ptr<PhysicsObject> plane2_uniquePtr = make_unique<PhysicsObject>("PO_Plan", PLANE, 1.0f, modelID, programID, 1.0f, 0.0f, true);
    PhysicsObject *terrain2 = plane2_uniquePtr.get();
    unique_ptr<PhysicsObject> plane3_uniquePtr = make_unique<PhysicsObject>("PO_Plan", PLANE, 1.0f, modelID, programID, 1.0f, 0.0f, true);
    PhysicsObject *terrain3 = plane3_uniquePtr.get();
    // unique_ptr<PhysicsObject> rampe_uniquePtr = make_unique<PhysicsObject>("PO_Plan", PLANE, 1.0f, modelID, programID, 1.0f, 0.0f, true);
    // PhysicsObject *rampe = rampe_uniquePtr.get();

    unique_ptr<PhysicsObject> Earth_uniquePtr = make_unique<PhysicsObject>("PO_Earth", SPHERE, 1.0f, modelID, programID, 5.0f, -gravity, false);
    PhysicsObject *Earth = Earth_uniquePtr.get();
    Earth->addVisualSphereRotation();
    // unique_ptr<GameObject> EarthRotation_uniquePtr = make_unique<GameObject>("GO_EarthRotation", SPHERE, 1.0f, modelID, programID);
    // GameObject *EarthRotation = EarthRotation_uniquePtr.get();

    unique_ptr<PhysicsObject> Moon_uniquePtr = make_unique<PhysicsObject>("PO_Moon", SPHERE, 1.0f, modelID, programID, 10.0f, -gravity, false);
    PhysicsObject *Moon = Moon_uniquePtr.get();
    unique_ptr<GameObject> MoonRotation_uniquePtr = make_unique<GameObject>("GO_MoonRotation", SPHERE, 1.0f, modelID, programID);
    GameObject *MoonRotation = MoonRotation_uniquePtr.get();
    unique_ptr<PhysicsObject> Sun_uniquePtr = make_unique<PhysicsObject>("PO_SunFall", SPHERE, 1.0f, modelID, programID, 5.0f, -gravity, false);
    PhysicsObject *Sun = Sun_uniquePtr.get();
    Sun->addVisualSphereRotation();

    unique_ptr<PhysicsObject> Cube_uniquePtr = make_unique<PhysicsObject>("PO_Cube", CUBE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *Cube = Cube_uniquePtr.get();
    unique_ptr<PhysicsObject> CubeFall_uniquePtr = make_unique<PhysicsObject>("PO_CubeFall", CUBE, 1.0f, modelID, programID, 5.0f, -gravity, false);
    PhysicsObject *CubeFall = CubeFall_uniquePtr.get();

    // --------------------
    // Textures
    // terrain->applyTexture(smiley_texture, TextureID);
    // Earth->ToDraw(false);
    // EarthRotation->applyTexture(earth_texture, TextureID);
    // Moon->ToDraw(false);
    // Earth->applyTexture(moon_texture, TextureID);
    terrain1->applyMaterial(&ice);
    terrain2->applyMaterial(&ice);
    Moon->applyTexture(moon_texture, TextureID);
    // MoonRotation->applyTexture(moon_texture, TextureID);
    Earth->applyMaterial(&fabric);
    Sun->applyTexture(sun_texture, TextureID);
    Cube->applyTexture(dice_texture, TextureID);
    CubeFall->applyTexture(dice_texture, TextureID);
    skybox->applyTexture(skybox_texture, TextureID);

    // --------------------
    // Définition des liens de parenté entre objets
    // terrain->addChild(move(Earth_uniquePtr));
    // Earth->addChild(move(EarthRotation_uniquePtr));
    Earth->addChild(move(defaultCamera_ptr));
    // Moon->addChild(move(MoonRotation_uniquePtr));

    // --------------------
    // Transformatios des objets dans l'espace local
    // light->transform->setLocalTranslation(vec3(0.0f, 0.0f, 0.0f));

    //skybox->transform->setLocalScale(vec3(5., 5., 5.));
    skybox->transform->setLocalRotation(vec3(180, 0, 0));

    cam->transform->setLocalTranslation(vec3(0, -20, 10));
    cam->transform->setLocalRotation(vec3(70, 0, 0));

    terrain1->transform->setLocalScale(vec3(5.0f, 5.0f, 5.0f));
    terrain2->transform->setLocalScale(vec3(5.0f, 5.0f, 5.0f));
    terrain2->transform->setLocalTranslation(vec3(5.0f, 0.0f, 0.0f));
    // terrain->transform->setLocalRotation(vec3(-90.0f, 0.0f, 0.0f));

    Earth->transform->setLocalScale(vec3(0.25f, 0.25f, 0.25f));
    Earth->transform->setLocalTranslation(vec3(2.5f, 0.0f, 0.0f));

    Moon->transform->setLocalScale(vec3(0.5f, 0.5f, 0.5f));
    Moon->transform->setLocalTranslation(vec3(2.5f, 2.5f, 0.0f));
    Moon->addVisualSphereRotation();
    // Moon->transform->setLocalTranslation(vec3(0.0f, 2.0f, 1.5f));

    Sun->transform->setLocalScale(vec3(0.25f, 0.25f, 0.25f));
    Sun->transform->setLocalTranslation(vec3(2.2f, 2.2f, 2.5f));
    // Sun->transform->setLocalTranslation(vec3(1.7f, 0.5f, 2.5f));

    Cube->transform->setLocalTranslation(vec3(1.0f, 0.5f, 0.5f));

    CubeFall->transform->setLocalTranslation(vec3(1.7f, 0.5f, 3.5f));

    // --------------------
    // Add Objects to Scene
    scene->addPlayer(Earth);
    scene->addLight(light);
    scene->addCamera(cam);
    scene->addPhysicsObject(terrain1);
    scene->addPhysicsObject(terrain2);
    scene->addPhysicsObject(Earth);
    // scene->addObject(EarthRotation);
    scene->addPhysicsObject(Moon);
    // scene->addObject(MoonRotation);
    scene->addPhysicsObject(Sun);
    scene->addObject(skybox);
    // scene->addPhysicsObject(Cube);
    // scene->addPhysicsObject(CubeFall);

    PositionSolver *positionSolver = new PositionSolver();
    ImpulseSolver *impulseSolver = new ImpulseSolver();
    scene->addSolver(positionSolver);
    scene->addSolver(impulseSolver);

    InputManager *inputManager = new InputManager(window, cam, scene);
    glfwSetKeyCallback(window, key_callback);

    do
    {
        // cout << Earth->transform->getWorldTranslation() << endl;
        // cout << cam->transform->getWorldTranslation() << endl;

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (wireframe_mode)
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
        GLint cameraPosID = glGetUniformLocation(programID, "camPos");
        glUniformMatrix3fv(cameraPosID, 1, GL_FALSE, &cam->transform->getLocalTranslation().x);

        // vec3 actualEarthPosition = Earth->transform->getLocalTranslation();
        // Earth->transform->setLocalTranslation(vec3(actualEarthPosition.x, actualEarthPosition.y, Earth->heightInTriangle(terrain->mesh) + 1.0 * Earth->transform->getLocalScale().x));

        vec3 actualCamRotation = cam->transform->getLocalRotation();
        cam->transform->setLocalRotation(vec3(actualCamRotation.x, actualCamRotation.y, actualCamRotation.z));

        // --------------------
        // Update Objects
        // skybox->updateSelfAndChild();
        // terrain1->updateSelfAndChild();
        // terrain2->updateSelfAndChild();
        // Earth->updateSelfAndChild();
        // Moon->updateSelfAndChild();
        // Sun->updateSelfAndChild();
        // Cube->updateSelfAndChild();
        // CubeFall->updateSelfAndChild();
        // cout << Moon->transform->getWorldTranslation() << endl;
        scene->update();

        scene->Step(deltaTime);
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

        // --------------------
        // Draw Scene

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
