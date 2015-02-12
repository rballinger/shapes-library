/**
 *  Author: Ryan Ballinger      Date: 2-12-15
 *  Project: Shapes Library     File: Example.h
 */

#include "BufferObject.h"

class Example : public BufferObject {
private:
    const float BREAD_THICKNESS = 0.75;
    const float CRUST_THICKNESS = 8.75;
    const float BREAD_WIDTH = 6.25;
    const float BREAD_LENGTH = 6.5;
    const float SMALL_RAD = 0.875;
    const int SUBDIV = 6;
    int total_count,
        before_left_round_count,
        before_right_round_count,
        before_top_count,
        square_count,
        square_points,
        crust_count,
        before_crust_count,
        round_count,
        side_points;

protected:
public:
    void build ();
    void render(bool) const;
};
