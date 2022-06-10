#pragma once
#include "..\Header\Vertex.h"
class Cost;
class Edge
{
public:
	int _index;
	vector<Vertex*> _vertices;
	vector<Face*> _nbFaces;
	vector<Vertex*> _others;
	Vertex* _odd;
	Cost* _cost;
public:
	Edge(int index, Vertex* v1, Vertex* v2) {
		_index = index;
		_vertices.push_back(v1);
		_vertices.push_back(v2);
		v1->_nbEdges.push_back(this);
		v2->_nbEdges.push_back(this);
		_odd = NULL;
	}
	~Edge() {
		delete _cost;
	}
	void ClearNeighbor() {
		_nbFaces.clear();
		_others.clear();
	}
};