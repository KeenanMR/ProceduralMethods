#pragma once
#include "PlaneMesh.h"
#include "Perlin.h"


class TerrainMesh :
	public PlaneMesh {
public:
	TerrainMesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128 );
	~TerrainMesh();

	void Resize( int newResolution );
	void Regenerate( ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Smooth(int iterations); //SMOOTH THE TERRAIN
	void BuildHeightMap(); // BUILDS A NEW HEIGHT MAP
	void Fault(float faultDifference, float scaling, float iterations); //FAULTS THE TERRAIN MESH
	void ParticleDeposition(int offset); //INDENTS A RANDOM POSITION UP VERTICALLY
	void GenerateFlatPlane(); //SETS THE HEIGHT MAP TO 0 TO GENRATE A FLAT PLANE
	const inline int GetResolution(){ return resolution; } //RETURN THE RESOLUTION
	void noiseCall2D(int octaves, float pFrequency, float pAmplitude); //2D PERLIN NOISE CALL
	void RoundHeight(); //ROUNDS THE HEIGHT TO AN INTEGER
	float getHeightAt(int x, int y); //RETURNS THE HEIGHT MAP AT A POINT
	Perlin getPHeight(); //GET THE PERLIN VARIABLE
	
	

	
private:
	void CreateBuffers( ID3D11Device* device, VertexType* vertices, unsigned long* indices );
	

	const float mUVscale = 10.0f;			//Tile the UV map 10 times across the plane
	const float terrainSize = 100.0f;		//What is the width and height of our terrain
	float* heightMap;

	//PERLIN VARIABLES
	Perlin height;




};
