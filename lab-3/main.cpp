#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "macros.h"
#include "declarations.h"

// Declare Windows procedure
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Declare DrawBezier spline
void DrawBezier(HDC hdc, POINT pnt[]);

// Make the class name into a global variable
TCHAR szClassName[] = _T("Vasile's Paint App");

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;               // This is the handle for our window
    MSG messages;            // Here are saved messages to the application
    WNDCLASSEX wincl;        // Data structure for the windowclass

    // The Window structure
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      // This function is called by windows
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof(WNDCLASSEX);

    // Use default icon and mouse-pointer
    wincl.hIcon = (HICON) LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_ICON));
    wincl.hIconSm = (HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE (IDR_ICON), IMAGE_ICON, 16, 16, 0);
    wincl.hCursor = (HCURSOR) LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;                            // No extra bytes after the window class
    wincl.cbWndExtra = 0;                            // structure or the window instance
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    // Register the window class, and if it fails quit the program
    if (!RegisterClassEx(&wincl))
        return 0;

    // The class is registered, let's create the program
    hwnd = CreateWindowEx(0, szClassName, _T("WP Lab#3"), WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                          HWND_DESKTOP, NULL, hThisInstance, NULL);

    // Make the window visible on the screen
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop. It will run until GetMessage() returns 0
    while (GetMessage(&messages, NULL, 0, 0)) {
        // Translate virtual-key messages into character messages
        TranslateMessage(&messages);
        // Send message to WindowProcedure
        DispatchMessage(&messages);
    }

    // The program return-value is 0 - The value that PostQuitMessage() gave
    return messages.wParam;
}

// This function is called by the Windows function DispatchMessage()
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // Declare variables used in window procedure
    HDC hdc;
    PAINTSTRUCT ps;
    COLORREF color = RGB(0, 0, 0);
    POINT ptClientUL;
    POINT ptClientLR;
    POINTS mouse;
    char text[10];
    static int weight = 5;
    static int cntPoints;
    static BOOL isDrawing = FALSE;
    static BOOL drawnPrev = FALSE;
    static BOOL isDrawingSpline = FALSE;
    static RECT rcClient;
    static HPEN hPen;
    static HPEN eraserPen;
    static HBRUSH hBrush;
    static POINT arrPoints[10];
    static POINTS client;
    static POINTS ptsBegin;
    static POINTS ptsEnd;
    static POINTS ptsPrev;
    static HWND penTool;
    static HWND lineTool;
    static HWND splineTool;
    static HWND rectTool;
    static HWND ellipseTool;
    static HWND eraseTool;
    static HWND weightEdit;
    static HWND weightStatic;
    static HWND colorStatic;
    static HWND redButton;
    static HWND greenButton;
    static HWND blueButton;
    static HWND fillButton;
    static HWND okButton;

    mouse = MAKEPOINTS(lParam);
    // handle the messages
    switch (message) {
        case WM_CREATE: {
            // Creation of hwnd objects
            penTool = CreateWindow(TEXT("BUTTON"), TEXT("Pen"),
                                   WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | WS_GROUP | WS_TABSTOP,
                                   0, 0, 0, 0,
                                   hwnd, (HMENU) IDC_BUTTONPEN, GetModuleHandle(NULL), NULL);

            lineTool = CreateWindow(TEXT("BUTTON"), TEXT("Line"),
                                    WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_BUTTONLINE, GetModuleHandle(NULL), NULL);

            splineTool = CreateWindow(TEXT("BUTTON"), TEXT("Spline"),
                                      WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                                      0, 0, 0, 0,
                                      hwnd, (HMENU) IDC_BUTTONSPLINE, GetModuleHandle(NULL), NULL);

            rectTool = CreateWindow(TEXT("BUTTON"), TEXT("Rect"),
                                    WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_BUTTONRECT, GetModuleHandle(NULL), NULL);

            ellipseTool = CreateWindow(TEXT("BUTTON"), TEXT("Ellipse"),
                                       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                                       0, 0, 0, 0,
                                       hwnd, (HMENU) IDC_BUTTONELLIPSE, GetModuleHandle(NULL), NULL);

            eraseTool = CreateWindow(TEXT("BUTTON"), TEXT("Eraser"),
                                    WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_BUTTONERASE, GetModuleHandle(NULL), NULL);

            weightStatic = CreateWindow(TEXT("STATIC"), TEXT("Line Weigth"),
                                        WS_VISIBLE | WS_CHILD | SS_CENTER,
                                        0, 0, 0, 0,
                                        hwnd, (HMENU) IDC_STATICWEIGHT, GetModuleHandle(NULL), NULL);

            weightEdit = CreateWindow(TEXT("EDIT"), TEXT("..."),
                                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_EDITWEIGHT, GetModuleHandle(NULL), NULL);

            colorStatic = CreateWindow(TEXT("STATIC"), TEXT("Color"),
                                        WS_VISIBLE | WS_CHILD | SS_CENTER,
                                        0, 0, 0, 0,
                                        hwnd, (HMENU) IDC_STATICCOLOR, GetModuleHandle(NULL), NULL);

            redButton = CreateWindow(TEXT("BUTTON"), TEXT("Red"),
                                     WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP | WS_TABSTOP,
                                     0, 0, 0, 0,
                                     hwnd, (HMENU) IDC_BUTTONRED, GetModuleHandle(NULL), NULL);

            greenButton = CreateWindow(TEXT("BUTTON"), TEXT("Green"),
                                       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                                       0, 0, 0, 0,
                                       hwnd, (HMENU) IDC_BUTTONGREEN, GetModuleHandle(NULL), NULL);

            blueButton = CreateWindow(TEXT("BUTTON"), TEXT("Blue"),
                                      WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                                      0, 0, 0, 0,
                                      hwnd, (HMENU) IDC_BUTTONBLUE, GetModuleHandle(NULL), NULL);

            fillButton = CreateWindow(TEXT("BUTTON"), TEXT("Fill"),
                                      WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                                      0, 0, 0, 0,
                                      hwnd, (HMENU) IDC_BUTTONFILL, GetModuleHandle(NULL), NULL);

            okButton = CreateWindow(TEXT("BUTTON"), TEXT("OK"),
                                    WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU)IDC_BUTTONOK, GetModuleHandle(NULL), NULL);

            Button_SetCheck(GetDlgItem(hwnd, IDC_BUTTONRED), BST_CHECKED);
            Button_SetCheck(GetDlgItem(hwnd, IDC_BUTTONPEN), BST_CHECKED);
            break;
        }

        case WM_LBUTTONDOWN: {
            // Left button of the mouse is pressed
            if (!Valid(mouse))
                break;

            if (Button_GetCheck(splineTool) == BST_UNCHECKED)
                isDrawingSpline = FALSE, cntPoints = 0;

            SetCapture(hwnd);
            GetClientRect(hwnd, &rcClient);
            ptClientUL.x = rcClient.left;
            ptClientUL.y = rcClient.top;
            ptClientLR.x = rcClient.right + 1;
            ptClientLR.y = rcClient.bottom + 1;
            ClientToScreen(hwnd, &ptClientUL);
            ClientToScreen(hwnd, &ptClientLR);
            SetRect(&rcClient, ptClientUL.x, ptClientUL.y, ptClientLR.x, ptClientLR.y);
            ClipCursor(&rcClient);
            ptsBegin = MAKEPOINTS(lParam);
            ptsEnd = ptsBegin;

            if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONRED)) == BST_CHECKED) {
				color = RGB(255, 0, 0);
			}
			else
            if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONGREEN)) == BST_CHECKED) {
				color = RGB(0, 255, 0);
			}
			else
			if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONBLUE)) == BST_CHECKED) {
				color = RGB(0, 0, 255);
			}

			hPen = CreatePen(PS_SOLID, weight, color);
			eraserPen = CreatePen(PS_SOLID, weight, RGB(255, 255, 255));
			hdc = GetDC(hwnd);
			SelectObject(hdc, hPen);

			if (Button_GetCheck(penTool) == BST_CHECKED) {
                DrawPen(hwnd, hdc, ptsBegin, ptsEnd);
				isDrawing = TRUE;
			}
			else
            if (Button_GetCheck(lineTool) == BST_CHECKED) {
                DrawLine(hwnd, hdc, ptsBegin, ptsEnd);
				isDrawing = TRUE;
			}
			else
			if (Button_GetCheck(rectTool) == BST_CHECKED) {
				hBrush = createBrush(hwnd, color);
				DrawRect(hwnd, hdc, hBrush, ptsBegin, ptsEnd);
				isDrawing = TRUE;
				DeleteObject(hBrush);
			}
			else
			if (Button_GetCheck(ellipseTool) == BST_CHECKED) {
				hBrush = createBrush(hwnd, color);
				DrawEllipse(hwnd, hdc, hBrush, ptsBegin, ptsEnd);
				isDrawing = TRUE;
				DeleteObject(hBrush);
			}
			else
            if (Button_GetCheck(eraseTool) == BST_CHECKED) {
                SelectObject(hdc, eraserPen);
                DrawPen(hwnd, hdc, ptsBegin, ptsEnd);
				isDrawing = TRUE;
			}
			if (Button_GetCheck(splineTool) == BST_CHECKED) {
                //DrawPen(hwnd, hdc, ptsBegin, ptsEnd);
                arrPoints[cntPoints].x = LOWORD(lParam);
                arrPoints[cntPoints].y = HIWORD(lParam);
                cntPoints++;
			}

			DeleteObject(hPen);
			ReleaseDC(hwnd, hdc);
			break;
        }

        case WM_RBUTTONDOWN: {
            // Right button of the mouse is pressed
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }

        case WM_MOUSEMOVE: {
            if (!isDrawing || !Valid(mouse))
                break;

            if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONRED)) == BST_CHECKED) {
				color = RGB(255, 0, 0);
			}
			else
            if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONGREEN)) == BST_CHECKED) {
				color = RGB(0, 255, 0);
			}
			else
			if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONBLUE)) == BST_CHECKED) {
				color = RGB(0, 0, 255);
			}

			hPen = CreatePen(PS_SOLID, weight, color);
			eraserPen = CreatePen(PS_SOLID, weight, RGB(255, 255, 255));
			hdc = GetDC(hwnd);
			SelectObject(hdc, hPen);

			if (Button_GetCheck(penTool) == BST_CHECKED) {
                ptsBegin = ptsEnd;
                ptsEnd = MAKEPOINTS(lParam);
                DrawPen(hwnd, hdc, ptsBegin, ptsEnd);
			}
			else
            if (Button_GetCheck(lineTool) == BST_CHECKED) {
                if (drawnPrev)
                    DrawLine(hwnd, hdc, ptsBegin, ptsPrev);

                ptsEnd = MAKEPOINTS(lParam);
                DrawLine(hwnd, hdc, ptsBegin, ptsEnd);
			}
			else
			if (Button_GetCheck(rectTool) == BST_CHECKED) {
				hBrush = createBrush(hwnd, color);
				if (drawnPrev)
                    DrawRect(hwnd, hdc, hBrush, ptsBegin, ptsPrev);

                ptsEnd = MAKEPOINTS(lParam);
                DrawRect(hwnd, hdc, hBrush, ptsBegin, ptsEnd);
                DeleteObject(hBrush);
			}
			else
			if (Button_GetCheck(ellipseTool) == BST_CHECKED) {
				hBrush = createBrush(hwnd, color);
				if (drawnPrev)
                    DrawEllipse(hwnd, hdc, hBrush, ptsBegin, ptsPrev);

                ptsEnd = MAKEPOINTS(lParam);
                DrawEllipse(hwnd, hdc, hBrush, ptsBegin, ptsEnd);
                DeleteObject(hBrush);
			}
			else
            if (Button_GetCheck(eraseTool) == BST_CHECKED) {
                SelectObject(hdc, eraserPen);
                ptsBegin = ptsEnd;
                ptsEnd = MAKEPOINTS(lParam);
                DrawPen(hwnd, hdc, ptsBegin, ptsEnd);
			}

            drawnPrev = TRUE;
            ptsPrev = ptsEnd;
            DeleteObject(hPen);
            ReleaseDC(hwnd, hdc);
			break;
        }

        case WM_LBUTTONUP: {
            // Left button of the mouse is released
            drawnPrev = FALSE;
            ClipCursor(NULL);
            ReleaseCapture();
            ptsEnd = MAKEPOINTS(lParam);
            isDrawing = FALSE;
			break;
        }

        case WM_COMMAND: {
            if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_EDITWEIGHT) {
                SetWindowText(weightEdit, NULL);
            }

            switch (LOWORD(wParam)) {
                case IDC_BUTTONOK: {
                    GetWindowText(weightEdit, text, 10);
                    weight = atoi(text);
                    break;
                }

                case IDC_BUTTONSPLINE: {
                    isDrawingSpline = TRUE;
                    cntPoints = 0;
                }

                default:
                    break;
            }

            break;
        }

        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONRED)) == BST_CHECKED) {
				color = RGB(255, 0, 0);
			}
			else
            if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONGREEN)) == BST_CHECKED) {
				color = RGB(0, 255, 0);
			}
			else
			if (Button_GetCheck(GetDlgItem(hwnd, IDC_BUTTONBLUE)) == BST_CHECKED) {
				color = RGB(0, 0, 255);
			}

			hPen = CreatePen(PS_SOLID, weight, color);
			SelectObject(hdc, hPen);

            if (isDrawingSpline) {
                PolyBezier(hdc, arrPoints, cntPoints);
                cntPoints = 0;
            }
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CTLCOLOREDIT: {
            if (IDC_EDITWEIGHT == GetDlgCtrlID((HWND) lParam)) {
                hBrush = CreateSolidBrush(RGB(240, 240, 240));
                SetBkMode((HDC) wParam, TRANSPARENT);
                return (INT_PTR) hBrush;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            if (IDC_STATICWEIGHT == GetDlgCtrlID((HWND) lParam) || IDC_STATICCOLOR == GetDlgCtrlID((HWND) lParam)) {
                hBrush = CreateSolidBrush(RGB(255, 255, 255));
                SetTextColor((HDC) wParam, RGB(0, 0, 0));
                SetBkMode((HDC) wParam, TRANSPARENT);
                return (INT_PTR) hBrush;
            }
            break;
        }

        case WM_SIZE: {
            client = MAKEPOINTS(lParam);
            MoveWindow(penTool, 0, 25, 50, 25, TRUE);
            MoveWindow(lineTool, 0, 50, 50, 25, TRUE);
            MoveWindow(splineTool, 0, 75, 50, 25, TRUE);
            MoveWindow(rectTool, 0, 100, 50, 25, TRUE);
            MoveWindow(ellipseTool, 0, 125, 50, 25, TRUE);
            MoveWindow(eraseTool, 0, 150, 50, 25, TRUE);
            MoveWindow(weightStatic, 50, 0, 100, 20, TRUE);
            MoveWindow(weightEdit, 150, 0, 20, 20, TRUE);
            MoveWindow(colorStatic, 200, 0, 70, 20, TRUE);
            MoveWindow(redButton, 270, 0, 70, 20, TRUE);
            MoveWindow(greenButton, 340, 0, 70, 20, TRUE);
            MoveWindow(blueButton, 410, 0, 70, 20, TRUE);
            MoveWindow(fillButton, 0, 175, 50, 25, TRUE);
            MoveWindow(okButton, 172, 0, 25, 20, TRUE);
            break;
        }

        case WM_GETMINMAXINFO: {
            LPMINMAXINFO winSize = (LPMINMAXINFO) lParam;
            winSize->ptMinTrackSize.x = 510;
            winSize->ptMinTrackSize.y = 375;
            winSize->ptMaxTrackSize.x = 830;
            winSize->ptMaxTrackSize.y = 625;
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);       // send a WM_QUIT to the message queue
            break;
        }

        default:                      // for messages that we don't deal with
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
