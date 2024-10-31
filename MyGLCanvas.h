#pragma once

#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H

#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.H>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <time.h>

#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneGraph.h"

#include "Camera.h"
#include "scene/SceneParser.h"

class MyGLCanvas : public Fl_Gl_Window {
public:
	glm::vec3 rotVec;
	glm::vec3 eyePosition;
	GLubyte* pixels = NULL;

	int isectOnly;
	int segmentsX, segmentsY;
	float scale;

	OBJ_TYPE objType;
	Cube* cube;
	Cylinder* cylinder;
	Cone* cone;
	Sphere* sphere;
	Shape* shape;

	Camera* camera;
	SceneParser* parser;
	SceneGraph* scene;


	MyGLCanvas(int x, int y, int w, int h, const char *l = 0);
	~MyGLCanvas();
	void renderShape(OBJ_TYPE type);
	void setSegments();
	void loadSceneFile(const char* filenamePath);
	void renderScene();
	void flatSceneData();
	void flatSceneDataRec(SceneNode* node, glm::mat4 curMat);

private:
	void setpixel(GLubyte* buf, int x, int y, int r, int g, int b);
	glm::vec3 getRayDirectionInWorld(glm::vec3 filmPointInWorld, glm::vec3 eyePositionInWorld, glm::mat4 viewMatrix);
	glm::vec3 getFilmPointWorld(int pixelX, int pixelY, int screenWidth, int screenHeight);
	void draw();
	void raycasting(glm::vec3 eyePosition, glm::vec3 lookVector, int i, int j);
	int handle(int);
	void resize(int x, int y, int w, int h);
	void updateCamera(int width, int height);

	int pixelWidth, pixelHeight;
};

#endif // !MYGLCANVAS_H