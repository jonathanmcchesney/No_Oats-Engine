#define GLEW_STATIC
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "SOIL2/SOIL2.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "CollisionDetection.h"
#include "AssetManager.h"
#include "ManualCube.h"
#include "ManualQuad.h"
#include "Windows.h"
#include "Skybox.h"
#include "Lighting.h"
#include "Shadows.h"
#include "Text.h"
#include "Instancing.h"
#include "Scenes.h"
#include "NormalMapping.h"
#include "ParallaxMapping.h"
#include "Depth.h"
#include "Billboard.h"
#include "Constants.h"
#include "ParticleFountain.h"
#include "Firework.h"
#include "Cube.h"
#include "Container.h"
#include "Assets.h"
#include "Buffers.h"
#include "TextureAtlas.h"
#include "ParticleTrail.h"

// Code Allocation:
// Jonathan McChesney: Setup, Scenes, Assets, creation & callback functionality.
// Erin Hughes: Particles, Texture Atlases, AssetManager (LOC ~= 52)
// Simon Campbell: Collision Cubes, Scripts
// Shane Fearon: Billboard

// Sources used throughout the project: These were used to teach and expand our understand of the theory surrounding various topics and user stories, from there we developed
// and expanded upon our own solutions to complete our user stories to the best of our abilities.
// •	CSC3049 & ELE4014 - Lectures
// •	https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process
// •	https ://www.opengl.org/
// •	www.opengl - tutorial.org / &www.freetype.org /
// •	Practical Algorithms for 3D Computer Graphics.
// •	LearnOpenGL.com & Offline Transcript.
// •	http ://www.codinglabs.net/article_physically_based_rendering.aspx
// •	http ://mrl.nyu.edu/~perlin/demox/Planet.html
// •	http ://http.developer.nvidia.com/GPUGems2/gpugems2_chapter26.html
// •	Recommended reading material from CSC3049 - GPU_GEMS & Real Time Collision Detection
// •	Research Papers by Disney & Epic Games - especially helpfully in learning and further developing algorithms for light & mapping.


// JM: Instantiation of our objects/classes
vector<const GLchar*> faces;
vector<GameObject *> oatsCubes;
Depth depthScene;
ParallaxMapping parallaxScene;
NormalMapping normalScene;
Windows windowsScene;
Skybox skybox;
Lighting lighting;
Shadows shadows;
Text text;
Instancing instance;
Scenes scene;
Container containerScene;
Assets assets;
Buffers glBuffer;

// JM: Initialisation of multiple camera object, add as many cameras as desired.
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f)), staticCamera1(glm::vec3(2.4f, 2.0f, 1.0f)), staticCamera2(glm::vec3(-2.4f, 2.0f, 1.0f));
Camera cameras[3] = { camera, staticCamera1, staticCamera2 };
glm::mat4 projection;

// JM: Method definitions for key and mouse input.
// Definitions for Spawning collision cubes, updating the frame and setup are also defined here. 
// EH: Definition of function for changing the texture atlas is also here.
void KeyTouch(GLFWwindow *window, int key, int scancode, int action, int mode),
MouseMovement(GLFWwindow *window, double xPos, double yPos),
MouseButtonCallbackFunction(GLFWwindow* window, int button, int pr, int mods),
Move(), Update(), UpdateAngle(float secondsElapsed), SetupShaders(),
SpawnGameObjectCubes(const glm::mat4 &projection);
void ChangeAtlasTexture(TextureAtlas &atlas);

// Variables for custom script functionality.
bool editMode = false;
int editModeIndex = 0, gameObjectIndex = 0;
glm::vec3 customScriptVector(0.0f, 0.0f, 0.0f);

// EH: Variables for particle functionality.
glm::vec3 mousePos(0.0f, 0.0f, 0.0f);
GLboolean drawTrail = false;
glm::mat4 trailView;

int main()
{
#pragma region Initial_Setup
	// JM: 
	glfwInit(); // initialises glfw

	if (!glfwInit()) // if not initialised correctly
	{
		fprintf(stderr, "Failed to initialize GLFW\n"); // print the relevant message
		return EXIT_FAILURE; // return an exit failure
	}

	// Set window hints as appropriate
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create the GLFW window, frame buffer size and input callbacks
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "NO_OATS: ENGINE", nullptr, nullptr);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwSetKeyCallback(window, KeyTouch);
	glfwSetCursorPosCallback(window, MouseMovement);
	glfwSetMouseButtonCallback(window, MouseButtonCallbackFunction);

	// Set the current context to that of the window & glewExperimental (allow extension entry points to be loaded) is activated.
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	// If GLEW is not initialised correctly then return accordingly.
	if (GLEW_OK != glewInit())
	{
		Logging::logError("Failure in initializing GLEW", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// If the window was not created correctly then handle appropriately.
	if (nullptr == window)
	{
		Logging::logError("Failure in creating GLFW window", __FILE__, __LINE__);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Set the input mode - disabling the cursor when the application is active & set up the viewport parameters.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Activate multi sampling for anti-aliasing, depth testing for use of depth buffers, set the depth function as needed (set to GL_LESS here),
	// enable blending functionality (using GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA for transparency).
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable stencil testing and stencil functionality (setting the relevant params and masks) as well as the relevant stencil operation.
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Enable seamless cube maps and face culling during our rendering process.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_CULL_FACE);

#pragma endregion Initial_Setup

#pragma region Model_Region

	// JM: Load in the relevant models from resources
	Model	sphereModel("res/models/sphere1.obj"),
		simpleSphereModel("res/models/simpleSphere.obj"),
		rock("res/models/rock.obj"),
		door("res/models/door/doorObj.obj"),
		planet("res/models/star/Death_Star.obj"),
		bulb("res/models/bulb.obj"),
		dino("res/models/Raptor/Raptor.obj"),
		stand("res/models/stand/stand.obj"),
		stand2("res/models/stand/stand2.obj"),
		statue1("res/models/statue/chine.obj"),
		statue2("res/models/lion/Lion_Head.obj"),
		plate("res/models/plate/Dishes_BlackSet_Model.obj"),
		table1("res/models/Bar table.obj"),
		chair1("res/models/chair/OBJ/georgetti_51521.obj"),
		muffin("res/models/Muffin.obj"),
		barrier("res/models/barrier.obj"),
		chain("res/models/chain.obj"),
		fountain("res/models/fountainOBJ/fountain.obj"),
		cig("res/models/cigarette.obj");

	// SF: Create the billboard
	Billboard billy(glm::vec3(-5.5, 1.0, 4.0), glm::vec2(1.0, 1.0), "res/images/image69420.jpg"); // SF

#pragma endregion Model_Region

#pragma region Setup_Region
	// JM: 
	// Generate our textures for the diffuse, specular and emission maps.
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);

	// Set up our projection matrix by setting its parameters accordingly (taking the current camera view, the
	projection = glm::perspective(cameras[currentCamera].GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f); cameras[currentCamera].position.y = 0.5f;
	cubemapTexture = skybox.RandomSkybox();
	containerScene.SetContainerImage(true, imageInt);

	// Initialise our scene models and do a first time set up of the window scene
	windowsScene.SortWindows(cameras[currentCamera]);

	// If we had more time these would have been more configurable allowing the user to easy switch
	// between models to use within the scenes and possible alter attributes such as position etc.
	scene.reflectiveModel = sphereModel, scene.simpleSphereModel = simpleSphereModel;
	containerScene.lights.lightBulb = bulb;
	scene.shadowModel = dino; scene.cigModel = cig;
	scene.fountainModel = fountain; 	scene.table1Model = table1;
	scene.standModel = stand; scene.stand2Model = stand2;
	scene.chair1Model = chair1, scene.doorModel = door;
	scene.muffinModel = muffin; scene.plateModel = plate;
	scene.barrierModel = barrier; scene.chainModel = chain;
	scene.statue1Model = statue1; scene.statue2Model = statue2;

	// JM: Load all our assets
	assets.SetupAssets();

	// JM: Set the initial attributes/maps for our various shaders.
	SetupShaders();

	// JM: Setup the windows.
	windowsScene.SetupWindows();

	// JM: Setup the shadow buffers (the depth buffers for directional and omnidirectional shadows).
	glBuffer.SetupShadowBuffers();

	// JM: Setup the regular framebuffers.
	glBuffer.SetupFrameBuffers();

	// JM: Setup the instancing models & intialise instance scene (i.e. set up relevant offsets).
	instance.modelMatrices = instance.InitInstancingModels();
	instance.InitInstancingScene(rock, buffer);

	// JM: Setup freetype within our engine.
	text.SetupFreeType(AssetManager::getShader("textShader"), &projection, WIDTH, HEIGHT);

	// EH: creation of particle objects (done here after the assets have been loaded in)
	ParticleFountain pFountain(glm::vec3(-80.f, 0.0f, 80.0f), AssetManager::getShader("particle"), AssetManager::getTexture("drop"));
	Firework fireworks[3] =
	{
		Firework(glm::vec3(0.f, 40.0f, -5.0f), AssetManager::getShader("particle"), AssetManager::getTexture("star")),
		Firework(glm::vec3(10.0f, 40.0f, -4.9f), AssetManager::getShader("particle"), AssetManager::getTexture("star")),
		Firework(glm::vec3(-10.0f, 40.0f, -4.8f), AssetManager::getShader("particle"), AssetManager::getTexture("star"))
	};
	TextureAtlas atlas(AssetManager::getTexture("atlas3"), 7, glm::vec3(4.5f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	TextureAtlas cigAtlas(AssetManager::getTexture("atlas5"), 4, glm::vec3(2.4f, 1.155f, 4.45f), glm::vec3(0.15f, 0.4f, 0.4f));
	ParticleTrail trail(mousePos, AssetManager::getShader("particle"), AssetManager::getTexture("star"));
	//setting the view matrix for drawing the particle trail
	trailView = cameras[currentCamera].GetViewMatrix();

#pragma endregion Setup_Region

	// JM: Set our projection matrix (perspective: taking the cameras view, viewpoint float params, near and far plane as parameters). FOV - Aspect - Near - Far
	projection = glm::perspective(cameras[currentCamera].GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	// JM: Game loop - Main body
	while (!glfwWindowShouldClose(window))
	{
		// JM: Invoke the Update Frame
		Update();

#pragma region Draw

		// JM: The point shadows object will render the depth (map & cubemap), taking the shadow/point shadow depth shaders, a boolean which decides if the metal room is visible,
		// the current camera and the projection matrix.
		shadows.RenderDepthMap(AssetManager::getShader("shadowDepthShader"), AssetManager::getShader("pointShadowDepthShader"), activeRoom, cameras[currentCamera], projection);

		// JM: Set the view matrix to the view matrix of the current camera.
		view = cameras[currentCamera].GetViewMatrix();

		// JM: Bind the framebuffer (allows for off screen rendering when we switch back to the default framebuffer to see the visual impact).
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// Reset the view port and clear our buffers in preperation for our next render pass.
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// EH: callback function changing the texture applied to a TextureAtlas object
		ChangeAtlasTexture(atlas);

		// JM: Sort through the transparent/translucent windows.
		windowsScene.SortWindows(cameras[currentCamera]); 

		// JM: Render our skybox scene
		skybox.RenderSkyboxScene(AssetManager::getShader("skyboxShader"),cameras[currentCamera], projection);

		// JM: Render our normal mapping scene.
		normalScene.RenderNormalMappingScene(AssetManager::getShader("tangentNormalShader"), cameras[currentCamera], projection);
	
		// JM: Render our parralax occlusion mapping scene.
		parallaxScene.RenderParallaxMappingScene(AssetManager::getShader("parallaxShader"), cameras[currentCamera], projection);

		// JM: Render our directional shadow scene.
		shadows.RenderDirectionalShadowScene(AssetManager::getShader("shadowShader"), AssetManager::getShader("debugQuad"), cameras[currentCamera], shadowsEnabled, quadDebugEnabled, projection);

		// JM: Render our omni-directional shadow scene.
		shadows.RenderPointShadowScene(AssetManager::getShader("pShadowShader"), cameras[currentCamera], activeRoom, projection);

		// JM: Render our floor with a Blinn-Phong lighting model scene.
		lighting.RenderBlinnScene(AssetManager::getShader("blinnShader"), AssetManager::getShader("shadowShader"), cameras[currentCamera], projection);
		
		// SC: Performs the Update loop on all GameObjects and their scripts
		for (int i = 0; i < oatsCubes.size(); i++)
		{
			oatsCubes[i]->Update();
		}

		// JM: Render our depth testing scene.
		depthScene.RenderDepthScene(AssetManager::getShader("depthShader"), cameras[currentCamera], projection);

		// JM: Render our environment mapping scene.
		scene.DrawReflectionModelScene(AssetManager::getShader("modelShader"), cameras[currentCamera], projection);

		billy.Draw(projection, cameras[currentCamera]); //SF

		// JM: Render our container scene with a Phong lighting model applied - Diffuse, Specular & Emission map applied.
		containerScene.RenderMovingContainerScene(AssetManager::getShader("lightingShader"), cameras[currentCamera], projection);

		// SC : Allows for Spawning of GameObjects
		SpawnGameObjectCubes(projection);
		
		// JM: Render our light objects scene.
		containerScene.lights.RenderLightScene(AssetManager::getShader("lightingShader"), AssetManager::getShader("modelShader"), projection);

		// JM: Render our various exhibit scenes.
		scene.RenderScene01(AssetManager::getShader("lightingShader"), cameras[currentCamera], activeRoom);
		scene.DrawStandScene(AssetManager::getShader("depthShader"), cameras[currentCamera]);
		scene.RenderDoorScene(AssetManager::getShader("lightingShader"), AssetManager::getShader("stencilShader"), cameras[currentCamera], isStencil, projection);

		// EH: Drawing the ParticleFountain
		pFountain.draw(projection, view); 

		// JM: Render our instanced model scene.
		instance.DrawInstancingScene(AssetManager::getShader("lightingShader"), AssetManager::getShader("instanceShader"), planet, rock, cameras[currentCamera]);

		// EH: the order in which the atlases and windows are drawn in changes depending on the direction the camera is facing (this is to allow for proper alpha blending)
		if (cameras[currentCamera].GetFront().z > 0)
		{
			windowsScene.Draw(AssetManager::getShader("windowShader"), cameras[currentCamera], projection); // JM
			cigAtlas.draw(AssetManager::getShader("textureAtlas"), projection, view);
			atlas.draw(AssetManager::getShader("textureAtlas"), projection, view);
		}
		else
		{
			atlas.draw(AssetManager::getShader("textureAtlas"), projection, view);
			cigAtlas.draw(AssetManager::getShader("textureAtlas"), projection, view);
			windowsScene.Draw(AssetManager::getShader("windowShader"), cameras[currentCamera], projection); //JM
		}

		// EH: draw every firework in the fireworks array
		for (int i = 0; i < 3; i++)
		{
			fireworks[i].draw(projection, view);
		}
		
		// EH: set the start position of the particle trail to the current mouse position
		trail.setStartPos(mousePos);
		// EH: if the drawing the trail has been enabled then draw the particle trail
		if (drawTrail) { trail.draw(projection, trailView); }

		// JM: Render our post processing framebuffer scene.
		glBuffer.RenderFrameBufferScene(AssetManager::getShader("fbShader"), AssetManager::getShader("fbCameraShader"), AssetManager::getShader("fbCameraShader2"), screenTexture, currentCamera);

		if (editMode)
		{
			text.RenderEditTextScene(AssetManager::getShader("textShader"), oatsCubes[gameObjectIndex], editModeIndex, gameObjectIndex, customScriptVector);
		}
		else
		{
			text.RenderTextScene(AssetManager::getShader("textShader"), currentCamera, fps);
		}

		for (int i = 0; i < oatsCubes.size(); i++)
		{
			for (int j = 0; j < oatsCubes.size(); j++)
			{
				if (i != j) 
				{
					AABBCollisionDetection::TestAABBToAABB(oatsCubes[i], oatsCubes[j]);
				}
			}
		}

#pragma endregion

		glfwSwapBuffers(window);
	}
	// JM: Delete our buffers and model matrices when the program ends.
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteFramebuffers(1, &depthMap);
	glDeleteFramebuffers(1, &pointDepthMapFBO);
	glDeleteFramebuffers(1, &depthCubemap);
	glDeleteFramebuffers(1, &buffer);
	glDeleteFramebuffers(1, &renderBufferObject);

	delete[] instance.modelMatrices;

	glfwTerminate();
	return 0;
}

// JM: This method gradually updates an angle, taking a float as a parameter, this is used to update
// the degrees rotated variable. If this variable exceeds 360.0f it is then rest by subtracting 360.0f.
void UpdateAngle(float secondsElapsed) 
{
	gDegreesRotated += secondsElapsed * degreesPerSecond;
	while (gDegreesRotated > 360.0f) 
		gDegreesRotated -= 360.0f;
}

// JM: This is the update method of the main.cpp - it consistently updates the current frame based on the current time retrieved from GLFW.
// A tertiary variable holds the difference in time between the current and last frame. This delta value is used to update the angle of 
// rotation. The last frame is now set to be the current frame for the next pass through. 
// A #FPS variable is then increased. A conditional statement then assess whether the difference between the curent frame and the last frame
// than 1.0f then we update the FPS count on the screen to reflect this by appending our FPS string with the number of frames converted to a string.
void Update() 
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	UpdateAngle((float)(currentFrame - lastFrame));
	lastFrame = currentFrame;
	nbFrames++;

	if (currentFrame - lastTime >= 1.0) 
	{
		fps = "FPS: " + std::to_string(nbFrames);
		nbFrames = 0;
		lastTime += 1.0;
	}

	// The background colour as well as our bits are cleared, face culling is enabled, as is depth testing.
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// We then poll for events using GLFW, allow for movement callbacks (keyboard input) and update the relevant light positions.
	glfwPollEvents();
	Move();
	lighting.UpdateLightPositions();
}

// JM: This method simply sets up our asset shaders and their attributes.
void SetupShaders()
{
	AssetManager::getShader("lightingShader").use();
	AssetManager::getShader("lightingShader").setInt("material.diffuse", 0);
	AssetManager::getShader("lightingShader").setInt("material.specular", 1);
	AssetManager::getShader("lightingShader").setInt("material.emission", 2);

	AssetManager::getShader("tangentNormalShader").use();
	AssetManager::getShader("tangentNormalShader").setInt("diffuseMap", 0);
	AssetManager::getShader("tangentNormalShader").setInt("normalMap", 1);

	AssetManager::getShader("parallaxShader").use();
	AssetManager::getShader("parallaxShader").setInt("diffuseMap", 0);
	AssetManager::getShader("parallaxShader").setInt("normalMap", 1);
	AssetManager::getShader("parallaxShader").setInt("depthMap", 2);

	AssetManager::getShader("shadowShader").use();
	AssetManager::getShader("shadowShader").setInt("diffuseTexture", 0);
	AssetManager::getShader("shadowShader").setInt("shadowMap", 1);

	AssetManager::getShader("pShadowShader").use();
	AssetManager::getShader("pShadowShader").setInt("diffuseTexture", 0);
	AssetManager::getShader("pShadowShader").setInt("depthMap", 1);

	AssetManager::getShader("simpleInstanceShader").use();
	AssetManager::getShader("simpleInstanceShader").setMat4("projection", projection);

	AssetManager::getShader("instanceShader").use();
	AssetManager::getShader("instanceShader").setMat4("projection", projection);
}

// JM: Handle keyboard input (input callback) for moving.
void Move()
{
	// Movement using WASD
	if (currentCamera == 0)
	{
		if (!drawTrail)
		{
			if (keys[GLFW_KEY_W]) { cameras[currentCamera].ProcessKeyboard(FORWARD, deltaTime, noclip, slowWalk); }

			if (keys[GLFW_KEY_S]) { cameras[currentCamera].ProcessKeyboard(BACKWARD, deltaTime, noclip, slowWalk); }

			if (keys[GLFW_KEY_A]) { cameras[currentCamera].ProcessKeyboard(LEFT, deltaTime, noclip, slowWalk); }

			if (keys[GLFW_KEY_D]) { cameras[currentCamera].ProcessKeyboard(RIGHT, deltaTime, noclip, slowWalk); }
		}
	}

	// Change rendering mode
	if (keys[GLFW_KEY_E])
	{
		if (mode != 1) { mode = 1; glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	} // Toggle line mode

	if (keys[GLFW_KEY_Q])
	{
		if (mode != 0) { mode = 0; glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	} // Toggle fill mode

	if (keys[GLFW_KEY_R])
	{
		if (mode != 2) { mode = 2; glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); }
	} // Toggle point mode
}

// Key callback for various functionality.
void KeyTouch(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// JM:
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) { keys[key] = true; }

		else if (action == GLFW_RELEASE)
		{
			keys[key] = false; keysPressed[key] = false;
		}
	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	} // Exit Program

	if (keys[GLFW_KEY_LEFT_SHIFT])
	{
		slowWalk = true; 
	} // Hold to walk slow
	
	else if (!keysPressed[GLFW_KEY_LEFT_SHIFT])
	{
		slowWalk = false;
	} // Release to walk fast

	if (keys[GLFW_KEY_B] && !keysPressed[GLFW_KEY_B])
	{
		blinn = !blinn; keysPressed[GLFW_KEY_B] = true;
	} // Toggle floor lighting model

	if (keys[GLFW_KEY_SPACE] && !keysPressed[GLFW_KEY_SPACE])
	{
		shadowsEnabled = !shadowsEnabled; keysPressed[GLFW_KEY_SPACE] = true;
	} // Toggle Shadows

	if (keys[GLFW_KEY_ENTER] && !keysPressed[GLFW_KEY_ENTER])
	{
		noclip = !noclip; keysPressed[GLFW_KEY_ENTER] = true;
	} // Toggle Shadows

	if (keys[GLFW_KEY_T] && !keysPressed[GLFW_KEY_T])
	{
		toggleShadowBlinn = !toggleShadowBlinn; keysPressed[GLFW_KEY_T] = true;
	} // Toggle Shadow model

	if (keys[GLFW_KEY_P] && !keysPressed[GLFW_KEY_P])
	{
		height_scale += 0.20; if (height_scale >= 1.5) height_scale = 0.1f; keysPressed[GLFW_KEY_P] = true;
	} // Increase Parallax Depth / Decrease Height

	if (keys[GLFW_KEY_O] && !keysPressed[GLFW_KEY_O])
	{
		height_scale -= 0.20; if (height_scale <= -0.7) height_scale = 0.1f; keysPressed[GLFW_KEY_O] = true;
	} // Decrease Parallax Depth / Increase Height

	if (keys[GLFW_KEY_Y] && !keysPressed[GLFW_KEY_Y])
	{
		quadDebugEnabled = !quadDebugEnabled; keysPressed[GLFW_KEY_Y] = true;
	} // Toggle visible debug Quad

	if (keys[GLFW_KEY_V] && !keysPressed[GLFW_KEY_V])
	{
		activeRoom = !activeRoom; keysPressed[GLFW_KEY_V] = true;
	} // Toggle visible debug Quad

	if (keys[GLFW_KEY_X] && !keysPressed[GLFW_KEY_X])
	{
		isStencil = !isStencil; keysPressed[GLFW_KEY_X] = true;
	} // Toggle object highligting/ stenciling
	
	if (keys[GLFW_KEY_C] && !keysPressed[GLFW_KEY_C])
	{
		imageInt++;
		if (imageInt > 5) { imageInt = 0; }	// Cycle container textures

		containerScene.SetContainerImage(false, imageInt);
		keysPressed[GLFW_KEY_C] = true;
	}

	// EH: toggle the drawTrail boolean (determines if a particle trail is drawn or not)
	// the cursor is enabled if drawTrail is set to true and disabled if drawTrail is set to false
	if (keys[GLFW_KEY_Z] && !keysPressed[GLFW_KEY_Z])
	{
		if (!editMode)
		{
			if (!drawTrail)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				drawTrail = true;
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				drawTrail = false;
			}
			keysPressed[GLFW_KEY_Z] = true;
		}
	}
	// SC: Edit Mode Control (Turning Edit Mode On/Off)
	if (keys[GLFW_KEY_G])
	{
		if (!editMode && oatsCubes.size() > 0 )
		{
			editMode = true;
		}
		else
		{
			editMode = false;
		}

		keys[GLFW_KEY_G] = false;
	}

	// SC: Edit Mode Specific Controls
	if (editMode)
	{
		//SC : Navigating Edit Mode Options
		if (keys[GLFW_KEY_DOWN])
		{
			if (editModeIndex <= 11)
			{
				editModeIndex++;
			}

			if (editModeIndex > 11)
			{
				editModeIndex = 0;
			}
			keys[GLFW_KEY_DOWN] = false;
		}


		if (keys[GLFW_KEY_UP])
		{
			if (editModeIndex >= 0)
			{
				editModeIndex--;
			}

			if (editModeIndex < 0)
			{
				editModeIndex = 11;
			}
			keys[GLFW_KEY_UP] = false;
		}

		//SC : Changing Option Values (Positive)
		if (keys[GLFW_KEY_RIGHT])
		{
			//SC : Custom Script Option
			if (editModeIndex == 0)
			{
				customScriptVector.x += 0.01f;
			}

			if (editModeIndex == 1)
			{
				customScriptVector.y += 0.01f;
			}

			if (editModeIndex == 2)
			{
				customScriptVector.z += 0.01f;
			}

			//SC : Translation 
			if (editModeIndex == 3)
			{
				oatsCubes[gameObjectIndex]->transform.Translation.x += 0.01f;
			}

			if (editModeIndex == 4)
			{
				oatsCubes[gameObjectIndex]->transform.Translation.y += 0.01f;
			}

			if (editModeIndex == 5)
			{
				oatsCubes[gameObjectIndex]->transform.Translation.z += 0.01f;
			}

			//SC : Scale
			if (editModeIndex == 6)
			{
				oatsCubes[gameObjectIndex]->transform.Scale.x += 0.01f;
			}

			if (editModeIndex == 7)
			{
				oatsCubes[gameObjectIndex]->transform.Scale.y += 0.01f;
			}

			if (editModeIndex == 8)
			{
				oatsCubes[gameObjectIndex]->transform.Scale.z += 0.01f;
			}

			//SC : Rotation
			if (editModeIndex == 9)
			{
				oatsCubes[gameObjectIndex]->transform.Rotation.x += 0.01f;
			}

			if (editModeIndex == 10)
			{
				oatsCubes[gameObjectIndex]->transform.Rotation.y += 0.01f;
			}

			if (editModeIndex == 11)
			{
				oatsCubes[gameObjectIndex]->transform.Rotation.z += 0.01f;
			}
		}

		//SC : Changing Option Values (Negative)
		if (keys[GLFW_KEY_LEFT])
		{
			//SC : Custom Script Option
			if (editModeIndex == 0)
			{
				customScriptVector.x -= 0.01f;
			}

			if (editModeIndex == 1)
			{
				customScriptVector.y -= 0.01f;
			}

			if (editModeIndex == 2)
			{
				customScriptVector.z -= 0.01f;
			}

			//SC : Translation
			if (editModeIndex == 3)
			{
				oatsCubes[gameObjectIndex]->transform.Translation.x -= 0.01f;
			}

			if (editModeIndex == 4)
			{
				oatsCubes[gameObjectIndex]->transform.Translation.y -= 0.01f;
			}

			if (editModeIndex == 5)
			{
				oatsCubes[gameObjectIndex]->transform.Translation.z -= 0.01f;
			}

			//SC : Scale
			if (editModeIndex == 6)
			{
				oatsCubes[gameObjectIndex]->transform.Scale.x -= 0.01f;
			}

			if (editModeIndex == 7)
			{
				oatsCubes[gameObjectIndex]->transform.Scale.y -= 0.01f;
			}

			if (editModeIndex == 8)
			{
				oatsCubes[gameObjectIndex]->transform.Scale.z -= 0.01f;
			}

			//SC : Rotation
			if (editModeIndex == 9)
			{
				oatsCubes[gameObjectIndex]->transform.Rotation.x -= 0.01f;
			}

			if (editModeIndex == 10)
			{
				oatsCubes[gameObjectIndex]->transform.Rotation.y -= 0.01f;
			}

			if (editModeIndex == 11)
			{
				oatsCubes[gameObjectIndex]->transform.Rotation.z -= 0.01f;
			}
		}

		//SC : Adding Translation Script
		if (keys[GLFW_KEY_1])
		{
			oatsCubes[gameObjectIndex]->AddScript(new TranslateScript(customScriptVector));
			keys[GLFW_KEY_1] = false;
		}

		//SC : Adding Scale Script
		if (keys[GLFW_KEY_2])
		{
			oatsCubes[gameObjectIndex]->AddScript(new ScaleScript(customScriptVector));
			keys[GLFW_KEY_2] = false;
		}

		//SC : Adding Rotation Script
		if (keys[GLFW_KEY_3])
		{
			oatsCubes[gameObjectIndex]->AddScript(new RotateScript(customScriptVector));
			keys[GLFW_KEY_3] = false;
		}

		//SC : Removing Translation Script
		if (keys[GLFW_KEY_4])
		{
			oatsCubes[gameObjectIndex]->RemoveScript<TranslateScript>();
			keys[GLFW_KEY_4] = false;
		}

		//SC : Removing Scale Script
		if (keys[GLFW_KEY_5])
		{
			oatsCubes[gameObjectIndex]->RemoveScript<ScaleScript>();
			keys[GLFW_KEY_5] = false;
		}

		//SC : Removing Rotate Script
		if (keys[GLFW_KEY_6])
		{
			oatsCubes[gameObjectIndex]->RemoveScript<RotateScript>();
			keys[GLFW_KEY_6] = false;
		}

		//SC : Cycling through GameObjects (Negative)
		if (keys[GLFW_KEY_LEFT_BRACKET])
		{
			if (gameObjectIndex >= 0)
			{
				gameObjectIndex--;
			}

			if (gameObjectIndex < 0)
			{
				gameObjectIndex = oatsCubes.size() - 1;
			}

			keys[GLFW_KEY_LEFT_BRACKET] = false;
		}

		//SC : Cycling through GameObjects (Positive)
		if (keys[GLFW_KEY_RIGHT_BRACKET])
		{
			if (gameObjectIndex <= oatsCubes.size() - 1)
			{
				gameObjectIndex++;
			}

			if (gameObjectIndex > oatsCubes.size() - 1)
			{
				gameObjectIndex = 0;
			}

			keys[GLFW_KEY_RIGHT_BRACKET] = false;
		}
	}
}

// EH: function for changing the texture atlas applied to a given object by using the arrow keys
void ChangeAtlasTexture(TextureAtlas &atlas)
{
	if (!editMode) //arrow keys are only used for this is edit mode is not enabled
	{
		if (keys[GLFW_KEY_UP] && !keysPressed[GLFW_KEY_UP])
		{
			atlas.changeTexture(AssetManager::getTexture("atlas1"), 8);
			keysPressed[GLFW_KEY_UP] = true;
		}
		if (keys[GLFW_KEY_LEFT] && !keysPressed[GLFW_KEY_LEFT])
		{
			atlas.changeTexture(AssetManager::getTexture("atlas2"), 4);
			keysPressed[GLFW_KEY_LEFT] = true;
		}
		if (keys[GLFW_KEY_RIGHT] && !keysPressed[GLFW_KEY_RIGHT])
		{
			atlas.changeTexture(AssetManager::getTexture("atlas3"), 7);
			keysPressed[GLFW_KEY_RIGHT] = true;
		}
		if (keys[GLFW_KEY_DOWN] && !keysPressed[GLFW_KEY_DOWN])
		{
			atlas.changeTexture(AssetManager::getTexture("atlas4"), 10);
			keysPressed[GLFW_KEY_DOWN] = true;
		}
	}
}

// Handle moust movement callback
void MouseMovement(GLFWwindow *window, double xPos, double yPos)
{
	// Set up for first time use within the program.
	if (firstMouseMovement)
	{
		lastXPosition = xPos;
		lastYPosition = yPos;
		firstMouseMovement = false;
	}

	// EH: if statement to ensure that movement is only processed if the particle trail is not being drawn
	if (!drawTrail)
	{
		// Create relevant offsets and assign x and y positions.
		GLfloat xOffset = xPos - lastXPosition;
		GLfloat yOffset = lastYPosition - yPos;
		lastXPosition = xPos;
		lastYPosition = yPos;
		// Use these offsets to alter the cameras direction.
		cameras[currentCamera].ProcessMouseMovement(xOffset, yOffset);
	}
	// EH: is drawTrail is true then the mousePos vec3 is updated (the trail object uses this to determine where the particles are spawned)
	else
	{
		mousePos.x = (xPos - (WIDTH / 2)) / 55;
		mousePos.y = ((yPos - (HEIGHT / 2)) / 55)*-1;
		mousePos.y += 1.75;
	}
}

// JM: Handle mouse button callback - switch between cameras.
void MouseButtonCallbackFunction(GLFWwindow* window, int button, int pr, int mods) // Cycle through Cameras
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) { currentCamera = 1; }
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) { currentCamera = 2; }
	if (button == GLFW_MOUSE_BUTTON_RIGHT) { currentCamera = 0; }
}

// SC: Spawn Example Cube GameObjects
void SpawnGameObjectCubes(const glm::mat4 &projection)
{
	if (keys[GLFW_KEY_1])
	{
		Cube * cube = new Cube(&camera, projection);
		oatsCubes.push_back(&cube->gameObject);

		keys[GLFW_KEY_1] = false;
	}
}