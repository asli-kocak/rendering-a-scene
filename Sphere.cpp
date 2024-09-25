#include "Sphere.h"
#include "Shape.h"
#define _CRT_SECURE_NO_WARNINGS
#define GL_SILENCE_DEPRECATION

#include <FL/gl.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

void Sphere::draw() {
    float radius = 0.5;
    int x_segments = Sphere::m_segmentsX;
    int y_segments = Sphere::m_segmentsY;
    float fi = 2 * M_PI / x_segments;
    float theta    = M_PI / y_segments;

    for (int i = 0; i < y_segments; i++) {
        for (int j = 0; j < x_segments; j++) {
            //top right
            float x = radius * sin(theta * i) * cos(fi * j);
            float z = radius * sin(theta * i) * sin(fi * j);
            float y = radius * cos(theta * i);
            //top left
            float x2 = radius * sin(theta * (i)) * cos(fi * (j + 1));
            float z2 = radius * sin(theta * i) * sin(fi * (j + 1));
            float y2 = radius * cos(theta * (i));
            //bottom right
            float x3 = radius * sin(theta * (i + 1)) * cos(fi * j);
            float z3 = radius * sin(theta * (i + 1)) * sin(fi * (j));;
            float y3 = radius * cos(theta * (i + 1));
            //bottom left
            float x4 = radius * sin(theta * (i + 1)) * cos(fi * (j + 1));
            float z4 = radius * sin(theta * (i + 1)) * sin(fi * (j + 1));;
            float y4 = radius * cos(theta * (i + 1));

            // B-A
            float xn1 = x2 - x;
            float yn1 = y2 - y;
            float zn1 = z2 - z;
            glm::vec3 BA = glm::vec3(xn1, yn1, zn1);

            //C-A
            float xn2 = x3 - x;
            float yn2 = y3 - y;
            float zn2 = z3 - z;
            glm::vec3 CA = glm::vec3(xn2, yn2, zn2);

            glBegin(GL_TRIANGLES);

            glm::vec3 normalized = glm::vec3(0);

            normalized = glm::normalize(glm::vec3(x3, y3, z3));
            glNormal3f(normalized[0], normalized[1], normalized[2]);
            glVertex3f(x3, y3, z3);

            normalized = glm::normalize(glm::vec3(x4, y4, z4));
            glNormal3f(normalized[0], normalized[1], normalized[2]);
            glVertex3f(x4, y4, z4);

            normalized = glm::normalize(glm::vec3(x2, y2, z2));
            glNormal3f(normalized[0], normalized[1], normalized[2]);
            glVertex3f(x2, y2, z2);

            //top triangles
            normalized = glm::normalize(glm::vec3(x3, y3, z3));
            glNormal3f(normalized[0], normalized[1], normalized[2]);
            glVertex3f(x3, y3, z3);

            normalized = glm::normalize(glm::vec3(x, y, z));
            glNormal3f(normalized[0], normalized[1], normalized[2]);
            glVertex3f(x, y, z);

            normalized = glm::normalize(glm::vec3(x2, y2, z2));
            glNormal3f(normalized[0], normalized[1], normalized[2]);
            glVertex3f(x2, y2, z2);

            glEnd();
        }
    }
}

void Sphere::drawNormal() {
    float radius = 0.5;
    int x_segments = Sphere::m_segmentsX;
    int y_segments = Sphere::m_segmentsY;
    float fi = 2 * M_PI / x_segments;
    float theta    = M_PI / y_segments;

    for (int i = 0; i < y_segments; i++) {
        for (int j = 0; j < x_segments; j++) {
            //top right A
            float x = radius * sin(theta * i) * cos(fi * j);
            float z = radius * sin(theta * i) * sin(fi * j);
            float y = radius * cos(theta * i);
            //top left B
            float x2 = radius * sin(theta * (i)) * cos(fi * (j + 1));
            float z2 = radius * sin(theta * i) * sin(fi * (j + 1));
            float y2 = radius * cos(theta * (i));
            //bottom right C
            float x3 = radius * sin(theta * (i + 1)) * cos(fi * j);
            float z3 = radius * sin(theta * (i + 1)) * sin(fi * (j));;
            float y3 = radius * cos(theta * (i + 1));
            //bottom left D
            float x4 = radius * sin(theta * (i + 1)) * cos(fi * (j + 1));
            float z4 = radius * sin(theta * (i + 1)) * sin(fi * (j + 1));;
            float y4 = radius * cos(theta * (i + 1));

            // B-A
            float xn1 = x2 - x;
            float yn1 = y2 - y;
            float zn1 = z2 - z;
            glm::vec3 BA = glm::vec3(xn1, yn1, zn1);

            //C-A
            float xn2 = x3 - x;
            float yn2 = y3 - y;
            float zn2 = z3 - z;
            glm::vec3 CA = glm::vec3(xn2, yn2, zn2);

            glm::vec3 crossed = glm::cross(BA, CA);
            glm::vec3 normalized = glm::normalize(crossed);

            glBegin(GL_LINES);

            glVertex3f(x, y, z); 
            glVertex3f(x + normalized.x, y + normalized.y, z + normalized.z);

            glEnd();
        }
    }
}
