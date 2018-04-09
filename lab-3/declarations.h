#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

void DrawPen(HWND hwnd, HDC hdc, POINTS ptsBegin, POINTS ptsEnd);
void DrawLine(HWND hwnd, HDC hdc, POINTS ptsBegin, POINTS ptsEnd);
void DrawSpline(HWND hwnd, HDC hdc, POINTS ptsBegin, POINTS ptsEnd);
void DrawRect(HWND hwnd, HDC hdc, HBRUSH hBrush, POINTS ptsBegin, POINTS ptsEnd);
void DrawEllipse(HWND hwnd, HDC hdc, HBRUSH hBrush, POINTS ptsBegin, POINTS ptsEnd);
bool Valid(POINTS mouse);
HBRUSH createBrush(HWND hwnd, COLORREF color);
