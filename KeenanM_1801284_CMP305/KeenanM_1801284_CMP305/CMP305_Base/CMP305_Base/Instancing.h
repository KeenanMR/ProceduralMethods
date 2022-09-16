#pragma once

#include "InstancedCubeMesh.h"
#include "InstanceShader.h"
#include "LineMesh.h"
#include "TerrainMesh.h"

//CLASS TO INSTANCE DIFFERENT MESHES
class Instancing
{
public:
	//CONSTRUCTOR
	Instancing(int res, D3D* rend, TerrainMesh* terr, InstancedCubeMesh* cube);

	//FUCNTION FOR INSTANCING THE CUBE MESH
	void BuildCubeInstances();
private:

	//VARIABLES FOR STORING INIT DATA
	int resolution;
	D3D* renderer;
	TerrainMesh* mTerrain;

	//INSTANCE MESH
	InstancedCubeMesh* mInstancedCube;

};

