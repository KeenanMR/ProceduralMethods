#include "Instancing.h"

//CONSTRUCTOR
Instancing::Instancing(int res, D3D* rend, TerrainMesh* terr, InstancedCubeMesh* cube)
{
	//VARIABLE SETUP
	resolution = res;
	renderer = rend;
	mTerrain = terr;
	mInstancedCube = cube;
}

void Instancing::BuildCubeInstances() {

	//GET THE MAX POSSIBLE CUBES GENERATED - THIS CAN HANDLE 1024 X 1024 RESOLUTION
	const int width = 110;
	const int maxCubes = width * width * width;

	//SETS THE POSSIBLE POSITIONS TO THE ORIGIN
	XMFLOAT3* pos = new XMFLOAT3[maxCubes];
	for (int k = 0; k < maxCubes; k++)
	{
		pos[k] = XMFLOAT3(0, 0, 0);
	}

	float height = 0;
	int instanceCount = 0;

	//GENERATES A HEIGHT AND POSITION FOR EACH CUBE INSTANCE 
	for (int j = 0; j < (mTerrain->GetResolution()); j++) {
		for (int i = 0; i < (mTerrain->GetResolution()); i++) {

			height = mTerrain->getHeightAt(i, j);

			pos[instanceCount] = XMFLOAT3(i, height, j);
			instanceCount++;

		}
	}

	mInstancedCube->initBuffers(renderer->getDevice(), pos, instanceCount);

	delete[] pos;
	pos = 0;
}