#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {
public:
	Cylinder() { calculate(); };
	~Cylinder() {};

	OBJ_TYPE getType() {
		return SHAPE_CYLINDER;
	}
	
	std::vector<Vertex> vertices;
	std::vector<Face>   Faces;

	void draw();

	void drawNormal();

	void calculate();

	void drawTriangleMeshFromFaces();

	double intersect(glm::vec3 eyePosition, glm::vec3 rayv, glm::mat4 viewMatrix);

private:
};

#endif