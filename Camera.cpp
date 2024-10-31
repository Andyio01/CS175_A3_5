#include "Camera.h"

Camera::Camera() {
	reset();
}

Camera::~Camera() {
}

void Camera::reset() {
	orientLookAt(glm::vec3(0.0f, 0.0f, DEFAULT_FOCUS_LENGTH), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	setViewAngle(VIEW_ANGLE);
	setNearPlane(NEAR_PLANE);
	setFarPlane(FAR_PLANE);
	screenWidth = screenHeight = 200;
	screenWidthRatio = 1.0f;
	rotU = rotV = rotW = 0;
}

//called by main.cpp as a part of the slider callback for controlling rotation
// the reason for computing the diff is to make sure that we are only incrementally rotating the camera
void Camera::setRotUVW(float u, float v, float w) {
	float diffU = u - rotU;
	float diffV = v - rotV;
	float diffW = w - rotW;
	rotateU(diffU);
	rotateV(diffV);
	rotateW(diffW);
	rotU = u;
	rotV = v;
	rotW = w;
}

// point + vector = new point
// n * vector = change length
void Camera::orientLookAt(glm::vec3 eyePoint, glm::vec3 lookatPoint, glm::vec3 upVec) {
	this->eyePoint = eyePoint;
    this->w = glm::normalize(eyePoint - lookatPoint);   // Negative look direction point - point 
    this->u = glm::normalize(glm::cross(upVec, w));    // Right direction vector
    this->v = glm::cross(w, u);                        // Up direction vector
    this->lookVector = glm::normalize(lookatPoint - eyePoint); // Update look vector
    this->upVector = v; // Update up vector
}


void Camera::orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec) {
	this->eyePoint = eyePoint;
    this->w = -glm::normalize(lookVec); // Negative look direction
    this->u = glm::normalize(glm::cross(upVec, w)); // Right direction vector
    this->v = glm::cross(w, u); // Up direction vector
    this->lookVector = glm::normalize(lookVec); // Update look vector
    this->upVector = v; // Update up vector
}

glm::mat4 Camera::getScaleMatrix() {
	float aspectRatio = 1.0f; // Could get better para later
	float heightAngleRadians = glm::radians(viewAngle);
    float tanHalfHeightAngle = tan(heightAngleRadians / 2.0f);
    float s_y = 1.0f / tanHalfHeightAngle;
    float s_x = s_y / aspectRatio;

    glm::mat4 scaleMat4(1.0f);
    scaleMat4[0][0] = s_x;
    scaleMat4[1][1] = s_y;

    return scaleMat4;
}

glm::mat4 Camera::getInverseScaleMatrix() {
	float aspectRatio = 1.0f; // Could get better para later
	float heightAngleRadians = glm::radians(viewAngle);
    float tanHalfHeightAngle = tan(heightAngleRadians / 2.0f);
    float inv_s_y = tanHalfHeightAngle;
    float inv_s_x = inv_s_y * aspectRatio;

    glm::mat4 invScaleMat4(1.0f);
    invScaleMat4[0][0] = inv_s_x;
    invScaleMat4[1][1] = inv_s_y;

    return invScaleMat4;
}

glm::mat4 Camera::getUnhingeMatrix() {
	float n = nearPlane;
    float f = farPlane;

    glm::mat4 unhingeMat4(0.0f);
    unhingeMat4[0][0] = 1.0f;
    unhingeMat4[1][1] = 1.0f;
    unhingeMat4[2][2] = -(f + n) / (f - n);
    unhingeMat4[2][3] = -(2.0f * f * n) / (f - n);
    unhingeMat4[3][2] = -1.0f;

    return unhingeMat4;
}


glm::mat4 Camera::getProjectionMatrix() {
	float n = nearPlane;
    float f = farPlane;
    float aspect = screenWidthRatio;
    float theta = glm::radians(viewAngle) / 2.0f;
    float d = 1.0f / tan(theta); // cotangent of half the FOV

    glm::mat4 projMat4(0.0f);

    projMat4[0][0] = d / aspect;
    projMat4[1][1] = d;
    projMat4[2][2] = -(f + n) / (f - n);
    projMat4[2][3] = -1.0f;
    projMat4[3][2] = -(2 * f * n) / (f - n);

    return projMat4;
}




void Camera::setViewAngle (float _viewAngle) { //24
	this->viewAngle = _viewAngle;
    //update film plane depth if needed
    this->filmPlanDepth = nearPlane * tan(glm::radians(viewAngle) / 2.0f);
}

void Camera::setNearPlane (float _nearPlane) { //23
	this->nearPlane = _nearPlane;
}

void Camera::setFarPlane (float _farPlane) { //22
	this->farPlane = _farPlane;
}

void Camera::setScreenSize (int _screenWidth, int _screenHeight) { //21
	this->screenWidth = _screenWidth;
    this->screenHeight = _screenHeight;
    this->screenWidthRatio = (float)_screenWidth / (float)_screenHeight;
}

// for moving all points in world coordinate to object coordinate
//R[column][row] in glm
glm::mat4 Camera::getModelViewMatrix() { //17 this is 

	glm::mat4 R(1.0f);

    R[0][0] = u.x; R[1][0] = u.y; R[2][0] = u.z; R[3][0] = 0.0f;
    R[0][1] = v.x; R[1][1] = v.y; R[2][1] = v.z; R[3][1] = 0.0f;
    R[0][2] = w.x; R[1][2] = w.y; R[2][2] = w.z; R[3][2] = 0.0f;
    R[0][3] = 0.0f; R[1][3] = 0.0f; R[2][3] = 0.0f; R[3][3] = 1.0f;
	//	| see page 46
	//	|
	//	|
	//	|
    glm::mat4 T = glm::translate(glm::mat4(1.0f), -eyePoint);
	//	| see page 45
    glm::mat4 modelViewMat4 = R * T;
	//  R^-1 * (T^-1 * v)
	// the freaking order matters
    return modelViewMat4;
}

glm::mat4 Camera::getInverseModelViewMatrix() { //16
//given uvw and eyepoint
// this is for moving camera right?
//rotate FITST and then translate
	glm::mat4 R(1.0f);

    R[0][0] = u.x; R[0][1] = u.y; R[0][2] = u.z; R[0][3] = 0.0f;
    R[1][0] = v.x; R[1][1] = v.y; R[1][2] = v.z; R[1][3] = 0.0f;
    R[2][0] = w.x; R[2][1] = w.y; R[2][2] = w.z; R[2][3] = 0.0f;
    R[3][0] = 0.0f; R[3][1] = 0.0f; R[3][2] = 0.0f; R[3][3] = 1.0f;
	//	| see page 41
	//	|
	//	|
	//	|
    glm::mat4 T = glm::translate(glm::mat4(1.0f), eyePoint);
	//	| see page 42
    glm::mat4 invModelViewMat4 = T * R;  // rotate first and then t

    return invModelViewMat4;
}

void Camera::rotateV(float degrees) { //15
	float radians = glm::radians(degrees);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, v);
    u = glm::vec3(rotationMatrix * glm::vec4(u, 0.0f));
    w = glm::vec3(rotationMatrix * glm::vec4(w, 0.0f));
    lookVector = -w;
    upVector = v;
}

void Camera::rotateU(float degrees) { //14
	float radians = glm::radians(degrees);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, u);
    v = glm::vec3(rotationMatrix * glm::vec4(v, 0.0f));
    w = glm::vec3(rotationMatrix * glm::vec4(w, 0.0f));
    lookVector = -w;
    upVector = v;
}

void Camera::rotateW(float degrees) {  //13
	float radians = glm::radians(degrees);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, w);
    u = glm::vec3(rotationMatrix * glm::vec4(u, 0.0f));
    v = glm::vec3(rotationMatrix * glm::vec4(v, 0.0f));
    upVector = v;
}

void Camera::translate(glm::vec3 v) { //11
	eyePoint += v;
}

void Camera::rotate(glm::vec3 point, glm::vec3 axis, float degrees) { //12
	float radians = glm::radians(degrees);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, axis);

    // Rotate eyePoint around the point
    glm::vec3 eyeRel = eyePoint - point; //make eyepoint object coordinate
    eyeRel = glm::vec3(rotationMatrix * glm::vec4(eyeRel, 1.0f)); //4d, 1 point can translate
    eyePoint = eyeRel + point; //move back to world coordinate
	// not only eyepoint
    // Rotate the camera axes 0 means directions cant translate
    u = glm::vec3(rotationMatrix * glm::vec4(u, 0.0f));
    v = glm::vec3(rotationMatrix * glm::vec4(v, 0.0f));
    w = glm::vec3(rotationMatrix * glm::vec4(w, 0.0f));
	//? need fix
    lookVector = -w;
    upVector = v;
}


glm::vec3 Camera::getEyePoint() { //10
	return eyePoint;
}

glm::vec3 Camera::getLookVector() { //9
	return lookVector;
}

glm::vec3 Camera::getUpVector() { //8
	return upVector;
}

float Camera::getViewAngle() { //7
	return viewAngle;
}

float Camera::getNearPlane() { //6
	return nearPlane;
}

float Camera::getFarPlane() { //5
	return farPlane;
}

int Camera::getScreenWidth() { //4
	return screenWidth;
}

int Camera::getScreenHeight() { //3
	return screenHeight;
}


float Camera::getFilmPlanDepth() { //2
    return filmPlanDepth;
}

float Camera::getScreenWidthRatio() { //1
	return screenWidthRatio;
}
