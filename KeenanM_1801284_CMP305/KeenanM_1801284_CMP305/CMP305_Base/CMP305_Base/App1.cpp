#include "App1.h"

App1::App1()
{
	mTerrain = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// CALL THE INIT FUNCTION
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Load textures
	textureMgr->loadTexture(L"grass", L"res/grass.png");
	textureMgr->loadTexture(L"mountain", L"res/mountain.png");
	textureMgr->loadTexture(L"water", L"res/water.png");
	textureMgr->loadTexture(L"snow", L"res/snow.png");

	textureMgr->loadTexture(L"m_dirt", L"res/minecraft_dirt.png");
	textureMgr->loadTexture(L"m_water", L"res/minecraft_water.png");
	textureMgr->loadTexture(L"m_snow", L"res/minecraft_snow.png");
	textureMgr->loadTexture(L"m_grass", L"res/minecraft_grass.png");
	textureMgr->loadTexture(L"m_stone", L"res/minecraft_stone.png");

	textureMgr->loadTexture(L"white", L"res/DefaultDiffuse.png");
	

	//CREATE THE TERRAIN MESH OBJECT AND THE SHADERS
	mTerrain = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext());
	shader = new LightShader(renderer->getDevice(), hwnd);
	tShader = new TextureShader(renderer->getDevice(), hwnd);

	//INITIALISE THE LIGHT
	light = new Light();
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(1.0f, -1.0f, 0.0f);

	//INITIALISE THE INSTANCING VARIABLES
	mInstancedCube = new InstancedCubeMesh(renderer->getDevice(), renderer->getDeviceContext(), 1);
	mInstanceShader = new InstanceShader(renderer->getDevice(), hwnd);
	instancer = new Instancing(terrainResolution,renderer,mTerrain,mInstancedCube);

	//INITILIASE THE LSYSTEM VARIABLES
	lSystem = new LSystem(renderer);
	lSystem->SetAxiom("A");
	lSystem->AddRule('A', "B[A]A");
	lSystem->AddRule('B', "BB");
	lSystem->AddRule('F', "F-G+F+G-F");
	lSystem->AddRule('G', "GG");
	mLine = new LineMesh(renderer->getDevice(), renderer->getDeviceContext());

}


App1::~App1()
{
	//RUN BASE APPLICATION DESTRUCTOR
	BaseApplication::~BaseApplication();

	//RELEASE THE DIRECT 3D OBJECT
	if (mTerrain)
	{
		delete mTerrain;
		mTerrain = 0;
	}

	if (shader)
	{
		delete shader;
		shader = 0;
	}

	if (mInstanceShader) {
		delete mInstanceShader;
		mInstanceShader = 0;
	}
	if (mInstancedCube) {
		delete mInstancedCube;
		mInstancedCube = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	//RENDER THE GRAPHICS
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//CLEAR THE SCENE AND SET TO DEFAULT COLOUR
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// UPDATE THE CAMERA AND GENERATE THE MATRIXES
	camera->update();

	// GET THE WORLD, VIEW AND PROJECTION MATRIX
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();


	//SEND GEOMETRY DATA, SET SHADER PARAMETERS AND RENDER
	if (!mode && !lMode)
	{
		//TERRAIN MESH
		mTerrain->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"water"), textureMgr->getTexture(L"grass"), textureMgr->getTexture(L"snow"), textureMgr->getTexture(L"mountain"), light);
		shader->render(renderer->getDeviceContext(), mTerrain->getIndexCount());

	
	}
	if (mode && !lMode)
	{
		//VOXEL GENERATION
	mInstanceShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"m_dirt"), textureMgr->getTexture(L"m_grass"), textureMgr->getTexture(L"m_water"), textureMgr->getTexture(L"m_stone"), textureMgr->getTexture(L"m_snow"), light);
	mInstancedCube->sendDataInstanced(renderer->getDeviceContext());
	mInstanceShader->renderInstanced(renderer->getDeviceContext(), mInstancedCube->getIndexCount(), mInstancedCube->GetInstanceCount());
	}

	if (lMode)
	{
		if (mLine->getIndexCount() > 0) {
			//LSYSTEM GENERATION
			tShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"));
			mLine->sendData(renderer->getDeviceContext());
			tShader->render(renderer->getDeviceContext(), mLine->getIndexCount());
		}
	}


	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// TURN OF UNNECESSARY SHADER STAGES
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// BUILD UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	ImGui::Checkbox("Voxel mode", &mode);
	ImGui::Checkbox("LSystem mode", &lMode);


	ImGui::SliderInt("Terrain Resolution", &terrainResolution, 2, 1024);
	
	//CHECK WHAT MODE THE UI IS IN
	if (!mode && !lMode)
	{
		//RENDERS THE GUI WITH MANIPULATION SLIDERS AND THE BUTTONS TO RENDER GEOMETRY DIFFERENTLY
		if (ImGui::Button("Regenerate Terrain")) {
			if (terrainResolution != mTerrain->GetResolution()) {
				mTerrain->Resize(terrainResolution);

			}
			mTerrain->BuildHeightMap();
			mTerrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (ImGui::Button("Smooth Terrain")) {
			if (terrainResolution != mTerrain->GetResolution()) {
				mTerrain->Resize(terrainResolution);

			}
			mTerrain->Smooth(1);
			mTerrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (ImGui::Button("Fault Terrain")) {
			if (terrainResolution != mTerrain->GetResolution()) {
				mTerrain->Resize(terrainResolution);

			}

			mTerrain->BuildHeightMap();
			mTerrain->Fault(fOffset, fScaling, fIterations);
			mTerrain->Smooth(fSIterations);
			mTerrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext());

		}

		ImGui::SliderInt("Fault Offset Amount", &fOffset, 1, 10);
		ImGui::SliderInt("Fault Scaling", &fScaling, 1, 5);
		ImGui::SliderInt("Fault Iterations", &fIterations, 1, 200);
		ImGui::SliderInt("Fault Smoothing Iterations", &fSIterations, 0, 10);

		if (ImGui::Button("Particle Drop")) {
			if (terrainResolution != mTerrain->GetResolution()) {
				mTerrain->Resize(terrainResolution);

			}
			mTerrain->ParticleDeposition(pDrop);
			mTerrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		}

		ImGui::SliderInt("Particle Drop Amount", &pDrop, -10, 10);

		if (ImGui::Button("Generate Flate Plane")) {
			if (terrainResolution != mTerrain->GetResolution()) {
				mTerrain->Resize(terrainResolution);

			}
			mTerrain->GenerateFlatPlane();
			mTerrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		}

		if (ImGui::Button("Perlin Noise")) {
			if (terrainResolution != mTerrain->GetResolution()) {
				mTerrain->Resize(terrainResolution);

			}

			mTerrain->noiseCall2D(tOctaves, tFreq, tAmplitude);
			mTerrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext());

		}
		ImGui::SliderInt("Terrain Octaves", &tOctaves, 1, 10);
		ImGui::SliderFloat("Terrain Frequency", &tFreq, 0,0.1);
		ImGui::SliderInt("Terrain Amplitude", &tAmplitude, 1, 20);
	}

	if (mode && !lMode)
	{
		if (ImGui::Button("Voxel Perlin")) {
			if (terrainResolution != mTerrain->GetResolution()) {
				mTerrain->Resize(terrainResolution);

			}
			mTerrain->noiseCall2D(vOctaves, vFreq, vAmplitude);
			mTerrain->RoundHeight();
			instancer->BuildCubeInstances();
			mTerrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext());
		}

		ImGui::SliderInt("Voxel Octaves", &vOctaves, 1, 10);
		ImGui::SliderFloat("Voxel Frequency", &vFreq, 0, 0.1);
		ImGui::SliderInt("Voxel Amplitude", &vAmplitude, 1, 20);
	}

	if (lMode)
	{
		if (ImGui::Button("LSystem 1")) {
			lSystem->resetCycle();
			lSystem->SetAxiom("A");
			lSystem->Run(l1Iterations);
			lSystem->Generate(true, l1Iterations360);
			mLine = lSystem->GetGeneration();
		}
		ImGui::SliderInt("LSytem 1 Iterations", &l1Iterations, 1, 10);
		ImGui::SliderInt("LSytem 1 Iterations 360", &l1Iterations360, 1, 50);

		if (ImGui::Button("LSystem 2")) {
			lSystem->resetCycle();
			lSystem->SetAxiom("F-G-G");
			lSystem->Run(l2Iteration);
			lSystem->Generate(false, 0);
			mLine = lSystem->GetGeneration();
		}
		ImGui::SliderInt("LSytem 2 Iterations", &l2Iteration, 1, 10);
	}

	if (ImGui::Button("New Seed"))
	{
		mTerrain->getPHeight().seed();
	}



	//RENDER UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


