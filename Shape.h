#ifndef SHAPE_H
#define SHAPE_H

#include <FL/gl.h>
#include <FL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#define PI glm::pi<float>()

enum OBJ_TYPE {
	SHAPE_CUBE = 0,
	SHAPE_CYLINDER = 1,
	SHAPE_CONE = 2,
	SHAPE_SPHERE = 3,
	SHAPE_SPECIAL1 = 4,
	SHAPE_SPECIAL2 = 5,
	SHAPE_SPECIAL3 = 6,
	SHAPE_MESH = 7,
	SHAPE_SCENE = 8
};

class Shape {
public:
	static int m_segmentsX;
	static int m_segmentsY;

	Shape() {
		setSegments(10, 10);
	};
	~Shape() {};

	void setSegments(int segX, int segY) {
		m_segmentsX = segX;
		m_segmentsY = segY;
	}

	virtual OBJ_TYPE getType() = 0;
	virtual void draw() {};
	virtual void drawNormal() {};

	struct vertex {
        // position in 3D space
        float x, y, z;
        // I believe this is used to determine if a vertex can be removed.
        // Search for polygon decimation or vertex removal for more ideas.
        // source: graphics.standford.edu/software/vrip/plyusage.html
        float confidence;
        // I believe this has to do with lighting and shading, and this value
        // determines the shading coefficent used. This can be useful for when
        // we color the model.
        // source: www.okino.com/conv/imp_ply.htm
        float intensity;
        // Color values
        float r, g, b;
        // surface normal values
        float nx, ny, nz;
        // texture coordinates
        float u, v, w;
    };

protected:
	void normalizeNormal(float x, float y, float z) {
		normalizeNormal(glm::vec3(x, y, z));
	};

	void normalizeNormal(glm::vec3 v) {
		glm::vec3 tmpV = glm::normalize(v);
		glNormal3f(tmpV.x, tmpV.y, tmpV.z);
	};
};

#endif