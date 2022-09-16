#include "LineMesh.h"

LineMesh::LineMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

//Cleanup the heightMap
LineMesh::~LineMesh() {
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

void LineMesh::BuildLine(ID3D11DeviceContext* deviceContext, ID3D11Device* device) {
	//Clear the vertex buffers
	if (vertexBuffer != NULL) {
		vertexBuffer->Release();
	}
	vertexBuffer = NULL;

	vertexCount = mLineList.size() * 2;
	indexCount = mLineList.size() * 2;

	delete[] vertices;
	delete[] indices;

	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	float uvStep = 1.0f / vertexCount;

	//FIX UVs
	for (int i = 0; i < mLineList.size(); i++) {
		XMStoreFloat3(&(vertices[i * 2].position), mLineList[i].start);
		vertices[i * 2].texture = XMFLOAT2(uvStep * (float)i, uvStep * (float)i);

		XMStoreFloat3(&(vertices[(i * 2) + 1].position), mLineList[i].end);
		vertices[(i * 2) + 1].texture = XMFLOAT2(uvStep*(float)i, uvStep * (float)i);
	}
	for (int i = 0; i < mLineList.size(); i++) {
		indices[i*2] = i * 2;
		indices[(i * 2)+1] = (i * 2) + 1;
	}

	initBuffers(device);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//  Disable GPU access to the vertex buffer data.
	deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//  Update the vertex buffer here.
	memcpy(mappedResource.pData, vertices, sizeof(VertexType) * vertexCount);
	//  Reenable GPU access to the vertex buffer data.
	deviceContext->Unmap(vertexBuffer, 0);
}

void LineMesh::sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(top);
}

// Generate buffers
void LineMesh::initBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Set up the description of the dyanmic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
}