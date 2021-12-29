#pragma once

#include "Vec3.h"
#include "vectorExtension.h"
#include "Matrix.h"
class Face;
class Edge;

class Vertex
{
public:
	int _index;
	Vec3<double> _pos;
	bool _edgeComputed;

	vector<Vertex*> _nbVertices;
	vector<Edge*> _nbEdges;
	vector<Face*> _nbFaces;
	Matrix<double> _qem;

public:
	Vertex(int index, Vec3<double> pos) {
		_index = index;
		_pos = pos;
		_edgeComputed = false;
	}
public:
	void ClearNeighbor() {
		_nbVertices.clear();
		_nbEdges.clear();
		_nbFaces.clear();
	}
public:
	inline double x(void) { return _pos[0]; }
	inline double y(void) { return _pos[1]; }
	inline double z(void) { return _pos[2]; }
};