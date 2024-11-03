#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere() { calculate(); };
	~Sphere() {};

	OBJ_TYPE getType() {
		return SHAPE_SPHERE;
	}

	void draw();

	void drawNormal();

	void calculate();

	void drawTriangleMeshFromFaces();

	void drawNormalsFromFaces();

	double intersect(glm::vec3 eyePosition, glm::vec3 rayv, glm::mat4 viewMatrix);
	
	glm::vec3 computeNormal(glm::vec3 isectPoint);
private:
};

#endif