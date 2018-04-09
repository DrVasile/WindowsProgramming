#include "declarations.h"
#include "macros.h"

void DrawPen(HWND hwnd, HDC hdc, POINTS ptsBegin, POINTS ptsEnd) {
    MoveToEx(hdc, ptsBegin.x, ptsBegin.y, NULL);
    LineTo(hdc, ptsEnd.x, ptsEnd.y);
    return;
}

void DrawLine(HWND hwnd, HDC hdc, POINTS ptsBegin, POINTS ptsEnd) {
    SetROP2(hdc, R2_NOTXORPEN);
    MoveToEx(hdc, ptsBegin.x, ptsBegin.y, NULL);
    LineTo(hdc, ptsEnd.x, ptsEnd.y);
    return;
}

void DrawSpline(HWND hwnd, HDC hdc, POINTS ptsBegin, POINTS ptsEnd) {
    SetROP2(hdc, R2_NOTXORPEN);
    MoveToEx(hdc, ptsBegin.x, ptsBegin.y, NULL);
    LineTo(hdc, ptsEnd.x, ptsEnd.y);
    return;
}

void DrawRect(HWND hwnd, HDC hdc, HBRUSH hBrush, POINTS ptsBegin, POINTS ptsEnd) {
    SetROP2(hdc, R2_NOTXORPEN);
    SelectObject(hdc, hBrush);
    Rectangle(hdc, ptsBegin.x, ptsBegin.y, ptsEnd.x, ptsEnd.y);
    return;
}

void DrawEllipse(HWND hwnd, HDC hdc, HBRUSH hBrush, POINTS ptsBegin, POINTS ptsEnd) {
    SetROP2(hdc, R2_NOTXORPEN);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, ptsBegin.x, ptsBegin.y, ptsEnd.x, ptsEnd.y);
    return;
}

bool Valid(POINTS mouse) {
    return (mouse.x > 50 && mouse.y > 20);
}

HBRUSH createBrush(HWND hwnd, COLORREF color) {
    if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONFILL)) == BST_CHECKED) {
        return (HBRUSH) CreateSolidBrush(color);
    }
    else {
        return (HBRUSH) GetStockObject(NULL_BRUSH);
    }
}
