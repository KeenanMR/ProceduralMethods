#include "InstancedCubeMesh.h"

InstancedCubeMesh::InstancedCubeMesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int lresolution, int instanceCount )
	: CubeMesh( device, deviceContext, lresolution ){
	mInstanceBuffer = 0;
}

InstancedCubeMesh::~InstancedCubeMesh() {
	// Run parent destructor
	CubeMesh::~CubeMesh();

	// Release the instance buffer.
	if( mInstanceBuffer ) {
		mInstanceBuffer->Release();
		mInstanceBuffer = 0;
	}
}

int InstancedCubeMesh::GetInstanceCount() {
	return mInstanceCount;
}

// Initialise instance buffers.
// Generate and store cube vertices, normals and texture coordinates
void InstancedCubeMesh::initBuffers( ID3D11Device* device, XMFLOAT3* p, int count ) {
	if( mInstanceBuffer != NULL ) {
		mInstanceBuffer->Release();
	}
	InstanceType* instances;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;

	// Set the number of instances in the array.
	mInstanceCount = count;
	int rt = cbrt( mInstanceCount );

	// Create the instance array.
	instances = new InstanceType[mInstanceCount];

	// Load the instance array with data.
	for( int inst = 0; inst < mInstanceCount; inst++ ) {
		instances[inst].position = p[inst];
	}

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof( InstanceType ) * mInstanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	device->CreateBuffer( &instanceBufferDesc, &instanceData, &mInstanceBuffer );
	
	// Release the instance array now that the instance buffer has been created and loaded.
	delete[] instances;
	instances = 0;
}

// Sends geometry data to the GPU. Default primitive topology is TriangleList.
// This function is called instead of sendData in order to send the instance buffer as well as the vertex & index buffers
void InstancedCubeMesh::sendDataInstanced( ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top ) {
	unsigned int strides[2];
	unsigned int offsets[2];

	ID3D11Buffer* bufferPointers[2];

	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = mInstanceBuffer;

	// Set vertex buffer stride and offset.
	strides[0] = sizeof( VertexType );
	strides[1] = sizeof( InstanceType );

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	deviceContext->IASetVertexBuffers( 0, 2, bufferPointers, strides, offsets );
	deviceContext->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	deviceContext->IASetPrimitiveTopology( top );
}