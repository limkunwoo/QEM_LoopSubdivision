#pragma warning(disable:4996)
#pragma once
#include "..\Header\Face.h"
#include "GL\glut.h"
#include <fstream>
#include <algorithm>
#include <queue>
#include <sstream>
#include <set>

class Mesh
{
public:
	vector<Vertex*> _vertices;
	vector<Face*> _faces;
	vector<Edge*> _edges;
	multiset<Cost> _costs;
public:
	Mesh() {}
	Mesh(const char* file) {
		LoadObj(file);
	}
public:
	void LoadObj(const char* file);
	void LoadPly(const char* file);
	void MoveToCenter(Vec3<double> minBound, Vec3<double> maxBound, double scale);
	void DrawPoint(void);
	void DrawFace(void);
	void DrawEdge(void);
	void ComputeNormal(void);
	void ComputeNeighbor(void);
	void ComputeQEM(void);
	void Clear();
	void CombineEdge(Edge* e0, Edge* e1);
	void DeleteEdge(Edge* e);
	void DeleteFace(Face* f);
	void DeleteVertex(Vertex* v);
	Cost* GetCost(Edge* e);
public:
	void LoopSubdivision(int n);
	void Simplification(double n);
	void Rebuild();
};

