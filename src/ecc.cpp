#include "elliptic_curve.h"

ImVec2 ECC::get_intersection(std::vector<ImVec2> points, int num, int num2){
    ImVec2 a = points.at(num);
    ImVec2 b = points.at(num2);

    float m = (a.y - b.y) / (a.x - b.x);
    float v = a.y - m * a.x;
    float x = std::pow(m, 2) - a.x - b.x;
    float y = m * x + v;
    if (y < 0){
        y=-y;
    }
    return ImVec2(x,y);
}


ImVec2 ECC::double_point(ImVec2 p, float a){
    float s = 3 * std::pow(p.x, 2) + a /  2 * p.y;
    float x3 = std::pow(s, 2) - p.x - p.x;
    float y3 = s*(p.x - x3) - p.y;
    
    return {x3, y3};
}


