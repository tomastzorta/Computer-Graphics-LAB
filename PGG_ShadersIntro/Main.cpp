// This code defines an OpenGL library for generating and linking GLEW, including a programming error message to the console. It also includes variables that can be used in various libraries such as glew's features or use it with glue-based programs.

// This is the main SDL include file
#include <SDL/SDL.h>

// I have added glew.h, wglew.h and glew.c to my VS project
// I find this the easiest way to include it
// To compile and link GLEW like this ('statically') you must add  GLEW_STATIC  into Configuration Properties -> C/C++ -> Preprocessor -> Preprocessor Definitions
#include "glew.h"

#include "Scene.h"

// GUI system: https://github.com/ocornut/imgui
// - prevent compile error by building with: WINDOWS_IGNORE_PACKING_MISMATCH
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

// iostream is so we can output error messages to console
#include <iostream>

bool showLightingWindow = true;

// An initialisation function, mainly for GLEW
// This will also print to console the version of OpenGL we are using
bool InitialiseSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Whoops! Something went very wrong, cannot initialise SDL :(" << std::endl;
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    return true;
}

bool InitialiseGLEW() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    return true;
}


bool InitialiseOpenGL(SDL_Window* window, SDL_GLContext& glcontext) {
    glcontext = SDL_GL_CreateContext(window);
    if (!InitialiseGLEW()) {
        return false;
    }
    return true;
}

SDL_Window* CreateSDLWindow(int width, int height) {
    int winPosX = 100, winPosY = 100;
    SDL_Window* window = SDL_CreateWindow("s5216712 - Osman Tzorta GEP", winPosX, winPosY, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	//unlimited fps
    return window;
}

void InitialiseGUI(SDL_Window* window, SDL_GLContext& glcontext) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    const char* glslVersion = "#version 130";
    ImGui_ImplSDL2_InitForOpenGL(window, glcontext);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

void HandleEvents(bool& running, SDL_Event& incomingEvent) {
    while (SDL_PollEvent(&incomingEvent)) {
        ImGui_ImplSDL2_ProcessEvent(&incomingEvent);
        if (incomingEvent.type == SDL_QUIT) {
            running = false;
        }
        // Additional event handling...
    }
}

void UpdateScene(Scene& myScene, float deltaTs) {
    myScene.Update(deltaTs);
}

void RenderScene(Scene& myScene) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    myScene.Draw();
}

void RenderGUI(Scene& myScene, bool& showLightingWindow) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    if (showLightingWindow)
		{
			// Create a window, give it a name
			// All ImGui commands after this to create widgets will be added to the window
			ImGui::Begin("Cube Animation Controls");
			
			// Here, we do the same sort of thing several times for different properties:
			//  1. Get a state from the scene
			//  2. Use the GUI to present an editor for it
			//  3. Send it back to the scene in case it's changed

			// Checkbox for whether or not the light is rotating
			bool currentAnimLight = myScene.m_animationManager.GetAnimateLight();
			if (ImGui::Checkbox("Animate Light", &currentAnimLight)) {
				myScene.m_animationManager.SetAnimateLight(currentAnimLight);
			}

			// Slider for the current angle of the light
			float lightAngle = myScene.m_animationManager.GetLightAngle();
			if (ImGui::SliderFloat("Light Angle", &lightAngle, 0.0f, 2.0f * 3.141592653589793238462643383)) {
				myScene.m_animationManager.SetLightAngle(lightAngle);
			}

			// Checkbox for the cube's animation
			bool currentAnimCube = myScene.m_animationManager.GetAnimateCube();
			if (ImGui::Checkbox("Animate Cube", &currentAnimCube)) {
				myScene.m_animationManager.SetAnimateCube(currentAnimCube);
			}

			// Slider for the cube's rotation angle
			float cube1Angle = myScene.m_animationManager.GetCube1Angle();
			if (ImGui::SliderFloat("Cube 1 Angle", &cube1Angle, 0.0f, 2.0f * 3.141592653589793238462643383)) {
				myScene.m_animationManager.SetCube1Angle(cube1Angle);
			}
			ImGui::End();

			ImGui::Begin("Lighting Models");
			// Shader Control Buttons
			if (ImGui::Button("Use Phyisical Based Rendering (Fresnel)"))
			{
				myScene.SetCurrentShader("PBR");
			}
			if (ImGui::Button("Use Phong Lighting"))
			{
				myScene.SetCurrentShader("Phong");
			}
    		if (ImGui::Button("Use Disney PBR"))
    		{
				myScene.SetCurrentShader("Disney");
			}
			ImGui::End();

			// Cube Settings Window
			ImGui::Begin("Cube Settings");
			if (myScene.GetCurrentShader() == "Phong")
			{
				// Colour editor for the diffuse colour
				glm::vec3 currentCubeDiffuseCol = myScene.m_cubeModel.GetCubeColour();
				if (ImGui::ColorEdit3("Cube Colour", &(currentCubeDiffuseCol[0])))
				{
					myScene.m_cubeModel.SetCubeColour(currentCubeDiffuseCol);
				}
				// Slider for cube shininess
				float cubeShininess = myScene.m_cubeModel.GetCubeShininess();
				if (ImGui::SliderFloat("Cube Shininess", &cubeShininess, 0.0f, 100.0f))
				{
					myScene.m_cubeModel.SetCubeShininess(cubeShininess);
				}

				// Colour editor for the specular colour
				glm::vec3 currentCubeSpecularCol = myScene.m_cubeModel.GetLightColour();
				if (ImGui::ColorEdit3("Light Colour", &(currentCubeSpecularCol[0])))
				{
					myScene.m_cubeModel.SetLightColour(currentCubeSpecularCol);
				}
			}
			else if (myScene.GetCurrentShader() == "PBR")
			{
				// Colour editor for the diffuse colour
				glm::vec3 currentCubeDiffuseCol = myScene.m_cubeModel.GetCubeColour();
				if (ImGui::ColorEdit3("Cube Colour", &(currentCubeDiffuseCol[0])))
				{
					myScene.m_cubeModel.SetCubeColour(currentCubeDiffuseCol);
				}
				// Colour editor for the specular colour
				glm::vec3 currentCubeSpecularCol = myScene.m_cubeModel.GetLightColour();
				if (ImGui::ColorEdit3("Light Colour", &(currentCubeSpecularCol[0])))
				{
					myScene.m_cubeModel.SetLightColour(currentCubeSpecularCol);
				}
				//metallic
				bool metallic = myScene.m_cubeModel.GetCubeMetallic();
				if (ImGui::Checkbox("Is Cube Metallic?", &metallic))
				{
					myScene.m_cubeModel.SetCubeMetallic(metallic);
				}

				//roughness
				float roughness = myScene.m_cubeModel.GetCubeRoughness();
				if (ImGui::SliderFloat("Cube Roughness", &roughness, 0.0f, 1.0f))
				{
					myScene.m_cubeModel.SetCubeRoughness(roughness);
				}
			}
    		else if (myScene.GetCurrentShader() == "Disney")
			{
    			if (myScene.GetCurrentShader() == "Disney")
    			{
    				ImGui::Text("Cube Presets");
				    if (ImGui::Button("Brushed Metal Cube"))
				    {
				    	myScene.SetBrushedMetalCube();
				    }
				    if (ImGui::Button("Plastic Cube"))
				    {
					    myScene.SetPlasticCube();
				    }
				    
    				// Colour editor for the albedo
    				glm::vec3 currentAlbedo = myScene.m_cubeModel.GetCubeColour();
    				if (ImGui::ColorEdit3("Cube Colour", &(currentAlbedo[0])))
    				{
    					myScene.m_cubeModel.SetCubeColour(currentAlbedo);
    				}

    				//metallic
    				bool metallic = myScene.m_cubeModel.GetCubeMetallic();
    				if (ImGui::Checkbox("Is Cube Metallic?", &metallic))
    				{
    					myScene.m_cubeModel.SetCubeMetallic(metallic);
    				}

    				// Slider for subsurface
    				float currentSubsurface = myScene.m_cubeModel.GetCubeSubsurface();
    				if (ImGui::SliderFloat("Subsurface", &currentSubsurface, 0.0f, 1.0f))
    				{
    					myScene.m_cubeModel.SetCubeSubsurface(currentSubsurface);
    				}

    				//roughness
    				float roughness = myScene.m_cubeModel.GetCubeRoughness();
    				if (ImGui::SliderFloat("Cube Roughness", &roughness, 0.0f, 1.0f))
    				{
    					myScene.m_cubeModel.SetCubeRoughness(roughness);
    				}

    				// slider for specular
    				float currentSpecular = myScene.m_cubeModel.GetCubeSpecular();
    				if (ImGui::SliderFloat("Specular", &currentSpecular, 0.0f, 1.0f))
    				{
    					myScene.m_cubeModel.SetCubeSpecular(currentSpecular);
    				}

    				// slider for specular tint
    				float currentSpecularTint = myScene.m_cubeModel.GetCubeSpecularTint();
    				if (ImGui::SliderFloat("Specular Tint", &currentSpecularTint, 0.0f, 1.0f))
    				{
    					myScene.m_cubeModel.SetCubeSpecularTint(currentSpecularTint);
    				}

    				// slider for anisotropic
    				float currentAnisotropic = myScene.m_cubeModel.GetCubeAnisotropic();
    				if (ImGui::SliderFloat("Anisotropic", &currentAnisotropic, 0.0f, 1.0f))
    				{
    					myScene.m_cubeModel.SetCubeAnisotropic(currentAnisotropic);
    				}

    				// Slider for sheen
    				float currentSheen = myScene.m_cubeModel.GetCubeSheen();
    				if (ImGui::SliderFloat("Sheen", &currentSheen, 0.0f, 1.0f))
    				{
    					myScene.m_cubeModel.SetCubeSheen(currentSheen);
    				}

    				// Slider for sheen tint
    				float currentSheenTint = myScene.m_cubeModel.GetCubeSheenTint();
    				if (ImGui::SliderFloat("Sheen Tint", &currentSheenTint, 0.0f, 1.0f))
    				{
						myScene.m_cubeModel.SetCubeSheenTint(currentSheenTint);
					}

					// slider for clearcoat
					float currentClearcoat = myScene.m_cubeModel.GetCubeClearcoat();
					if (ImGui::SliderFloat("Clearcoat", &currentClearcoat, 0.0f, 1.0f))
					{
						myScene.m_cubeModel.SetCubeClearcoat(currentClearcoat);
					}

    				// slider for clearcoat gloss
    				float currentClearcoatGloss = myScene.m_cubeModel.GetCubeClearcoatGloss();
    				if (ImGui::SliderFloat("Clearcoat Gloss", &currentClearcoatGloss, 0.0f, 1.0f))
    				{
    					myScene.m_cubeModel.SetCubeClearcoatGloss(currentClearcoatGloss);
    				}

    				// Colour editor for the specular colour
    				glm::vec3 currentCubeSpecularCol = myScene.m_cubeModel.GetLightColour();
    				if (ImGui::ColorEdit3("Light Colour", &(currentCubeSpecularCol[0])))
					{
						myScene.m_cubeModel.SetLightColour(currentCubeSpecularCol);
					}
    			}
			}
    	
			ImGui::End();
			// Statistic Window
			ImGui::Begin("Statistics");
			// Showing how to insert text into a string and also get FPS!!
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
			
		}
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(int argc, char *argv[]) {
    if (!InitialiseSDL()) return -1;

    SDL_Window* window = CreateSDLWindow(1280, 1280);
    if (!window) return -1;

    SDL_GLContext glcontext;
    if (!InitialiseOpenGL(window, glcontext)) return -1;

    InitialiseGUI(window, glcontext);

    bool running = true;
    SDL_Event incomingEvent;

    Scene myScene;
    unsigned int lastTime = SDL_GetTicks();
    glEnable(GL_DEPTH_TEST);

    while (running) {
        HandleEvents(running, incomingEvent);
        unsigned int current = SDL_GetTicks();
        float deltaTs = (float)(current - lastTime) / 1000.0f;
        lastTime = current;

        UpdateScene(myScene, deltaTs);
        RenderScene(myScene);
        RenderGUI(myScene, showLightingWindow);

        SDL_GL_SwapWindow(window);

        if (deltaTs < (1.0f / 50.0f)) {
            SDL_Delay((unsigned int)(((1.0f / 50.0f) - deltaTs) * 1000.0f));
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}