#include "Cylinder.h"
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

using namespace std;

void drawCircle(Cylinder::vertex v);
void drawRectangle(Cylinder::vertex v1);

void Cylinder::draw() {
    Cylinder::vertex top_face;
    top_face.x = 0.0;
    top_face.y = 0.5;
    top_face.z = 0.0;
    drawCircle(top_face);
    
    Cylinder::vertex bottom_face;
    bottom_face.x = 0.0;
    bottom_face.y = -0.5;
    bottom_face.z = 0.0;
    drawCircle(bottom_face);
    
    float side_y = 0.5;
    for (int i = 0; i < Cylinder::m_segmentsY; i++) {
        Cylinder::vertex side_face;
        side_face.x = 0.5;
        side_face.y = side_y; 
        side_face.z = 0.0;
        side_y = side_y - (1.0 / Cylinder::m_segmentsY);
        drawRectangle(side_face);
    }
}

void Cylinder::drawNormal() {
    //Bases middle
    float yTop = 0.5;
    float yBottom = -0.5;
    glBegin(GL_LINES);
    glVertex3f(0, yTop, 0);
    glVertex3f(0, yTop + 0.1, 0);
    glVertex3f(0, yBottom, 0);
    glVertex3f(0, yBottom - 0.1, 0);
    glEnd();

    //Sides
    float tl_x, tl_y, tl_z;
    float tr_x, tr_y, tr_z;
    float bl_x, bl_y, bl_z;
    float br_x, br_y, br_z;

    int num_segments = Cylinder::m_segmentsX;
    float rec_height = 1.0 / Cylinder::m_segmentsY;

    tl_y = 0.5;
    bl_y = tl_y - rec_height;
    br_y = tl_y - rec_height;
    tr_y = tl_y;

    tl_x = 0.5;
    tl_z = 0;

    bl_x = tl_x;
    bl_z = tl_z;

    if (num_segments < 4) {
        tr_x = (-0.5)/2;
        tr_z = (sqrt(3)/2)/2;
        br_x = (-0.5)/2;
        br_z = (sqrt(3)/2)/2;
    } else {
        tr_x = cosf((2 * M_PI) / num_segments)/2;
        tr_z = sinf((2 * M_PI) / num_segments)/2;
        br_x = cosf((2 * M_PI) / num_segments)/2;
        br_z = sinf((2 * M_PI) / num_segments)/2;
    }
    
    for (int tier = 0; tier < Cylinder::m_segmentsY + 1; tier++) {
        float triangle_height = 1.0 / Cylinder::m_segmentsY;
        for (int i = 0; i < num_segments; i++) {
            glBegin(GL_LINES);

            glVertex3f(tl_x, tl_y - (tier * triangle_height), tl_z);
            glVertex3f(tl_x*1.1, tl_y - (tier * triangle_height), tl_z*1.1);

            glEnd();
            glRotatef((360.0/num_segments), 0, 1, 0);
        }
    }
    

    for (int i = 0; i < num_segments; i++) {
        glBegin(GL_LINES);

        glVertex3f(tl_x, tl_y, tl_z);
        glVertex3f(tl_x, tl_y*1.1, tl_z);

        glVertex3f(tl_x, -tl_y, tl_z);
        glVertex3f(tl_x, tl_y*-1.1, tl_z);

        glEnd();
        glRotatef((360.0/num_segments), 0, 1, 0);
    }
}

void drawCircle(Cylinder::vertex v1) {
    float x1, y1, z1;
    float x2, y2, z2;
    float x3, y3, z3;

    int num_segments = Cylinder::m_segmentsX;
    
    y1 = y2 = y3 = v1.y;

    x1           = v1.x;
    x2           = 0.5;

    z1           = v1.z;
    z2           = 0;

    if (num_segments < 4) {
        x3           = (-0.5)/2;
        z3           = (sqrt(3)/2)/2;
    } else {
        x3           = cosf((2 * M_PI) / num_segments)/2;
        z3           = sinf((2 * M_PI) / num_segments)/2;
    }
    
    for (int i = 0; i < num_segments; i++) {
        glBegin(GL_TRIANGLES);
        
        if (y1 == -0.5) {
            glNormal3f(x1, y1, z1);
            glVertex3f(x1, y1, z1);
            // glNormal3f(x2, y2, z2);
            glVertex3f(x2, y2, z2);
            // glNormal3f(x3, y3, z3);
            glVertex3f(x3, y3, z3);
        } else {
            glNormal3f(x1, y1, z1);
            glVertex3f(x1, y1, z1);
            // glNormal3f(x3, y3, z3);
            glVertex3f(x3, y3, z3);
            // glNormal3f(x2, y2, z2);
            glVertex3f(x2, y2, z2);
        }
        
        glEnd();
      
        glRotatef(360.0/num_segments, v1.x, v1.y, v1.z);
    }
}

void drawRectangle(Cylinder::vertex v1) {
    float tl_x, tl_y, tl_z;
    float tr_x, tr_y, tr_z;
    float bl_x, bl_y, bl_z;
    float br_x, br_y, br_z;

    int num_segments = Cylinder::m_segmentsX;
    float rec_height = 1.0 / Cylinder::m_segmentsY;

    tl_y = v1.y;
    bl_y = tl_y - rec_height;
    br_y = tl_y - rec_height;
    tr_y = tl_y;

    tl_x = v1.x;
    tl_z = v1.z;

    bl_x = tl_x;
    bl_z = tl_z;

    if (num_segments < 4) {
        tr_x = (-0.5)/2;
        tr_z = (sqrt(3)/2)/2;
        br_x = (-0.5)/2;
        br_z = (sqrt(3)/2)/2;
    } else {
        tr_x = cosf((2 * M_PI) / num_segments)/2;
        tr_z = sinf((2 * M_PI) / num_segments)/2;
        br_x = cosf((2 * M_PI) / num_segments)/2;
        br_z = sinf((2 * M_PI) / num_segments)/2;
    }

    glm::vec3 normalized = glm::vec3(1.0);

    for (int i = 0; i < num_segments; i++) {
        glBegin(GL_TRIANGLES);

        normalized = glm::normalize(glm::vec3(tl_x, 0, tl_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(tl_x, tl_y, tl_z);

        normalized = glm::normalize(glm::vec3(bl_x, 0, bl_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(bl_x, bl_y, bl_z);

        normalized = glm::normalize(glm::vec3(br_x, 0, br_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(br_x, br_y, br_z);

        
        normalized = glm::normalize(glm::vec3(tl_x, 0, tl_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(tl_x, tl_y, tl_z);

        normalized = glm::normalize(glm::vec3(tr_x, 0, tr_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(tr_x, tr_y, tr_z);

        normalized = glm::normalize(glm::vec3(br_x, 0, br_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(br_x, br_y, br_z);

        glEnd();
        glRotatef((360.0/num_segments), 0, 1, 0);
    }
}