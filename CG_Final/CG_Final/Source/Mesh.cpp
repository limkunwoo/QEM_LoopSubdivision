#include "..\Header\Mesh.h"

void Mesh::LoadObj(const char* file)
{
	Clear();
	FILE* fp;
	int id;
	int index[3];
	char buffer[256] = { 0 };
	Vec3<double> pos;
	Vec3<double> minBound, maxBound;
	minBound.Set(DBL_MAX);
	maxBound.Set(DBL_MIN);

	id = 0;
	fopen_s(&fp, file, "r");
	while (fscanf(fp, "%s %lf %lf %lf", &buffer, &pos[0], &pos[1], &pos[2]) != EOF) {
		if (buffer[0] == 'v' && buffer[1] == NULL) {
			if (minBound[0] > pos[0])	minBound[0] = pos[0];
			if (minBound[1] > pos[1])	minBound[1] = pos[1];
			if (minBound[2] > pos[2])	minBound[2] = pos[2];
			if (maxBound[0] < pos[0])	maxBound[0] = pos[0];
			if (maxBound[1] < pos[1])	maxBound[1] = pos[1];
			if (maxBound[2] < pos[2])	maxBound[2] = pos[2];
			_vertices.push_back(new Vertex(id++, pos));
		}
	}
	

	id = 0;
	fseek(fp, 0, SEEK_SET);
	while (fscanf(fp, "%s %d %d %d", &buffer, &index[0], &index[1], &index[2]) != EOF) {
		if (buffer[0] == 'f' && buffer[1] == NULL) {
			_faces.push_back(new Face(id++, _vertices[index[0] - 1], _vertices[index[1] - 1], _vertices[index[2] - 1]));
		}
	}
	
	fclose(fp);

	MoveToCenter(minBound, maxBound, 5.0f);
	ComputeNeighbor();
	ComputeNormal();
	ComputeQEM();
}

void Mesh::LoadPly(const char* file)
{
	Clear();
	string filename(file);
	vector<string> lines;
	string line;

	ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		cerr << "Could not open file : '" << filename << "'" << endl;
		return;
	}
	while (getline(inputFile, line))
	{
		lines.push_back(line);
	}
	int numVertex = 0;
	int numFace = 0;
	Vec3<double> minBound, maxBound;
	minBound.Set(DBL_MAX);
	maxBound.Set(DBL_MIN);
	//tmp
	int numProperty = 12;
	
	int lineSequence = -1;
	for (lineSequence = 0; lineSequence < lines.size(); ++lineSequence)
	{
		istringstream iss(lines[lineSequence]);
		string buffer;
		vector<string> splited;

		while (getline(iss, buffer, ' '))
		{
			splited.push_back(buffer);
		}
		if (splited[0] == "element")
		{
			if (splited[1] == "vertex")
				numVertex = stoi(splited[2]);
			if (splited[1] == "face")
				numFace = stoi(splited[2]);
		}
		if (splited[0] == "comment")
		{
			continue;
		}
		if (splited[0] == "end_header")
		{
			break;
		}
	}
	int vId = 0;
	for (; numVertex; --numVertex)
	{
		++lineSequence;
		istringstream iss(lines[lineSequence]);
		string buffer;
		vector<string> splited;

		while (getline(iss, buffer, ' '))
		{
			splited.push_back(buffer);
		}

		Vec3<double> pos;
		pos[0] = stod(splited[0]);
		pos[1] = stod(splited[1]);
		pos[2] = stod(splited[2]);

		if (minBound[0] > pos[0])	minBound[0] = pos[0];
		if (minBound[1] > pos[1])	minBound[1] = pos[1];
		if (minBound[2] > pos[2])	minBound[2] = pos[2];
		if (maxBound[0] < pos[0])	maxBound[0] = pos[0];
		if (maxBound[1] < pos[1])	maxBound[1] = pos[1];
		if (maxBound[2] < pos[2])	maxBound[2] = pos[2];

		Vec3<double> color;
		color[0] = stod(splited[3]);
		color[1] = stod(splited[4]);
		color[2] = stod(splited[5]);

		Vec3<double> normal;
		normal[0] = stod(splited[7]);
		normal[1] = stod(splited[8]);
		normal[2] = stod(splited[9]);

		Vertex* vertex = new Vertex(vId++, pos);
		vertex->_color = color;
		vertex->_normal = normal;
		vertex->_u = stod(splited[10]);
		vertex->_v = stod(splited[11]);
		_vertices.push_back(vertex);
	}
	int fid = 0;
	for (; numFace; --numFace)
	{
		++lineSequence;

		istringstream iss(lines[lineSequence]);
		string buffer;
		vector<string> splited;

		while (getline(iss, buffer, ' '))
		{
			splited.push_back(buffer);
		}

		if (splited[0] == "3")
		{
			int idx1 = stoi(splited[1]);
			int idx2 = stoi(splited[2]);
			int idx3 = stoi(splited[3]);

			_faces.push_back(new Face(fid++, _vertices[idx1], _vertices[idx2], _vertices[idx3]));
		}
	}
	inputFile.close();

	MoveToCenter(minBound, maxBound, 5.0f);
	ComputeNeighbor();
	ComputeNormal();
	ComputeQEM();
}

void Mesh::MoveToCenter(Vec3<double> minBound, Vec3<double> maxBound, double scale)
{
	double longestLength = fmaxf(fmaxf(fabs(maxBound.x() - minBound.x()), fabs(maxBound.y() - minBound.y())), fabs(maxBound.z() - minBound.z()));
	auto center = (maxBound + minBound) / 2.0f;
	Vec3<double> origin(0.0, 0.0, 0.0);

	for (auto v : _vertices)
	{
		auto pos = v->_pos;
		auto vecVertexFromCenter = pos - center;
		vecVertexFromCenter /= longestLength;
		vecVertexFromCenter *= scale;
		pos = origin;
		pos += vecVertexFromCenter;
		v->_pos = pos;
	}
}

void Mesh::DrawPoint(void)
{
	glDisable(GL_LIGHTING);
	glPointSize(5.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	for (auto v : _vertices)
	{
		glNormal3f(1.f, 1.f, 0.f);
		auto pos = v->_pos;
		glVertex3f(pos.x(), pos.y(), pos.z());
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void Mesh::DrawFace(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	for (auto f : _faces)
	{
		glBegin(GL_POLYGON);
		glNormal3f(f->_normal.x(), f->_normal.y(), f->_normal.z());
		for (auto v : f->_vertices)
		{
			glVertex3f(v->x(), v->y(), v->z());
		}
		glEnd();
	}
	glPointSize(1.0f);
}

void Mesh::DrawEdge(void)
{
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(3.0f);
	for (auto e : _edges)
	{
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glNormal3f(1.f, 1.f, 0.f);
		glVertex3f(e->_vertices[0]->x(), e->_vertices[0]->y(), e->_vertices[0]->z());
		glVertex3f(e->_vertices[1]->x(), e->_vertices[1]->y(), e->_vertices[1]->z());
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void Mesh::ComputeNormal(void)
{
	for (auto f : _faces)
	{
		auto v01 = f->_vertices[1]->_pos - f->_vertices[0]->_pos;
		auto v02 = f->_vertices[2]->_pos - f->_vertices[0]->_pos;

		f->_normal = v01.Cross(v02);
		f->_normal.Normalize();
	}
}

void Mesh::ComputeNeighbor(void)
{
	for (auto v : _vertices) {
		v->ClearNeighbor();
	}
	for (auto f : _faces) {
		f->ClearNeighbor();
	}
	for (auto e : _edges) {
		delete e;
	}
	_edges.clear();

	for (auto f : _faces) {
		f->_vertices[0]->_nbFaces.push_back(f);
		f->_vertices[1]->_nbFaces.push_back(f);
		f->_vertices[2]->_nbFaces.push_back(f);
	}
	for (auto v : _vertices) {
		for (auto f : v->_nbFaces) {
			int index = GetIndex(f->_vertices, v);
			Vertex* otherV1 = f->_vertices[(index + 1) % 3];
			Vertex* otherV2 = f->_vertices[(index + 2) % 3];

			if (IsContain(v->_nbVertices, otherV1) == false)
				v->_nbVertices.push_back(otherV1);
			if (IsContain(v->_nbVertices, otherV2) == false)
				v->_nbVertices.push_back(otherV2);
		}
	}
	int id = 0;
	for (auto v : _vertices) {
		for (auto nv : v->_nbVertices) {
			if (!nv->_edgeComputed) {
				_edges.push_back(new Edge(id++, v, nv));
			}
		}
		v->_edgeComputed = true;
	}
	for (auto v : _vertices) {
		v->_edgeComputed = false;
	}
	for (auto e : _edges) {
		auto v = e->_vertices[0];
		for (auto nf : v->_nbFaces) {
			if (IsContain(nf->_vertices, e->_vertices[0]) && IsContain(nf->_vertices, e->_vertices[1])) {
				if (IsContain(nf->_nbEdges, e) == false) {
					nf->_nbEdges.push_back(e);
				}
				if (IsContain(e->_nbFaces, nf) == false) {
					int idx1 = GetIndex(nf->_vertices, e->_vertices[0]);
					int idx2 = GetIndex(nf->_vertices, e->_vertices[1]);
					int otherIdx = 3 - (idx1 + idx2);

					e->_nbFaces.push_back(nf);
					e->_others.push_back(nf->_vertices[otherIdx]);
				}
			}
		}
	}
	for (auto e : _edges) {
		for (auto f : e->_nbFaces) {
			int idx1 = GetIndex(f->_vertices, e->_vertices[0]);
			int idx2 = GetIndex(f->_vertices, e->_vertices[1]);
			int otherIdx = 3 - (idx1 + idx2);

			switch (otherIdx)
			{
			case 0:
				f->_edgeIdx[1] = GetIndex(f->_nbEdges, e);
				break;
			case 1:
				f->_edgeIdx[2] = GetIndex(f->_nbEdges, e);
				break;
			case 2:
				f->_edgeIdx[0] = GetIndex(f->_nbEdges, e);
				break;
			}
		}
	}
	printf("Number of Vertex : %d\n", _vertices.size());
	printf("Number of face : %d\n", _faces.size());
	printf("Number of edge : %d\n", _edges.size());
}

void Mesh::ComputeQEM(void)
{
	_costs = multiset<Cost>();
	Matrix<double> Kj(4, 4);
	Matrix<double> p(4, 1);
	for (auto v : _vertices) {
		Matrix<double> QEM(4, 4);
		for (auto f : v->_nbFaces) {
			f->_normal.Normalize();
			p[0][0] = f->_normal.x();				//a
			p[1][0] = f->_normal.y();				//b
			p[2][0] = f->_normal.z();				//c
			p[3][0] = -(f->_normal.Dot(v->_pos));	//d
			Kj = p * p.GetTransPose();
			QEM = QEM + Kj;
		}
		v->_qem = QEM;
	}
	for (int i = 0; i < _edges.size(); i++) {
		Matrix<double> newQ = _edges[i]->_vertices[0]->_qem + _edges[i]->_vertices[1]->_qem;
		Matrix<double> tmp(newQ);
		tmp[3][0] = 0;
		tmp[3][1] = 0;
		tmp[3][2] = 0;
		tmp[3][3] = 1;

		Matrix<double> newV;
		Matrix<double> b(4, 1);
		b[0][0] = 0;
		b[1][0] = 0;
		b[2][0] = 0;
		b[3][0] = 1;
		if (Matrix<double>::Solution(tmp, newV, b) == false) {
			Vec3<double> newPos = (_edges[i]->_vertices[0]->_pos + _edges[i]->_vertices[1]->_pos) / 2;
			newV[0][0] = newPos.x();
			newV[1][0] = newPos.y();
			newV[2][0] = newPos.z();
			newV[3][0] = 1.0;
		}
		double cost = ((newV.GetTransPose() * newQ) * newV)[0][0];
		Cost* error = new Cost(Vec3<double>(newV[0][0], newV[1][0], newV[2][0]), _edges[i], cost);
		_edges[i]->_cost = error;
		_costs.insert(*error);
	}
}

void Mesh::Clear()
{
	for (auto v : _vertices)
		delete v;
	for (auto f : _faces)
		delete f;
	for (auto e : _edges) {
		delete e;
	}
	_vertices.clear();
	_faces.clear();
	_edges.clear();
}

void Mesh::CombineEdge(Edge* e0, Edge* e1)
{
	for (auto f : e1->_nbFaces) {
		if (!IsContain(e0->_nbFaces, f)) {
			e0->_nbFaces.push_back(f);
			f->_nbEdges.push_back(e0);
		}
	}
}

void Mesh::DeleteEdge(Edge* e)
{
	for (auto v : e->_vertices) {
		DeleteElement(v->_nbEdges, e);
	}
	for (auto f : e->_nbFaces) {
		DeleteElement(f->_nbEdges, e);
	}
	multiset<Cost>::iterator setIt;
	setIt = _costs.find(*e->_cost);
	_costs.erase(setIt);
	vector<Edge*>::iterator it;
	it = BinarySearch(_edges,_edges.begin(), _edges.end(), e);
	if (it != _edges.end())
		_edges.erase(it);
	delete e;
}

void Mesh::DeleteFace(Face* f)
{
	for (auto v : f->_vertices) {
		DeleteElement(v->_nbFaces, f);
	}
	for (auto e : f->_nbEdges) {
		DeleteElement(e->_nbFaces, f);
	}
	vector<Face*>::iterator it;
	it = BinarySearch(_faces, _faces.begin(), _faces.end(), f);
	if (it != _faces.end())
		_faces.erase(it);
	delete f;
}

void Mesh::DeleteVertex(Vertex* v)
{
	for (auto nv : v->_nbVertices) {
		if(IsContain(nv->_nbVertices, v))
			DeleteElement(nv->_nbVertices, v);
	}
	for (auto nf : v->_nbFaces) {
		if (IsContain(nf->_vertices, v))
			DeleteElement(nf->_vertices, v);
	}
	for (auto ne : v->_nbEdges) {
		if (IsContain(ne->_vertices, v))
			DeleteElement(ne->_vertices, v);
	}
	vector<Vertex*>::iterator it;
	it = BinarySearch(_vertices, _vertices.begin(), _vertices.end(), v);
	if (it != _vertices.end())
		_vertices.erase(it);
	delete v;
}

Cost* Mesh::GetCost(Edge* e)
{
	Matrix<double> newQ = e->_vertices[0]->_qem + e->_vertices[1]->_qem;
	Matrix<double>tmp(newQ);
	tmp[3][0] = 0;
	tmp[3][1] = 0;
	tmp[3][2] = 0;
	tmp[3][3] = 1;

	Matrix<double> newV;
	Matrix<double> b(4, 1);
	b[0][0] = 0;
	b[1][0] = 0;
	b[2][0] = 0;
	b[3][0] = 1;
	if (Matrix<double>::Solution(tmp, newV, b) == false) {
		Vec3<double> newPos = (e->_vertices[0]->_pos + e->_vertices[1]->_pos) / 2;
		newV[0][0] = newPos.x();
		newV[1][0] = newPos.y();
		newV[2][0] = newPos.z();
		newV[3][0] = 1.0;
	}
	double cost = ((newV.GetTransPose() * newQ) * newV)[0][0];
	Cost* error = new Cost(Vec3<double>(newV[0][0], newV[1][0], newV[2][0]), e, cost);
	e->_cost = error;
	return error;
}

void Mesh::LoopSubdivision(int n)
{
	for (int i = 0; i < n; i++) {
		vector<Vertex*> odds;
		//create odd Vertices
		int id = 0;
		for (auto e : _edges) {
			Vertex* odd;
			if (e->_others.size() != 2) {
				odd = new Vertex(_vertices.size() + id++, (e->_vertices[0]->_pos + e->_vertices[1]->_pos) / 2.0f);
			}
			else {
				odd = new Vertex(_vertices.size() + id++, (e->_vertices[0]->_pos + e->_vertices[1]->_pos) * 3.0f / 8.0f + (e->_others[0]->_pos + e->_others[1]->_pos) * 1.0f / 8.0f);
			}
			odds.push_back(odd);
			e->_odd = odd;
		}
		//even Vertices update
		for (auto v : _vertices)
		{
			int n = v->_nbVertices.size();

			float B;
			B = 3.0f / 8.0f + powf(3.0f / 8.0f + 1.0f / 3.0f * cosf(2 * 3.141592 / (float)n), 2.0f);

			Vec3<double> sumOfNb(0.0f, 0.0f, 0.0f);
			for (auto nv : v->_nbVertices) {
				sumOfNb += nv->_pos;
			}

			Vec3<double> newPos(0.0f, 0.0f, 0.0f);
			newPos = v->_pos * B + sumOfNb * ((1.0f - B) / (float)n);

			v->_pos = newPos;
		}
		//Add Odd Vertices
		for (auto v : odds) {
			_vertices.push_back(v);
		}
		odds.clear();

		//rebuild Mesh
		Rebuild();
	}
}

void Mesh::Simplification(double ratio)
{
	cout << endl << "Simplification..." << endl;
	ratio = max(0.0, min(ratio, 1.0));
	int loop = _faces.size() * (1 - ratio);
	for (int i = 0; i < loop; i++) {
		Cost cost = *_costs.begin();
		Edge* e = cost._edge;
		if (e->_nbFaces.size() != 2) {
			DeleteEdge(e);
			continue;
		}
		if (e->_vertices[0] == e->_vertices[1]) {
			DeleteEdge(e);
			continue;
		}
		bool isManifold = true;
		if (e == NULL) {
			_costs.erase(_costs.begin());
			continue;
		}
		for (auto f : e->_nbFaces) {
			if (f->_nbEdges.size() != 3)
				isManifold = false;
			for (auto e : f->_nbEdges) {
				if (e->_nbFaces.size() != 2)
					isManifold = false;
			}
		}
		if (!isManifold) {
			DeleteEdge(e);
			continue;
		}
		Vertex* v0 = e->_vertices[0];
		Vertex* v1 = e->_vertices[1];
		v0->_pos = (*_costs.begin())._newPos;
		v0->_qem = v0->_qem + v1->_qem;
		
		for (auto f : e->_nbFaces) {
			Edge* e0 = f->_nbEdges[(GetIndex(f->_nbEdges, e) + 1) % 3];
			Edge* e1 = f->_nbEdges[(GetIndex(f->_nbEdges, e) + 2) % 3];
			if (!IsContain(e0->_vertices, v0)) {
				Edge* tmp = e0;
				e0 = e1;
				e1 = tmp;
			}
			Face* fixF = e1->_nbFaces[(GetIndex(e1->_nbFaces, f) + 1) % 2];
			fixF->_nbEdges.push_back(e0);
			e0->_nbFaces.push_back(fixF);
			DeleteEdge(e1);
		}
		for (auto f : e->_nbFaces) {
			DeleteFace(f);
		}
		DeleteEdge(e);
		for (auto f : v1->_nbFaces) {
			f->_vertices[GetIndex(f->_vertices, v1)] = v0;
			v0->_nbFaces.push_back(f);
		}
		for (auto e : v1->_nbEdges) {
			int index = GetIndex(e->_vertices, v1);
			e->_vertices.push_back(v0);
			v0->_nbEdges.push_back(e);
			Vertex* otherV = e->_vertices[(index + 1) % 2];
			v0->_nbVertices.push_back(otherV);
			otherV->_nbVertices.push_back(v0);
		}
		DeleteVertex(v1);
		multiset<Cost>::iterator it;
		for (auto ne : v0->_nbEdges) {
			it = _costs.find(*ne->_cost);
			Cost* oldCost = (*it)._edge->_cost;
			Cost* newCost = GetCost(ne);
			(*it)._edge->_cost = newCost;
			_costs.erase(it);
			_costs.insert(*newCost);
			delete oldCost;
		}
	}
	for (int i = 0; i < _faces.size(); i++) {
		_faces[i]->_index = i;
	}
	for (int i = 0; i < _vertices.size(); i++) {
		_vertices[i]->_index = i;
	}
	for (int i = 0; i < _edges.size(); i++) {
		_edges[i]->_index = i;
	}
	ComputeNeighbor();
	ComputeQEM();
	ComputeNormal();
}

void Mesh::Rebuild()
{
	for (auto v : _vertices)
		v->ClearNeighbor();
	vector<Face*> newFaces;
	int id = 0;
	for (auto f : _faces) {
		newFaces.push_back(new Face(id++, f->_vertices[0], f->_nbEdges[f->_edgeIdx[0]]->_odd, f->_nbEdges[f->_edgeIdx[2]]->_odd));
		newFaces.push_back(new Face(id++, f->_nbEdges[f->_edgeIdx[0]]->_odd, f->_vertices[1], f->_nbEdges[f->_edgeIdx[1]]->_odd));
		newFaces.push_back(new Face(id++, f->_nbEdges[f->_edgeIdx[2]]->_odd, f->_nbEdges[f->_edgeIdx[1]]->_odd, f->_vertices[2]));
		newFaces.push_back(new Face(id++, f->_nbEdges[f->_edgeIdx[0]]->_odd, f->_nbEdges[f->_edgeIdx[1]]->_odd, f->_nbEdges[f->_edgeIdx[2]]->_odd));
	}
	for (int i = 0; i < _faces.size(); i++) {
		delete _faces[i];
	}
	_faces.clear();
	_faces = newFaces;
	ComputeNeighbor();
	ComputeNormal();
	ComputeQEM();
}
