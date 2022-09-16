#pragma once
#include "BaseMesh.h"
#include <vector>

class LineSegment {
public:
	LineSegment() {
		start = XMVectorSet(0, 0, 0, 1); end = XMVectorSet(0, 1, 0, 1);
	}
	LineSegment(XMVECTOR s, XMVECTOR e) {
		start = s; end = e;
	}
	
	XMVECTOR start;
	XMVECTOR end;
};

class LineMesh :
	public BaseMesh
{
public:
	
	LineMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~LineMesh();

	void	sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	void	BuildLine(ID3D11DeviceContext* deviceContext, ID3D11Device* device);

	inline const int		GetLineCount() { return mLineList.size(); }
	inline const XMMATRIX	GetTransform() { return mTransform; }

	//Line manipulation
	void	AddLine(XMVECTOR start, XMVECTOR end) { mLineList.push_back(LineSegment(start, end)); }
	void	AddSegment() { mLineList.push_back(LineSegment()); }
	void	Clear() { mLineList.clear(); }
private:
	VertexType*			vertices;
	unsigned long*		indices;
	std::vector<LineSegment>	mLineList;
	XMMATRIX mTransform;
	
	void	initBuffers(ID3D11Device* device);
};

