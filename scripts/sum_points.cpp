#include <cstdio>

struct point{
    int x;
    int y;
};

int main(){
    // over y^2 = x^3 - 7x + 10
    point P{1,2}, Q{3,4}, R;
    
    int m = (P.y - Q.y) / (P.x - Q.x);
    R.x = m*m - P.x - Q.x;
    R.y = P.y + m * (R.x - P.x);
    R.y = -1 * R.y;
    printf("m = %d; Rx = %d; Ry = %d\n", m, R.x, R.y);

    return 0;
}
