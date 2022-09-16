// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"
#include "TerrainMesh.h"
#include "InstanceShader.h"
#include "InstancedCubeMesh.h"
#include "Instancing.h"
#include "LSystem.h"
#include "LineMesh.h"
#include "Poisson.h"
class App1 : public BaseApplication
{
public:

	//CONSTUCTOR AND DESTRUCTOR
	App1();
	~App1();

	//INITIALISATION
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	//FUNCTION TO RENDER GEOMETRY
	bool render();

	//FUNCTION TO RENDER THE GUI
	void gui();

private:

	//VOXEL INSTANCING VARIABLES
	InstanceShader* mInstanceShader;
	InstancedCubeMesh* mInstancedCube;
	Instancing* instancer;

	//SHADER VARIABLES
	LightShader* shader;
	TextureShader* tShader;

	//MESH VARIABLES
	TerrainMesh* mTerrain;
	LineMesh* mLine;

	//LIGHT VARIABLES
	Light* light;

	//LSYSTEM VARIABLES
	LSystem* lSystem;



	//MANIP[UL;ATION VARIABLES
	int terrainResolution = 100;
	int fOffset = 2, fScaling = 1, fIterations = 100, fSIterations = 3, pDrop = 5, tOctaves = 10, tAmplitude = 7, vOctaves = 10, vAmplitude = 7, l1Iterations360 = 20, l1Iterations = 7, l2Iteration = 7;
	float tFreq = 0.008, vFreq = 0.008;

	//MODE BOOLEANS
	bool mode = false;
	bool lMode = false;
};

#endif