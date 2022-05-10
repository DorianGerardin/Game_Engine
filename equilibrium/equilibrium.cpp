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
// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;
#include "src/InputManager.cpp"

// void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    crystal.normal = "textures/crystal/normal.bmp";
    crystal.metallic = "textures/crystal/metallic.bmp";
    crystal.roughness = "textures/crystal/roughness.bmp";
    crystal.ao = "textures/crystal/ao.bmp";

    Material abstract;
    abstract.albedo = "textures/abstract1/albedo.bmp";
    abstract.normal = "textures/abstract1/normal.bmp";
    abstract.metallic = "textures/abstract1/metallic.bmp";
    abstract.roughness = "textures/abstract1/roughness.bmp";
    abstract.ao = "textures/abstract1/ao.bmp";

    Material fabric;
    fabric.albedo = "textures/fabric1/albedo.bmp";
    fabric.normal = "textures/fabric1/normal.bmp";
    fabric.metallic = "textures/fabric1/metallic.bmp";
    fabric.roughness = "textures/fabric1/roughness.bmp";
    fabric.ao = "textures/fabric1/ao.bmp";

    Material ice;
    ice.albedo = "textures/ice/albedo.bmp";
    ice.normal = "textures/ice/normal.bmp";
    ice.metallic = "textures/ice/metallic.bmp";
    ice.roughness = "textures/ice/roughness.bmp";
    ice.ao = "textures/ice/ao.bmp";

    Material concrete;
    concrete.albedo = "textures/concrete/albedo.bmp";
    concrete.normal = "textures/concrete/normal.bmp";
    concrete.metallic = "textures/concrete/metallic.bmp";
    concrete.roughness = "textures/concrete/roughness.bmp";
    concrete.ao = "textures/concrete/ao.bmp";

    Material lava;
    lava.albedo = "textures/lava/albedo.bmp";
    lava.normal = "textures/lava/normal.bmp";
    lava.metallic = "textures/lava/metallic.bmp";
    lava.roughness = "textures/lava/roughness.bmp";
    lava.ao = "textures/lava/ao.bmp";

    Material rusty;
    rusty.albedo = "textures/rustyMetal/albedo.bmp";
    rusty.normal = "textures/rustyMetal/normal.bmp";
    rusty.metallic = "textures/rustyMetal/metallic.bmp";
    rusty.roughness = "textures/rustyMetal/roughness.bmp";
    rusty.ao = "textures/rustyMetal/ao.bmp";

    Material brick;
    brick.albedo = "textures/brick/albedo.bmp";
    brick.normal = "textures/brick/normal.bmp";
    brick.metallic = "textures/brick/metallic.bmp";
    brick.roughness = "textures/brick/roughness.bmp";
    brick.ao = "textures/brick/ao.bmp";

    Material gold_nugget;
    gold_nugget.albedo = "textures/gold_nugget/albedo.bmp";
    gold_nugget.normal = "textures/gold_nugget/normal.bmp";
    gold_nugget.metallic = "textures/gold_nugget/metallic.bmp";
    gold_nugget.roughness = "textures/gold_nugget/roughness.bmp";
    gold_nugget.ao = "textures/gold_nugget/ao.bmp";

    Material asphalt;
    asphalt.albedo = "textures/asphalt/albedo.bmp";
    asphalt.normal = "textures/asphalt/normal.bmp";
    asphalt.metallic = "textures/asphalt/metallic.bmp";
    asphalt.roughness = "textures/asphalt/roughness.bmp";
    asphalt.ao = "textures/asphalt/ao.bmp";

    GLuint earth_texture = loadBMP_custom("textures/earthTexture.bmp");
    GLuint moon_texture = loadBMP_custom("textures/moonTexture.bmp");
    GLuint sun_texture = loadBMP_custom("textures/sunTexture.bmp");
    GLuint rock_texture = loadBMP_custom("textures/rock.bmp");
    GLuint parquet_texture = loadBMP_custom("textures/parquet.bmp");
    GLuint smiley_texture = loadBMP_custom("textures/smiley.bmp");
    GLuint dice_texture = loadBMP_custom("textures/dice.bmp");
    GLuint skybox_texture = loadBMP_custom("textures/metallic.bmp");

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
    float gravity = earthGravity;
    // float gravity = 0.5f;

    // --------------------------------------------------------------------------------------------
    // |                                         SCENE                                            |
    // --------------------------------------------------------------------------------------------

    Scene *scene = new Scene();
    /*InputManager *inputManager = new InputManager(window, scene);*/
    // CAMERA camera = inputManager->scene->cameras[0];

    // --------------------
    // Déclaration des Objets
    unique_ptr<LightObject> light_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain1_1 = light_uniquePtr.get();
    light_terrain1_1->position = vec3(1.,1. ,.9);

    unique_ptr<LightObject> light2_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain1_2 = light2_uniquePtr.get();
    light_terrain1_2->position = vec3(5., 5.,0.7 );

    unique_ptr<LightObject> light3_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain1_3 = light3_uniquePtr.get();
    light_terrain1_3->position = vec3(1., 9.,0.9 );

    unique_ptr<LightObject> light4_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain1_4 = light4_uniquePtr.get();
    light_terrain1_4->position = vec3(9., 1.,0.9 );

    unique_ptr<LightObject> light5_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain1_5 = light5_uniquePtr.get();
    light_terrain1_5->position = vec3(9., 9.,0.9 );

    unique_ptr<LightObject> light6_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain1_6 = light6_uniquePtr.get();
    light_terrain1_6->position = vec3(5.0f, 0.f, 0.9 );
    light_terrain1_6->ambient = vec3(1., 0.2, 0.7);

    //terrain 2
    unique_ptr<LightObject> light1_terrain2_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain2_1 = light1_terrain2_uniquePtr.get();
    light_terrain2_1->position = vec3(15.0f, 5.0f, -0.8f );
    light_terrain2_1->ambient = vec3(.61, 0.83, 0.87);

    unique_ptr<LightObject> light2_terrain2_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain2_2 = light2_terrain2_uniquePtr.get();
    light_terrain2_2->position = vec3(11.0f, 1.0f, -0.8f );
    light_terrain2_2->ambient = vec3(.61, 0.83, 0.87);

    unique_ptr<LightObject> light3_terrain2_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain2_3 = light3_terrain2_uniquePtr.get();
    light_terrain2_3->position = vec3(19.0f, 1.0f, -0.8f );
    light_terrain2_3->ambient = vec3(.61, 0.83, 0.87);

    unique_ptr<LightObject> light4_terrain2_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain2_4 = light4_terrain2_uniquePtr.get();
    light_terrain2_4->position = vec3(11.0f, 9, -0.8f );
    light_terrain2_4->ambient = vec3(.61, 0.83, 0.87); //.61, 0.83, 0.87s

    unique_ptr<LightObject> light5_terrain2_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_terrain2_5 = light5_terrain2_uniquePtr.get();
    light_terrain2_5->position = vec3(19.0f, 9.f, -0.8f );
    light_terrain2_5->ambient = vec3(.61, 0.83, 0.87);

    //lava
    unique_ptr<LightObject> light1_lava_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_lava_1 = light1_lava_uniquePtr.get();
    light_lava_1->position = vec3(28.4f, 5.0f, -1.8f);
    light_lava_1->ambient = vec3(1.0, 0, 0);

    unique_ptr<LightObject> light2_lava_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_lava_2 = light2_lava_uniquePtr.get();
    light_lava_2->position = vec3(28.4f, 7.0f, -1.9f);
    light_lava_2->ambient = vec3(1.0, 0, 0);

    unique_ptr<LightObject> light3_lava_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_lava_3 = light3_lava_uniquePtr.get();
    light_lava_3->position = vec3(28.3f, 3.0f, -1.8f);
    light_lava_3->ambient = vec3(1.0, 0, 0);

    unique_ptr<LightObject> light4_lava_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_lava_4 = light4_lava_uniquePtr.get();
    light_lava_4->position = vec3(24.5, 3., -1.8);
    light_lava_4->ambient = vec3(1, 0, 0);

    //checkpoint
    unique_ptr<LightObject> light1_checkpoint_uniquePtr = make_unique<LightObject>(programID);
    LightObject *light_checkpoint_1 = light1_checkpoint_uniquePtr.get();
    light_checkpoint_1->position = vec3(31.2f, 7.2f, 2.5f);
    light_checkpoint_1->ambient = vec3(.95, 0.97, 0.05);

    // unique_ptr<LightObject> light_uniquePtr = make_unique<LightObject>(programID);
    // LightObject *light_begin_1 = light_uniquePtr.get();
    // unique_ptr<LightObject> light_uniquePtr = make_unique<LightObject>(programID);
    // LightObject *light_begin_1 = light_uniquePtr.get();
    // unique_ptr<LightObject> light2_uniquePtr = make_unique<LightObject>(programID);
    // LightObject *light2 = light2_uniquePtr.get();
    // light2->position = vec3(0.0f);
    // light2->ambient = vec3(1.);
    // 5.0f, 5.0f, 5.0f


    unique_ptr<GameObject> skybox_uniquePtr = make_unique<GameObject>("GO_SkyBox", SPHERE, 200.0f, modelID, programID);
    GameObject *skybox = skybox_uniquePtr.get();

    // CameraObject *cam = scene->cameras2[0];
    // vec3 position = vec3(1.75f, 1.f, 4.5f);
    vec3 target = vec3(0.0f, 0.0f, -1.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    unique_ptr<CameraObject> defaultCamera_ptr = make_unique<CameraObject>("CO_Camera", target, up);
    CameraObject *cam = defaultCamera_ptr.get();
    cam->ToDraw(false);

    unique_ptr<PhysicsObject> Earth_uniquePtr = make_unique<PhysicsObject>("PO_Earth", SPHERE, 1.0f, modelID, programID, 50.0f, -gravity, false);
    PhysicsObject *Earth = Earth_uniquePtr.get();
    // Earth->setPlayer(true);
    Earth->addVisualSphereRotation();

    unique_ptr<PhysicsObject> plane1_uniquePtr = make_unique<PhysicsObject>("PO_Plan1", PLANE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *terrain1 = plane1_uniquePtr.get();
    // unique_ptr<PhysicsObject> plane1_uniquePtr = make_unique<PhysicsObject>("PO_Plan1", CUBE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    // PhysicsObject *terrain1 = plane1_uniquePtr.get();
    terrain1->setPhysicsCoeffs(0.7f, 0.6f, 1.0f);

    unique_ptr<PhysicsObject> plane2_uniquePtr = make_unique<PhysicsObject>("PO_Plan2", PLANE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *terrain2 = plane2_uniquePtr.get();
    terrain2->setPhysicsCoeffs(0.7f, 0.25f, 1.0f);

    unique_ptr<PhysicsObject> plateforme0_uniquePtr = make_unique<PhysicsObject>("PO_Plateforme0", CUBE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *plateforme0 = plateforme0_uniquePtr.get();
    unique_ptr<PhysicsObject> plateforme1_uniquePtr = make_unique<PhysicsObject>("PO_Plateforme1", CUBE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *plateforme1 = plateforme1_uniquePtr.get();
    plateforme1->setPhysicsCoeffs(0.7f, 0.6f, 1.0f);

    unique_ptr<PhysicsObject> plateforme2_uniquePtr = make_unique<PhysicsObject>("PO_Plateforme2", CUBE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *plateforme2 = plateforme2_uniquePtr.get();
    plateforme2->setPhysicsCoeffs(0.7f, 0.6f, 1.0f);

    unique_ptr<PhysicsObject> lavaPlane_uniquePtr = make_unique<PhysicsObject>("PO_LAVA_PLANE", PLANE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *lavaPlane = lavaPlane_uniquePtr.get();
    lavaPlane->setTrigger(true);
    lavaPlane->setTriggerType(MOVE_TO_LAST_CHECKPOINT);

    unique_ptr<PhysicsObject> ballBrick_uniquePtr = make_unique<PhysicsObject>("PO_ballBrick", SPHERE, 1.0f, modelID, programID, 75.0f, -gravity, false);
    PhysicsObject *ballBrick = ballBrick_uniquePtr.get();
    unique_ptr<PhysicsObject> ballFabric_uniquePtr = make_unique<PhysicsObject>("PO_ballFabric", SPHERE, 1.0f, modelID, programID, 1.0f, -gravity, false);
    PhysicsObject *ballFabric = ballFabric_uniquePtr.get();

    unique_ptr<PhysicsObject> CubeFall_uniquePtr = make_unique<PhysicsObject>("PO_CubeFall", CUBE, 1.0f, modelID, programID, 20.0f, -gravity, false);
    PhysicsObject *CubeFall = CubeFall_uniquePtr.get();

    unique_ptr<PhysicsObject> planeFall_uniquePtr = make_unique<PhysicsObject>("PO_PlaneFall", INFINITE_PLANE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *planeFall = planeFall_uniquePtr.get();
    planeFall->ToDraw(false);
    planeFall->setTrigger(true);
    planeFall->setTriggerType(MOVE_TO_LAST_CHECKPOINT);

    unique_ptr<PhysicsObject> checkpoint1_uniquePtr = make_unique<PhysicsObject>("PO_CHECKPOINT", INFINITE_PLANE, 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *checkpoint1 = checkpoint1_uniquePtr.get();
    checkpoint1->ToDraw(false);
    checkpoint1->setTrigger(true);
    checkpoint1->setTriggerType(UPDATE_LAST_CHECKPOINT);

    unique_ptr<PhysicsObject> endLevel_uniquePtr = make_unique<PhysicsObject>("PO_endLevel", "objects/ring.obj", 1.0f, modelID, programID, 5.0f, -gravity, true);
    PhysicsObject *endLevel = endLevel_uniquePtr.get();
    endLevel->ToDraw(true);
    endLevel->setTrigger(true);
    endLevel->setTriggerType(LEVEL_COMPLETE);

    // --------------------
    // Textures
    // terrain->applyTexture(smiley_texture, TextureID);
    // Earth->ToDraw(false);
    // EarthRotation->applyTexture(earth_texture, TextureID);
    // Moon->ToDraw(false);
    // Earth->applyTexture(moon_texture, TextureID);
    terrain1->applyMaterial(&concrete);
    terrain2->applyMaterial(&ice);

    plateforme0->applyMaterial(&concrete);
    plateforme1->applyMaterial(&asphalt);
    plateforme2->applyMaterial(&asphalt);
    lavaPlane->applyMaterial(&lava);

    // terrain2->applyMaterial(ICE);
    checkpoint1->applyTexture(smiley_texture, TextureID);
    // ballFabric->applyTexture(moon_texture, TextureID);
    // ballFabricRotation->applyTexture(ballFabric_texture, TextureID);
    ballBrick->applyMaterial(&brick);
    ballFabric->applyMaterial(&fabric);
    Earth->applyMaterial(&crystal);
    CubeFall->applyMaterial(&rusty);
    endLevel->applyMaterial(&gold_nugget);
    // CubeFall->applyTexture(dice_texture, TextureID);
    skybox->applyTexture(skybox_texture, TextureID);

    // --------------------
    // Définition des liens de parenté entre objets
    // terrain->addChild(move(Earth_uniquePtr));
    // Earth->addChild(move(EarthRotation_uniquePtr));
    Earth->addChild(move(defaultCamera_ptr));
    // Earth->addChild(move(light6_uniquePtr));
    // Moon->addChild(move(MoonRotation_uniquePtr));

    // --------------------
    // Transformatios des objets dans l'espace local
    // light->transform->setLocalTranslation(vec3(0.0f, 0.0f, 0.0f));

    // skybox->transform->setLocalScale(vec3(5., 5., 5.));
    skybox->transform->setLocalRotation(vec3(180, 0, 0));

    cam->transform->setLocalTranslation(vec3(0, -20, 10));
    cam->transform->setLocalRotation(vec3(70, 0, 0));

    terrain1->transform->setLocalScale(vec3(10.0f, 10.0f, 10.0f));
    // terrain1->transform->setLocalScale(vec3(10.0f, 10.0f, 2.0f));
    // terrain1->transform->setLocalTranslation(vec3(5.0f, 5.0f, -1.0f));
    plateforme0->transform->setLocalScale(vec3(1.0f, 10.0f, 2.0f));
    plateforme0->transform->setLocalTranslation(vec3(9.5f, 5.0f, -1.0f));

    terrain2->transform->setLocalScale(vec3(10.0f, 10.0f, 10.0f));
    terrain2->transform->setLocalTranslation(vec3(10.0f, 0.0f, -2.0f));

    checkpoint1->transform->setLocalScale(vec3(5.0f, 2.0f, 1.0f));
    checkpoint1->transform->setLocalRotation(vec3(0.0f, 90.0f, 0.0f));
    checkpoint1->transform->setLocalTranslation(vec3(20.0f, 5.0f, 2.0f));

    plateforme1->transform->setLocalScale(vec3(2.5f, 10.0f, 3.0f));
    plateforme1->transform->setLocalTranslation(vec3(21.0f, 5.0f, -0.5f));
    lavaPlane->transform->setLocalScale(vec3(10.0f, 10.0f, 10.0f));
    lavaPlane->transform->setLocalTranslation(vec3(22.0f, 0.0f, -2.0f));
    plateforme2->transform->setLocalScale(vec3(5.0f, 10.0f, 3.0f));
    plateforme2->transform->setLocalTranslation(vec3(31.0f, 5.0f, -0.5f));

    planeFall->transform->setLocalScale(vec3(100.0f, 100.0f, 100.0f));
    planeFall->transform->setLocalTranslation(vec3(0.0f, 0.0f, -5.0f));

    Earth->transform->setLocalScale(vec3(0.25f, 0.25f, 0.25f));
    Earth->transform->setLocalTranslation( vec3(5.0f, 0.5f, 10.0f)); //25.9f, 5.0f, -1.8f vec3(5.0f, 0.5f, 10.0f)

    ballBrick->transform->setLocalTranslation(vec3(6.3f, 5.0f, 23.0f));
    // ballBrick->addVisualSphereRotation();

    ballFabric->transform->setLocalScale(vec3(0.5f, 0.5f, 0.5f));
    ballFabric->transform->setLocalTranslation(vec3(5.9f, 4.95, 25.0f));
    // ballFabric->addVisualSphereRotation();

    CubeFall->transform->setLocalTranslation(vec3(2.9f, 5.0f, 22.0f));


    endLevel->transform->setLocalRotation(vec3(0.0f, 90.0f, 0.0f));
    endLevel->transform->setLocalTranslation(vec3(31.0f, 7.5f, 2.5f));
    // endLevel->transform->setLocalTranslation(vec3(8.0f, 2.0f, 2.0f));

    // --------------------
    // Add Objects to Scene
    scene->addPlayer(Earth);
    scene->addLight(light_terrain1_1);
    scene->addLight(light_terrain1_2);
    scene->addLight(light_terrain1_3);
    scene->addLight(light_terrain1_4);
    scene->addLight(light_terrain1_5);
    scene->addLight(light_terrain1_6);
    scene->addLight(light_terrain2_1);
    scene->addLight(light_terrain2_2);
    scene->addLight(light_terrain2_3);
    scene->addLight(light_terrain2_4);
    scene->addLight(light_terrain2_5);
    scene->addLight(light_lava_1);
    scene->addLight(light_lava_2);
    scene->addLight(light_lava_3);
    scene->addLight(light_lava_4);

    scene->addLight(light_checkpoint_1);
    
    scene->addCamera(cam);
    scene->addObject(skybox);
    scene->addPhysicsObject(Earth);

    scene->addPhysicsObject(planeFall);
    scene->addPhysicsObject(ballFabric);
    scene->addPhysicsObject(ballBrick);
    scene->addPhysicsObject(terrain1);
    scene->addPhysicsObject(plateforme0);
    scene->addPhysicsObject(terrain2);
    scene->addPhysicsObject(lavaPlane);
    scene->addPhysicsObject(checkpoint1);
    scene->addPhysicsObject(plateforme1);
    scene->addPhysicsObject(CubeFall);
    scene->addPhysicsObject(plateforme2);
    scene->addPhysicsObject(endLevel);

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
