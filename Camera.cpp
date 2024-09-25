#include "Camera.h"

#include <iostream>
//this is for printing out the matrix 
#include <glm/gtx/string_cast.hpp>

using namespace std;

Camera::Camera() {
	reset();
}

Camera::~Camera() {
}

void Camera::reset() {
	// orientLookAt(eyePoint, lookatPoint, upVec) 
	//orientLookAt(glm::vec3(0.0f, 0.0f, DEFAULT_FOCUS_LENGTH), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

/*
 - Maintaining a matrix that implements the perspective transformation
 - Setting the cameras absolute position and orientation given an eye point, 
 look vector, and up vector
 
 - Setting the cameras view (height) angle and aspect ratio (aspect ratio 
 is determined through setting the screens width and height).

 - Translating the camera in world space

 - Rotating the camera about one of the axes in its own virtual coordinate syst.

 - Setting the near and far clipping planes
 
 - Having the ability to, at any point, spit out the current eye point, 
 look vector, up vector, view (height) angle, aspect ratio (screen width ratio), 
 and the perspective and model view matrices.

Asli's Question:

-- I understand that we are implementing the matrix that will be transformed 
so that we have coordinates of where the camera is moving. I understand that 
my question is elementary but, I'm not sure what our "starting point" or 
starting matrix is; or what an arbitrary position of the camera would be in this 
situation; is it the matrix of projection? and would it be an arbitrary distance
the z axis and 0,0 for x and y?

 */
 //slide 27
void Camera::orientLookAt(glm::vec3 eyePoint, glm::vec3 lookatPoint, glm::vec3 upVec) {
	// eyeVec3 = eyePoint;
	// lookVec3 = {(lookatPoint[0] - eyePoint[0]),  (lookatPoint[1] - eyePoint[1]), (lookatPoint[2] - eyePoint[2])};
	// lookVec3 = glm::normalize(lookVec3);
	// upVec3 = upVec;
	orientLookVec(eyePoint, glm::normalize(lookatPoint - eyePoint), upVec);
}

void Camera::orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec) { 
	eyeVec3  = eyePoint;
	lookVec3 = lookVec - eyePoint;
	upVec3 = upVec;
}

glm::mat4 Camera::getScaleMatrix() {
	float theta_w = glm::radians(getViewAngle());
	float theta_h = theta_w * getScreenWidthRatio();
	float far = getFarPlane();
	glm::mat4 scaleMat4 = {
		(1.0/(glm::tan(theta_w/2.0) * far)), 0,									 0, 		0,
 		0, 									(1.0/(glm::tan(theta_h/2.0) * far)), 0, 		0,
		0, 									 0, 								(1.0/far),  0,
		0,									 0,									 0, 		1
	};

	return scaleMat4;
}

glm::mat4 Camera::getInverseScaleMatrix() {
	glm::mat4 invScaleMat4 = glm::inverse(getScaleMatrix());
	return invScaleMat4;
}

glm::mat4 Camera::getUnhingeMatrix() {
	float c = -getNearPlane() / getFarPlane();
	glm::mat4 unhingeMat4 = {
		1, 0, 0, 			0,
		0, 1, 0,		    0,
		0, 0, (-1.0/(c + 1.0)),  -1,
		0, 0, (c/(c + 1.0)),    0
	};
	return unhingeMat4;
}


glm::mat4 Camera::getProjectionMatrix() {
	glm::mat4 projMat4(1.0);
	projMat4 = getUnhingeMatrix() * getScaleMatrix();
	return projMat4;
}

glm::mat4 Camera::getInverseModelViewMatrix() {
	glm::mat4 invModelViewMat4(1.0);
	invModelViewMat4 = glm::inverse(getModelViewMatrix());
	return invModelViewMat4;
}


void Camera::setViewAngle (float _viewAngle) {
	viewAngle = _viewAngle;
}

void Camera::setNearPlane (float _nearPlane) {
	nearPlane = _nearPlane;
}

void Camera::setFarPlane (float _farPlane) {
	farPlane = _farPlane;
}

void Camera::setScreenSize (int _screenWidth, int _screenHeight) {
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
}

glm::mat4 Camera::getModelViewMatrix() {
	glm::vec3 lookVec = -getLookVector();
	glm::vec3 upVec = getUpVector();
	glm::vec3 w = lookVec / glm::length(lookVec);
	glm::vec3 u = glm::cross(upVec, w) / glm::length(glm::cross(upVec, w));
	glm::vec3 v = glm::cross(w, u);

	//col major
	glm::mat4 rotation = {
		u[0], v[0], w[0], 0,
		u[1], v[1], w[1], 0,
		u[2], v[2], w[2], 0,
		0, 0, 0, 1
	};

	glm::vec3 eye = getEyePoint();

	glm::mat4 translation = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye[0], -eye[1], -eye[2], 1
	};

	glm::mat4 modelViewMat4(1.0);

	modelViewMat4 = rotation * translation;
	return modelViewMat4;
}

//Rotate about the "y" axis (v) -- Yaw
void Camera::rotateV(float degrees) {
	cerr << "in rotateV" << endl;
	float theta_r = glm::radians(degrees);
	float sin_r = glm::sin(theta_r);
	float cos_r = glm::cos(theta_r);

	glm::mat4 rotate_yaw = {
		cos_r, 	0, -sin_r, 	0,
		0, 		1, 0, 		0,
		sin_r, 	0, cos_r, 	0,
		0, 		0, 0, 		1
	};

	glm::vec4 look4 = {lookVec3[0], lookVec3[1],lookVec3[2], 0};
	
	look4 = look4 * rotate_yaw;
	lookVec3[0] = look4[0];
	lookVec3[1] = look4[1];
	lookVec3[2] = look4[2];
}


//Rotate about the "x" axis (u) -- Pitch
void Camera::rotateU(float degrees) {
	cerr << "in rotateU" << endl;
	float theta_r = glm::radians(degrees);
	float sin_r = glm::sin(theta_r);
	float cos_r = glm::cos(theta_r);

	glm::mat4 rotate_pitch = {
		1, 0, 		0, 		0,
		0, cos_r, 	sin_r, 0,
		0, -sin_r, 	cos_r, 	0,
		0, 0, 		0, 		1
	};
	
	glm::vec4 look4 = {lookVec3[0], lookVec3[1],lookVec3[2], 0};
	
	look4 = look4 * rotate_pitch;
	lookVec3[0] = look4[0];
	lookVec3[1] = look4[1];
	lookVec3[2] = look4[2];
	// lookVec3 = lookVec3 * r
}

//Rotate about the "z" axis (w) -- Roll/Spin
//We think this is good :)))))
void Camera::rotateW(float degrees) {
	cerr << "rotateW" << endl;
	float theta_r = glm::radians(degrees);
	float sin_r = glm::sin(theta_r);
	float cos_r = glm::cos(theta_r);

	glm::mat4 rotate_spin = {
		cos_r, 	sin_r, 	0, 0,
		-sin_r, cos_r, 	0, 0,
		0, 		0, 		1, 0,
		0, 		0, 		0, 1
	};

	glm::vec4 up4 = {upVec3[0], upVec3[1], upVec3[2], 0};
	
	up4 = up4 * rotate_spin;
	upVec3[0] = up4[0];
	upVec3[1] = up4[1];
	upVec3[2] = up4[2];
}

void Camera::translate(glm::vec3 v) {
	cerr << "translate" << endl;
}

void Camera::rotate(glm::vec3 point, glm::vec3 axis, float degrees) {
	cerr << "rotate given axis" << endl;
	// glm::vec3 lookVec = -getLookVector();
	// glm::vec3 upVec = getUpVector();
	// glm::vec3 w = lookVec / glm::length(lookVec);
	// glm::vec3 u = glm::cross(upVec, w) / glm::length(glm::cross(upVec, w));
	// glm::vec3 v = glm::cross(w, u);

	float theta_r = glm::radians(degrees);
	float sin_r = glm::sin(theta_r);
	float cos_r = glm::cos(theta_r);

	glm::mat4 rotate_yaw = {
		cos_r, 	0, -sin_r, 	0,
		0, 		1, 0, 		0,
		sin_r, 	0, cos_r, 	0,
		0, 		0, 0, 		1
	};

	glm::mat4 rotate_spin = {
		cos_r, 	sin_r, 	0, 0,
		-sin_r, cos_r, 	0, 0,
		0, 		0, 		1, 0,
		0, 		0, 		0, 1
	};

	//we got this wrong on the ws so im not sure
	glm::mat4 rotate_pitch = {
		1, 0, 		0, 		0,
		0, cos_r, 	sin_r, 0,
		0, -sin_r, 	cos_r, 	0,
		0, 0, 		0, 		1
	};

}


glm::vec3 Camera::getEyePoint() {
	return eyeVec3;
}

glm::vec3 Camera::getLookVector() {
	return lookVec3;
}

glm::vec3 Camera::getUpVector() {
	return upVec3;
}

float Camera::getViewAngle() {
	return viewAngle;
}

float Camera::getNearPlane() {
	return nearPlane;
}

float Camera::getFarPlane() {
	return farPlane;
}

int Camera::getScreenWidth() {
	return screenWidth;
}

int Camera::getScreenHeight() {
	return screenHeight;
}

float Camera::getScreenWidthRatio() {
	return screenWidthRatio;
}
