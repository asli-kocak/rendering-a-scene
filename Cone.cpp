#include "Cone.h"
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

using namespace std;

void drawTipTriangle();
void drawCircle(Cone::vertex v1);
void drawBaseTriangle(float tier); 

void Cone::draw() {
    Cone::vertex base;
    base.x = 0.0;
    base.y = -0.5;
    base.z = 0.0;
    drawCircle(base); // coming from cylinder.cpp
    drawTipTriangle();
    for (int i = 1.0; i < Cone::m_segmentsY; i++) {
        drawBaseTriangle(i);
    }
}

void Cone::drawNormal() {
    //Base middle
    float yBottom = -0.5;
    glBegin(GL_LINES);
    glVertex3f(0, yBottom, 0);
    glVertex3f(0, yBottom - 0.1, 0);
    glEnd();

    //base triangles
    float upper_x, upper_y, upper_z;
    float right_x, right_y, right_z;
    float left_x, left_y, left_z;
    float upper_xR, upper_yR, upper_zR;

    float num_segments_y = Cone::m_segmentsY;
    int num_segments_x = Cone::m_segmentsX;
    
    for (int tier = 1; tier < num_segments_y; tier++) {
        left_y = -0.5 + ((tier - 1) / num_segments_y);
        right_y = -0.5 + ((tier - 1) / num_segments_y);
        upper_y = -0.5 + (tier * (1.0/num_segments_y));
        upper_yR = -0.5 + (tier * (1.0/num_segments_y));

        right_x = 0.5 * ((num_segments_y + 1 - tier)/num_segments_y);
        right_z = 0;
        upper_xR = 0.5 * ((num_segments_y - tier)/num_segments_y);
        upper_zR = 0;

        if (num_segments_x < 4) {
            left_x = (-0.5/2.0) * ((num_segments_y + 1 - tier)/num_segments_y);
            left_z = (sqrt(3.0)/2.0)/2.0 * 
                ((num_segments_y + 1 - tier)/num_segments_y);
            upper_x = (-0.5/2.0) * ((num_segments_y - tier)/num_segments_y);
            upper_z = ((sqrt(3.0)/2.0/2.0)) * 
                ((num_segments_y - tier)/num_segments_y);
        } else {
            float left_x_edge_len = 
                (cosf((2.0 * M_PI) / num_segments_x) / 2.0);
            float left_z_edge_len = 
                (sinf((2.0 * M_PI) / num_segments_x) / 2.0);
            float b_tier_factor = ((num_segments_y + 1 - tier)/num_segments_y);
            float tip_factor = ((num_segments_y - tier)/num_segments_y);
            left_x = left_x_edge_len * b_tier_factor;
            left_z = left_z_edge_len * b_tier_factor;
            upper_x = left_x_edge_len * tip_factor;
            upper_z = left_z_edge_len * tip_factor;
        }

        for (int i = 0; i < num_segments_x; i++) {
            glBegin(GL_LINES);
            glVertex3f(left_x, left_y, left_z);
            glVertex3f(left_x + (0.1 * (2/sqrt(5))), 
                        left_y + (0.1* (1.0/sqrt(5.0))), left_z);

            ////base 
            if (tier == 1) {
                glVertex3f(left_x, left_y, left_z);
                glVertex3f(left_x, left_y * (1.1), left_z);
            }
            glEnd();
            glRotatef((360.0/num_segments_x), 0, 1, 0);
        }
    }
}

void drawTipTriangle() {
    float tip_x, tip_y, tip_z;
    float right_x, right_y, right_z;
    float left_x, left_y, left_z;

    tip_x = 0;
    tip_y = 0.5;
    tip_z = 0;

    float num_segments_x = Cone::m_segmentsX;
    float num_segments_y = Cone::m_segmentsY;

    left_y = 0.5 - (1.0/num_segments_y); 
    right_y = 0.5 - (1.0/num_segments_y);

    right_x = 0.5 / num_segments_y;
    right_z = 0;

    if (num_segments_x < 4) {
        left_x = (-0.5)/2.0/num_segments_y;
        left_z = (sqrt(3.0)/2.0)/2.0/num_segments_y;
    } else {
        left_x = (cosf((2.0 * M_PI) / num_segments_x) / 2.0) / num_segments_y;
        left_z = (sinf((2.0 * M_PI) / num_segments_x) / 2.0) / num_segments_y;
    }

    glm::vec3 normalized = glm::vec3(0);
    for (int i = 0; i < num_segments_x; i++) {
        glBegin(GL_TRIANGLES);


        normalized = glm::normalize(glm::vec3(tip_x, 0, tip_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(tip_x, tip_y, tip_z);

        normalized = glm::normalize(glm::vec3(left_x, 0, left_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(left_x, left_y, left_z);

        normalized = glm::normalize(glm::vec3(right_x, 0, right_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(right_x, right_y, right_z);


        glEnd();
        
        glRotatef((360.0/num_segments_x), 0, 1, 0);
    }
}

void drawBaseTriangle(float tier) {
    float upper_x, upper_y, upper_z;
    float right_x, right_y, right_z;
    float left_x, left_y, left_z;
    float upper_xR, upper_yR, upper_zR;

    float num_segments_y = Cone::m_segmentsY;
    int num_segments_x = Cone::m_segmentsX;

    left_y = -0.5 + ((tier - 1) / num_segments_y);
    right_y = -0.5 + ((tier - 1) / num_segments_y);
    upper_y = -0.5 + (tier * (1.0/num_segments_y));
    upper_yR = -0.5 + (tier * (1.0/num_segments_y));

    right_x = 0.5 * ((num_segments_y + 1 - tier)/num_segments_y);
    right_z = 0;
    upper_xR = 0.5 * ((num_segments_y - tier)/num_segments_y);
    upper_zR = 0;

    if (num_segments_x < 4) {
        left_x = (-0.5/2.0) * ((num_segments_y + 1 - tier)/num_segments_y);
        left_z = (sqrt(3.0)/2.0)/2.0 * 
                    ((num_segments_y + 1 - tier)/num_segments_y);
        upper_x = (-0.5/2.0) * ((num_segments_y - tier)/num_segments_y);
        upper_z = ((sqrt(3.0)/2.0/2.0)) * 
                    ((num_segments_y - tier)/num_segments_y);
    } else {
        float left_x_edge_len = (cosf((2.0 * M_PI) / num_segments_x) / 2.0);
        float left_z_edge_len = (sinf((2.0 * M_PI) / num_segments_x) / 2.0);
        float b_tier_factor = ((num_segments_y + 1 - tier)/num_segments_y);
        float tip_factor = ((num_segments_y - tier)/num_segments_y);
        left_x = left_x_edge_len * b_tier_factor;
        left_z = left_z_edge_len * b_tier_factor;
        upper_x = left_x_edge_len * tip_factor;
        upper_z = left_z_edge_len * tip_factor;
    }

    glm::vec3 normalized = glm::vec3(0);
    for (int i = 0; i < num_segments_x; i++) {
        glBegin(GL_TRIANGLES);

        //upper left right
        //left upper right

        normalized = glm::normalize(glm::vec3(upper_x, 0, upper_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(upper_x, upper_y, upper_z);

        normalized = glm::normalize(glm::vec3(left_x, 0, left_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(left_x, left_y, left_z);


        normalized = glm::normalize(glm::vec3(right_x, 0, right_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(right_x, right_y, right_z);



        //upper right xr
        //xr upper right

        normalized = glm::normalize(glm::vec3(upper_xR, 0, upper_zR));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(upper_xR, upper_yR, upper_zR);

        normalized = glm::normalize(glm::vec3(upper_x, 0, upper_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(upper_x, upper_y, upper_z);

        normalized = glm::normalize(glm::vec3(right_x, 0, right_z));
        glNormal3f(normalized[0], normalized[1], normalized[2]);
        glVertex3f(right_x, right_y, right_z);




 

        glEnd();
        glRotatef((360.0/num_segments_x), 0, 1, 0);
    }
}