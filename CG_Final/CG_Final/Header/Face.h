#pragma once

#include "Cost.h"
class Face 
{
public:
	int _index;
	Vec3<double> _normal;
	vector<Vertex*> _vertices;
	
	vector<Edge*> _nbEdges;
	int _edgeIdx[3];

public:
	Face(int index, Vertex* v0, Vertex* v1, Vertex* v2) {
		_index = index;
		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
	}
public:
	void ClearNeighbor() {
		_nbEdges.clear();
	}
	int GetVIndex(Vertex* v) {
		return GetIndex(_vertices, v);
	}
	bool HasVertex(Vertex* v) {
		return IsContain(_vertices, v);
	}
	Vertex* GetOtherVertex(Edge* edge) {
		for (auto v : _vertices) {
			if (edge->_vertices[0] != v && edge->_vertices[1] != v) {
				return v;
			}
		}
		return nullptr;
	}
};

