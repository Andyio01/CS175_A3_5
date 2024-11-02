#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

const float EPSILON = 1e-20;
#define IN_RANGE(a,b)   (((a>(b-EPSILON))&&(a<(b+EPSILON)))?1:0)

#define DEFAULT_FOCUS_LENGTH 1.0f
#define NEAR_PLANE 0.01f
#define FAR_PLANE 20.0f
#define VIEW_ANGLE 60.0f
#define PI glm::pi<float>()


class Camera {
public:
	float rotU, rotV, rotW;  //values used by the callback in main.cpp

	Camera();
	~Camera();

	void reset();
	void orientLookAt(glm::vec3 eyePoint, glm::vec3 focusPoint, glm::vec3 upVec);
	void orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec);
	void setViewAngle(float _viewAngle); //24
	void setNearPlane(float _nearPlane); //23
	void setFarPlane(float _farPlane); //22
	void setScreenSize(int _screenWidth, int _screenHeight); //21
	void setRotUVW(float u, float v, float w);  //called by main.cpp as a part of the slider callback

	glm::mat4 getUnhingeMatrix(); //21
	glm::mat4 getProjectionMatrix(); //20
	glm::mat4 getScaleMatrix(); //19
	glm::mat4 getInverseScaleMatrix(); //18
	glm::mat4 getModelViewMatrix(); //17
	glm::mat4 getInverseModelViewMatrix(); //16

	void rotateV(float degree); //15
	void rotateU(float degree); //14
	void rotateW(float degree); //13
	void rotate(glm::vec3 point, glm::vec3 axis, float degree); //12

	void translate(glm::vec3 v); //11

	glm::vec3 getEyePoint(); //10
	glm::vec3 getLookVector(); //9
	glm::vec3 getUpVector(); //8
	float getViewAngle(); //7
	float getNearPlane(); //6
	float getFarPlane(); //5
	int getScreenWidth(); //4
	int getScreenHeight(); //3

	float getFilmPlanDepth(); //2
	float getScreenWidthRatio(); //1

	glm::vec3 eyePoint;
    glm::vec3 u, v, w;  // Camera coordinate axes
    glm::vec3 lookVector;
    glm::vec3 upVector;

	float viewAngle, filmPlanDepth;
	float nearPlane, farPlane;
	int screenWidth, screenHeight;
	float screenWidthRatio;
};
#endif

