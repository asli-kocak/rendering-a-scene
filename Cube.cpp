#include "Cube.h"
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

const int FRONT_BACK = 0;
const int LEFT_RIGHT = 1;
const int TOP_BOTTOM = 2;


void drawSquare(Cube::vertex top_left, float x_len, float y_len, int face);
void drawCubeFace(Cube::vertex top_left, float delta_x, float delta_y, float delta_z, int face);

void drawNormalFace(Cube::vertex face, Cube::vertex face_added, int facetype);

//Helper Functions
// uses 2 triangle to create a square (one of the squares making up the face of a cube)
//void drawSquare(Cube::vertex v1, Cube::vertex v2, Cube::vertex v3, Cube::vertex v4);
void drawSquare(Cube::vertex top_left, float x_len, float y_len, float z_len, int face) {


    float tl_z, tl_x, tl_y;
    float tr_z, tr_x, tr_y;
    float bl_z, bl_x, bl_y;
    float br_z, br_x, br_y;

    tl_x = top_left.x;
    tl_y = top_left.y;
    tl_z = top_left.z;
    
    float normal_x, normal_y, normal_z;

    if (face == FRONT_BACK) {
        tr_z = tl_z;
        tr_x = tl_x + x_len;
        tr_y = tl_y;

        br_z = tl_z;
        br_x = tl_x + x_len;
        br_y = tl_y - y_len;

        bl_z = tl_z;
        bl_x = tl_x;
        bl_y = tl_y - y_len;
        
        normal_z = 2 * bl_z;

        normal_x = 0;
        normal_y = 0;


    } else if (face == LEFT_RIGHT) {
        tr_z = tl_z + z_len;
        tr_x = tl_x;
        tr_y = tl_y;

        br_z = tl_z + z_len;
        br_x = tl_x;
        br_y = tl_y - y_len;

        bl_z = tl_z;
        bl_x = tl_x;
        bl_y = tl_y - y_len;

        normal_x = 2 * tl_x;
        normal_z = 0;
        normal_y = 0;
        

    } else {
        tr_z = tl_z;
        tr_x = tl_x + x_len;
        tr_y = tl_y;

        br_z = tl_z + z_len;
        br_x = tl_x + x_len;
        br_y = tl_y;

        bl_z = tl_z + z_len;
        bl_x = tl_x;
        bl_y = tl_y;


        normal_y = 2 * bl_y;
        normal_z = 0;
        normal_x = 0;


    }

    glBegin(GL_TRIANGLES);


    if (normal_x > 0 || normal_y < 0 || normal_z < 0) {
        
        glNormal3f(normal_x,normal_y,normal_z);
        glVertex3f(br_x, br_y, br_z);
        glVertex3f(bl_x, bl_y, bl_z);
        glVertex3f(tl_x, tl_y, tl_z);

        glVertex3f(tr_x, tr_y, tr_z);
        glVertex3f(br_x, br_y, br_z);
        glVertex3f(tl_x, tl_y, tl_z);
    } else {
        glNormal3f(normal_x,normal_y,normal_z);

        glVertex3f(tl_x, tl_y, tl_z);
        glVertex3f(bl_x, bl_y, bl_z);
        glVertex3f(br_x, br_y, br_z);

        glVertex3f(tl_x, tl_y, tl_z);
        glVertex3f(br_x, br_y, br_z);
        glVertex3f(tr_x, tr_y, tr_z);
    }
     
    glEnd();

    //make vector holding all 4 vertices
}
//Cube Class Functions

float calculateDelta_x() {
    // cout << "segments x:" << Cube::m_segmentsX << endl;
    float deltaX = 1.0 / Cube::m_segmentsX;
    // cout << "delta x:" << deltaX << endl;

    return deltaX;
}

float calculateDelta_y() {
    float deltaY = 1.0 / Cube::m_segmentsY;
    // cout << "delta y:" << deltaY << endl;

    return deltaY;
}

void Cube::draw() {
    // cout << "calling cube draw" << endl;
    // top left vertices for each face
    Cube::vertex front_face; 
        front_face.x= -0.5;       
        front_face.y= 0.5;
        front_face.z= 0.5;
        
    Cube::vertex back_face; 
        back_face.x= -0.5;       
        back_face.y= 0.5;
        back_face.z= -0.5;

    Cube::vertex right_face; 
        right_face.x= 0.5;       
        right_face.y= 0.5;
        right_face.z= -0.5;


    Cube::vertex left_face; 
        left_face.x= -0.5;       
        left_face.y= 0.5;
        left_face.z= -0.5;    
    

    Cube::vertex top_face; 
        top_face.x= -0.5;       
        top_face.y= 0.5;
        top_face.z= -0.5;

    Cube::vertex bottom_face; 
        bottom_face.x= -0.5;       
        bottom_face.y= -0.5;
        bottom_face.z= -0.5;
    
    drawCubeFace(front_face, calculateDelta_x(), calculateDelta_y(), 0, 0);
    drawCubeFace(back_face, calculateDelta_x(), calculateDelta_y(), 0, 0);
    drawCubeFace(right_face, 0, calculateDelta_y(), calculateDelta_x(), 1);
    drawCubeFace(left_face, 0, calculateDelta_y(), calculateDelta_x(), 1);
    drawCubeFace(top_face, calculateDelta_x(), 0, calculateDelta_y(), 2);
    drawCubeFace(bottom_face, calculateDelta_x(), 0, calculateDelta_y(), 2);
}

void drawCubeFace(Cube::vertex top_left, float delta_x, float delta_y, float delta_z, int face) {

  //  face *facelist = new facelist[1*Cube::m_segmentsX*Cube::m_segmentsY*6];

    float reset_y = top_left.y;
    float reset_x = top_left.x;
    float reset_z = top_left.z;

    if (face == FRONT_BACK) {
        for (int i = 0; i < Cube::m_segmentsX; i ++) {
            for (int j = 0; j < Cube::m_segmentsY; j++) {
                drawSquare(top_left, delta_x, delta_y, delta_z, face);
                top_left.y -= delta_y;
                
            }
            top_left.y = reset_y;
            top_left.x += delta_x;
        }
    } else if (face == LEFT_RIGHT) {
        for (int i = 0; i < Cube::m_segmentsX; i ++) {
            for (int j = 0; j < Cube::m_segmentsY; j++) {
                drawSquare(top_left, delta_x, delta_y, delta_z, face);
                top_left.y -= delta_y;
            }
            top_left.y = reset_y;
            top_left.z += delta_z;
        }
    } else {
        for (int i = 0; i < Cube::m_segmentsX; i ++) {
            for (int j = 0; j < Cube::m_segmentsY; j++) {
                drawSquare(top_left, delta_x, delta_y, delta_z, face);
                top_left.z += delta_z;
            }
            top_left.z = reset_z;
            top_left.x += delta_x;
        }
    }
}

void Cube::drawNormal() {

// 1- if front : 0, 0, 1
// 2- if back  : 0, 0,-1
// 3- if right : 1, 0, 0
// 4- if left  :-1, 0, 0
// 5- if top   : 0, 1, 0
// 6- if bottom: 0,-1, 0vv


    
    vertex face, addition;
    face.x = -0.5;      face.y = -0.5;      face.z = 0.5;
    addition.x = 0;     addition.y = 0;     addition.z = 0.25;
    drawNormalFace(face, addition, FRONT_BACK);

    face.x = -0.5;      face.y = -0.5;      face.z = -0.5;
    addition.x = 0;     addition.y = 0;     addition.z = -0.25;    
    drawNormalFace(face, addition, FRONT_BACK);

    face.x = -0.5;      face.y = -0.5;      face.z = -0.5;
    addition.x = -0.25; addition.y = 0;     addition.z = 0;    
    drawNormalFace(face, addition, LEFT_RIGHT);

    face.x = 0.5;       face.y = -0.5;      face.z = -0.5;
    addition.x = 0.25;  addition.y = 0;     addition.z = 0;    
    drawNormalFace(face, addition, LEFT_RIGHT);

    face.x = -0.5;       face.y = -0.5;      face.z = -0.5;
    addition.x = 0;      addition.y = -0.25; addition.z = 0;    
    drawNormalFace(face, addition, TOP_BOTTOM);

    face.x = -0.5;       face.y = 0.5;       face.z = -0.5;
    addition.x = 0;      addition.y = 0.25;  addition.z = 0;    
    drawNormalFace(face, addition, TOP_BOTTOM);

}

void drawNormalFace(Cube::vertex face, Cube::vertex face_added, int facetype) {
    int numX = Cube::m_segmentsX+1;
    int numY = Cube::m_segmentsY+1;

    int numNormals = numX * numY;

    Cube::vertex normalList[numNormals];
    if (facetype == FRONT_BACK){
        for (int i = 0; i < numX; i ++) {
            for (int j = 0; j < numY; j ++) {
                normalList[(numY*i)+j].x = face.x + (calculateDelta_x() * i);
                normalList[(numY*i)+j].y = face.y + (calculateDelta_y() * j);
                normalList[(numY*i)+j].z = face.z;
            }
        }
    } else if (facetype == LEFT_RIGHT) {
        for (int i = 0; i < numX; i ++) {
            for (int j = 0; j < numY; j ++) {
                normalList[(numY*i)+j].x = face.x;
                normalList[(numY*i)+j].y = face.y + (calculateDelta_y() * j);
                normalList[(numY*i)+j].z = face.z + (calculateDelta_x() * i);
            }
        }
    } else if (facetype == TOP_BOTTOM) {
        for (int i = 0; i < numX; i ++) {
            for (int j = 0; j < numY; j ++) {
                normalList[(numY*i)+j].x = face.x + (calculateDelta_x() * i);
                normalList[(numY*i)+j].y = face.y;
                normalList[(numY*i)+j].z = face.z + (calculateDelta_y() * j);
            }
        }
    }

    for (int i = 0; i < numNormals; i ++) {
        glBegin(GL_LINES);
        glVertex3f(normalList[i].x , normalList[i].y, normalList[i].z);
        glVertex3f(normalList[i].x + face_added.x, 
                   normalList[i].y + face_added.y, 
                   normalList[i].z + face_added.z);
        glEnd();
    }



}