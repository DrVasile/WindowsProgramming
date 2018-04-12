#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <vector>

class Ball {
public:
    COLORREF ballColor;
    POINT center;
    int horVelocity;
    int verVelocity;
    Ball(POINT, int, int, COLORREF);
    void ChangeColor(const COLORREF &color);
    void ChangeVelocity(const int &newHorVelocity, const int &newVerVelocity);
    BOOL CollisionWithWall(const RECT &rect);
void MoveBall(const HDC &hdc, const RECT &rect, HBRUSH &hBrush);
};

BOOL Collision(Ball &first, Ball &second);
