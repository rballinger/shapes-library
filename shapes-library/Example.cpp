/**
 *  Author: Ryan Ballinger      Date: 2-12-15
 *  Project: Shapes Library     File: Example.cpp
 */

#include <cmath>
#include "Example.h"
#define M_PI           3.14159265358979323846

using glm::vec3;

void Example::build(){
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    vec3 v1, v2, c1, c2, c3, c4;

    // build bread

    // bread bottom square
    c1 = {-BREAD_WIDTH / 2, BREAD_LENGTH / 2, 0};
    c2 = {BREAD_WIDTH / 2, BREAD_LENGTH / 2, 0};
    c3 = {-BREAD_WIDTH / 2, -BREAD_LENGTH / 2, 0};
    c4 = {BREAD_WIDTH / 2, -BREAD_LENGTH / 2, 0};
    all_index.push_back(all_points.size());
    all_points.push_back(c1);
    all_index.push_back(all_points.size());
    all_points.push_back(c3);

    for(int i = 1; i < SUBDIV; i++){
        float t = (float) i / SUBDIV;
        v1 = {t * c2 + (1 - t) * c1};
        v2 = {t * c4 + (1 - t) * c3};
        all_index.push_back(all_points.size());
        all_points.push_back(v1);
        all_index.push_back(all_points.size());
        all_points.push_back(v2);
    }

    all_index.push_back(all_points.size());
    all_points.push_back(c2);
    all_index.push_back(all_points.size());
    all_points.push_back(c4);

    square_count = all_index.size();
    square_points = all_points.size();

    // bread round edges
    // left round edge
    c1.y -= SMALL_RAD;
    all_index.push_back(all_points.size());
    all_points.push_back(c1);
    v1 = {c1.x, c1.y - SMALL_RAD, c1.z};
    for(int i = 0; i < SUBDIV; i++){
        v1 = {c1.x - (SMALL_RAD * sin(i * M_PI / SUBDIV)), c1.y + (SMALL_RAD * cos(i * M_PI / SUBDIV)), c1.z};
        all_index.push_back(all_points.size());
        all_points.push_back(v1);
    }
    c1.y -= SMALL_RAD;
    all_index.push_back(all_points.size());
    all_points.push_back(c1);

    before_right_round_count = all_index.size();
    round_count = all_index.size() - square_count;
    // right round edge
    c2.y -= SMALL_RAD;
    all_index.push_back(all_points.size());
    all_points.push_back(c2);
    v1 = {c2.x, c2.y - SMALL_RAD, c2.z};
    for(int i = 0; i < SUBDIV; i++){
        v1 = {c2.x + (SMALL_RAD * sin(i * M_PI / 6)), c2.y + (SMALL_RAD * cos(i * M_PI / 6)), c2.z};
        all_index.push_back(all_points.size());
        all_points.push_back(v1);
    }
    c2.y -= SMALL_RAD;
    all_index.push_back(all_points.size());
    all_points.push_back(c2);

    before_top_count = all_index.size();
    side_points = all_points.size();
    // bread top
    for(auto v : all_points){
        v.z += BREAD_THICKNESS;
        all_index.push_back(all_points.size());
        all_points.push_back(v);
    }

    before_crust_count = all_index.size();
    // crust
    // top side
    for(int i = 0; i < square_points; i++){
        all_index.push_back(i);
        all_index.push_back(i + side_points);
        i++;
    }
    // right round side
    for(int i = 24; i < 30; i++){
        all_index.push_back(i);
        all_index.push_back(i + side_points);
    }
    // bottom side
    for(int i = square_points - 1; i > 0; i--){
        all_index.push_back(i);
        all_index.push_back(i + side_points);
        i--;
    }
    // left round side
    for(int i = 21; i > 14; i--){
        all_index.push_back(i);
        all_index.push_back(i + side_points);
    }

    total_count = all_index.size();
    crust_count = total_count - before_crust_count;

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, all_points.size() * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
    float *vertex_ptr = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    /* Initialize the vertices */
    float *ptr = vertex_ptr;
    for (auto v : all_points) {
        ptr[0] = v.x;
        ptr[1] = v.y;
        ptr[2] = v.z;
        ptr += 3;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Initialize the indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, all_index.size() * sizeof(GLushort), all_index.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Example::render(bool outline) const{
    /* bind vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDisableClientState(GL_COLOR_ARRAY);
//    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
//    glColorPointer(3, GL_FLOAT, 0, 0);v1 = {TOP_WIDTH/2, -TOP_LENGTH/2, HEIGHT/2};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    /* render the polygon */
    // set mode
    if(outline){
        glPolygonMode(GL_FRONT, GL_LINE);
    }else{
        glPolygonMode(GL_FRONT, GL_FILL);
    }
    // bread bottom square
    glFrontFace(GL_CW);
    glColor3ub (196, 140, 89);
    glDrawRangeElements(GL_QUAD_STRIP, 0, 0, square_count, GL_UNSIGNED_SHORT, 0);

    // bread left round edge
    glFrontFace(GL_CW);
    glColor3ub (196, 140, 89);
    glDrawRangeElements(GL_TRIANGLE_FAN, 0, 0, round_count, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * square_count));

    // bread right round edge
    glFrontFace(GL_CCW);
    glColor3ub (196, 140, 89);
    glDrawRangeElements(GL_TRIANGLE_FAN, 0, 0, round_count, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * before_right_round_count));

    // bread top square
    glFrontFace(GL_CCW);
    glColor3ub (196, 140, 89);
    glDrawRangeElements(GL_QUAD_STRIP, 0, 0, square_count, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * before_top_count));

    // bread left round edge
    glFrontFace(GL_CCW);
    glColor3ub (196, 140, 89);
    glDrawRangeElements(GL_TRIANGLE_FAN, 0, 0, round_count, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * (before_top_count + square_count)));

    // bread right round edge
    glFrontFace(GL_CW);
    glColor3ub (196, 140, 89);
    glDrawRangeElements(GL_TRIANGLE_FAN, 0, 0, round_count, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * (before_top_count + square_count + round_count)));

    // crust
    glFrontFace(GL_CCW);
    glColor3ub (134, 61, 18);
    glDrawRangeElements(GL_QUAD_STRIP, 0, 0, crust_count, GL_UNSIGNED_SHORT, (void *) (sizeof(GLushort) * before_crust_count));

    /* unbind the buffers */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnableClientState(GL_COLOR_ARRAY);
}
