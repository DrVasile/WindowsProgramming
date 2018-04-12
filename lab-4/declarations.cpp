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

void Ball::CollisionWithWall(const RECT &rect) {
    if (this->center.x - (ballRadius / 2) < rect.left)
        this->horVelocity = -this->horVelocity;

    if (this->center.x + (ballRadius / 2) > rect.right)
        this->horVelocity = -this->horVelocity;

    if (this->center.y - (ballRadius / 2) < rect.top)
        this->verVelocity = -this->verVelocity;

    if (this->center.y + (ballRadius / 2) > rect.bottom)
        this->verVelocity = -this->verVelocity;

    return;
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

int Norm(POINT &vect) {
    return (int) sqrt(vect.x * vect.x + vect.y * vect.y);
}

int DotProduct(POINT &first, POINT &second) {
    return (first.x * second.x) + (first.y * second.y);
}

void Collision(Ball &first, Ball &second) {
    DOUBLE distance;
    distance = sqrt(pow((DOUBLE) (first.center.x - second.center.x), 2.0) + pow((DOUBLE) (first.center.y - second.center.y), 2.0));
    srand(time(NULL));
    int horResVelocity;
    int VerResVelocity;

    if (distance < (DOUBLE) ballRadius * 2.0) {
        ModifyVelocities(first, second);
		first.ChangeColor(RGB(RAND_COLOR, RAND_COLOR, RAND_COLOR));
		second.ChangeColor(RGB(RAND_COLOR, RAND_COLOR, RAND_COLOR));
    }

    return;
}

void ModifyVelocities(Ball &first, Ball &second) {
    int coefficient1;
    int coefficient2;
    int dotP;
    POINT a;
    POINT b;
    POINT newV1;
    POINT newV2;
    // Calculate velocity of first ball
    a.x = first.horVelocity - second.horVelocity;
    a.y = first.verVelocity - second.verVelocity;
    b.x = first.center.x - second.center.x;
    b.y = first.center.y - second.center.y;
    dotP = DotProduct(a, b);
    dotP = (int) dotP / Norm(b);
    b.x *= dotP;
    b.y *= dotP;
    newV1.x = first.horVelocity - b.x;
    newV1.y = first.verVelocity - b.y;
    // Calculate velocity of second ball
    a.x = second.horVelocity - first.horVelocity;
    a.y = second.verVelocity - first.verVelocity;
    b.x = second.center.x - first.center.x;
    b.y = second.center.y - first.center.y;
    dotP = DotProduct(a, b);
    dotP = (int) dotP / Norm(b);
    b.x *= dotP;
    b.y *= dotP;
    newV2.x = second.horVelocity - b.x;
    newV2.y = second.verVelocity - b.y;
    first.horVelocity = newV1.x;
    first.verVelocity = newV1.y;
    second.horVelocity = newV2.x;
    second.verVelocity = newV2.y;
    return;
}
