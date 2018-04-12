#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <stdio.h>
#include "macros.h"
#include "declarations.h"

using namespace std;

// Declare Windows procedure
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Make the class name into a global variable
TCHAR szAppName[] = TEXT("Balls Animation");

// Vector where we store the balls
static vector< Ball > balls;

// Main function
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;               // This is the handle for our window
    MSG messages;            // Here are saved messages to the application
    WNDCLASSEX wincl;        // Data structure for the windowclass

    // The Window structure
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szAppName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = (HICON) LoadIcon (NULL, MAKEINTRESOURCE(IDR_ICON));
    wincl.hIconSm = (HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE (IDR_ICON), IMAGE_ICON, 16, 16, 0);
    wincl.hCursor = (HCURSOR) LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;                                       // No extra bytes after the window class
    wincl.cbWndExtra = 0;                                       // structure or the window instance
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    // Register the window class, and if it fails quit the program
    if (!RegisterClassEx(&wincl))
        return 0;

    // The class is registered, let's create the program
    hwnd = CreateWindowEx(0, szAppName, TEXT("WP Lab#4"), WS_OVERLAPPEDWINDOW,
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
    HBRUSH hBrush;
    PAINTSTRUCT ps;
    POINT position;
    static RECT rect;
    static HDC hdcMem;
    static HBITMAP hBitmap;
    static int timerRate = 10;
    static int nrOfBalls = 0;

    // handle the messages
    switch (message) {
        case WM_CREATE: {
            // Creation of objects
            SetTimer (hwnd, IDT_TIMER, timerRate, NULL);
            break;
        }

        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd,&rect);
            for (int i = 0; i < nrOfBalls - 1; i++)
                for (int j = i + 1; j < nrOfBalls; j++)
                    Collision(balls[i], balls[j]);

            FillRect(hdcMem, &rect, (HBRUSH) GetStockObject(WHITE_BRUSH));
            for (int i = 0; i < nrOfBalls; i++)
                balls[i].MoveBall(hdcMem, rect, hBrush);

            BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_LBUTTONDOWN: {
            // Left button of the mouse is pressed
            srand(time(NULL));
            position.x = LOWORD(lParam);
            position.y = HIWORD(lParam);
            Ball newBall(position, RAND_VELOCITY, RAND_VELOCITY, RGB(255, 0, 0));
            balls.push_back(newBall);
            nrOfBalls++;
			break;
        }

        case WM_RBUTTONDOWN: {
            // Right button of the mouse is pressed
            break;
        }

        case WM_MOUSEMOVE: {
            // Mouse has moved
			break;
        }

        case WM_LBUTTONUP: {
            // Left button of the mouse is released
			break;
        }

        case WM_RBUTTONUP: {
            // Left button of the mouse is released
			break;
        }

        case WM_SIZE: {
            if (hBitmap)
                DeleteObject(hBitmap);

            hdc = GetDC(hwnd);
            GetClientRect(hwnd,&rect);
            hdcMem = CreateCompatibleDC(hdc);
            hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            SelectObject(hdcMem, hBitmap);
            ReleaseDC(hwnd, hdc);
            break;
        }

        case WM_TIMER: {
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }

        case WM_MOUSEWHEEL: {
            if ((SHORT) HIWORD(wParam) < 0) {
                timerRate += 5;
            }else {
                timerRate -= 5;
                if (timerRate <= 0)
                    timerRate = 1;
            }

            KillTimer(hwnd, IDT_TIMER);
            SetTimer(hwnd, IDT_TIMER, timerRate, NULL);
            break;
        }

        case WM_DESTROY: {
            if (hBitmap)
                DeleteObject(hBitmap);

            DeleteDC(hdcMem);
            KillTimer(hwnd, IDT_TIMER);
            PostQuitMessage(0);       // send a WM_QUIT to the message queue
            break;
        }

        default:                      // for messages that we don't deal with
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
