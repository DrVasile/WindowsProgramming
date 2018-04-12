#include <iostream>
#include "declarations.h"
#include "macros.h"

using namespace std;

Ball::Ball(POINT center, int newHorVelocity, int newVerVelocity, COLORREF color) {
    this->center = center;
    this->horVelocity = newHorVelocity;
    this->verVelocity = newVerVelocity;
    this->ballColor = color;
    return;
}

void Ball::ChangeColor(const COLORREF &color) {
    this->ballColor = color;
    return;
}

void Ball::ChangeVelocity(const int &newHorVelocity, const int &newVerVelocity) {
    this->horVelocity = newHorVelocity;
    this->verVelocity = newVerVelocity;
    return;
}

BOOL Ball::CollisionWithWall(const RECT &rect) {
    if (this->center.x - (ballRadius / 2) < rect.left) {
        this->horVelocity = -this->horVelocity;
        return TRUE;
    }

    if (this->center.x + (ballRadius / 2) > rect.right) {
        this->horVelocity = -this->horVelocity;
        return TRUE;
    }

    if (this->center.y - (ballRadius / 2) < rect.top) {
        this->verVelocity = -this->verVelocity;
        return TRUE;
    }

    if (this->center.y + (ballRadius / 2) > rect.bottom) {
        this->verVelocity = -this->verVelocity;
        return TRUE;
    }

    return FALSE;
}

void Ball::MoveBall(const HDC &hdc, const RECT &rect, HBRUSH &hBrush) {
    hBrush = CreateSolidBrush(this->ballColor);
    SelectObject(hdc, hBrush);
    CollisionWithWall(rect);
    this->center.x += this->horVelocity;
    this->center.y += this->verVelocity;
    Ellipse(hdc, center.x - ballRadius, center.y - ballRadius, center.x + ballRadius, center.y + ballRadius);
    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return;
}


BOOL Collision(Ball &first, Ball &second) {
    DOUBLE distance;
    distance = sqrt(pow((DOUBLE) (first.center.x - second.center.x), 2.0) + pow((DOUBLE) (first.center.y - second.center.y), 2.0));
    srand(time(NULL));
    printf("%lf %d %d %d %d\n", distance, first.center.x, first.center.y, second.center.x, second.center.y);
    if (distance < (DOUBLE) ballRadius * 2.0) {
        first.ChangeVelocity(second.horVelocity, second.verVelocity);
		second.ChangeVelocity(first.horVelocity, first.verVelocity);
		first.ChangeColor(RGB(RAND_COLOR, RAND_COLOR, RAND_COLOR));
		second.ChangeColor(RGB(RAND_COLOR, RAND_COLOR, RAND_COLOR));
		return TRUE;
    }

    return FALSE;
}
