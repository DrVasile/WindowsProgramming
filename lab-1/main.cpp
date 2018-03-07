#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

/* Declare IDC of Controls */
#define IDC_BUTTON1 101
#define IDC_BUTTON2 102
#define IDC_STATIC1 103
#define IDC_STATIC2 104
#define IDC_EDIT1 105
#define IDC_EDIT2 106

/* Max length of the written text in edit1 */
#define MAX_LENGTH 255

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T ("Vasile's First App");

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush (RGB (100, 100, 100));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T ("WP Lab#1"),     /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               200,                 /* Windows decides the position */
               200,                 /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0)) {
        /* Translate virtual-key messages into character messages */
        TranslateMessage (&messages);
        /* Send message to WindowProcedure */
        DispatchMessage (&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc = NULL;
    RECT clientRect;
    PAINTSTRUCT ps;
    LRESULT textSize;
    HFONT fontConsolas;
    HFONT fontCorbel;
    HFONT fontArial;
    HBRUSH textBrush;
    char writtenText[MAX_LENGTH];
    static HWND button1;
    static HWND button2;
    static HWND static1;
    static HWND static2;
    static HWND edit1;
    static HWND edit2;
    static HGDIOBJ fontDefault;
    static bool textFlagRed, textFlagBlue, textFlagGreen;

    /* handle the messages */
    switch (message) {
        case WM_CREATE: {
            button1 = CreateWindow (TEXT ("BUTTON"), TEXT ("Add"),
                                    WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_BUTTON1, GetModuleHandle (NULL), NULL);

            button2 = CreateWindow (TEXT ("BUTTON"), TEXT ("Clear"),
                                    WS_VISIBLE | WS_CHILD | WS_THICKFRAME | BS_DEFPUSHBUTTON,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_BUTTON2, GetModuleHandle (NULL), NULL);

            static1 = CreateWindow (TEXT ("STATIC"), TEXT ("<< Useless Win32 GUI Application >>"),
                                    WS_CHILD | WS_VISIBLE | SS_CENTER,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_STATIC1, GetModuleHandle (NULL), NULL);

            static2 = CreateWindow (TEXT ("STATIC"), TEXT ("What is the Object-Oriented way to become wealthy ? Inheritance !!!"),
                                    WS_CHILD | WS_VISIBLE | SS_CENTER,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_STATIC2, GetModuleHandle (NULL), NULL);

            edit1 = CreateWindowEx (WS_EX_CLIENTEDGE, TEXT ("EDIT"), NULL,
                                    WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_EDIT1, GetModuleHandle (NULL), NULL);

            edit2 = CreateWindowEx (WS_EX_CLIENTEDGE, TEXT ("EDIT"), NULL,
                                    WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) IDC_EDIT1, GetModuleHandle (NULL), NULL);
            break;
        }

        case WM_PAINT: {
            hdc = BeginPaint (hwnd, &ps);
            fontCorbel = CreateFont (20, 0, 0, 0, FW_BOLD, false, false, false,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, FF_DONTCARE, "Corbel");

            fontConsolas = CreateFont (20, 0, 0, 0, FW_BOLD, false, false, false,
                                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                      DEFAULT_QUALITY, FF_DONTCARE, "Consolas");

            fontArial = CreateFont (20, 0, 0, 0, FW_DONTCARE, false, false, false,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, FF_DONTCARE, "Arial");

            SetBkMode (hdc, TRANSPARENT);
            GetClientRect (hwnd, &clientRect);
            fontDefault = GetStockObject (DEFAULT_GUI_FONT);
            SendMessage (button1, WM_SETFONT, (WPARAM) fontDefault, TRUE);
            SendMessage (button2, WM_SETFONT, (WPARAM) fontConsolas, TRUE);
            SendMessage (static2, WM_SETFONT, (WPARAM) fontDefault, TRUE);
            SendMessage (static1, WM_SETFONT, (WPARAM) fontArial, TRUE);
            SendMessage (edit2, WM_SETFONT, (WPARAM) fontDefault, TRUE);
            SendMessage (edit1, WM_SETFONT, (WPARAM) fontCorbel, TRUE);
            EndPaint (hwnd, &ps);
            break;
        }

        case WM_COMMAND: {
            switch (LOWORD (wParam)) {
                case IDC_BUTTON1: {
                    textSize = SendMessage (edit1, WM_GETTEXT, MAX_LENGTH, (LPARAM) writtenText); // text size
                    writtenText[textSize] = _T ('\0');                                            // add null character at the end
                    SendMessage (edit2, EM_REPLACESEL, 0, (LPARAM) writtenText);                  // add inputed text to edit2
                    RedrawWindow (hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    break;
                }

                case IDC_BUTTON2: {
                    SendMessage(edit2, WM_SETTEXT, (WPARAM) NULL, (LPARAM) NULL);
                    break;
                }
            }
            break;
        }

        case WM_SYSCOMMAND: {
            switch (wParam) {
                case SC_MINIMIZE: {
                    srand (time (0));
                    INT x = rand() % 544 + 1;
                    INT y = rand() % 375 + 1;
                    SetWindowPos (hwnd, HWND_TOP, x, y, 544, 375, SWP_SHOWWINDOW);
                    break;
                }

                case SC_MAXIMIZE: {
                    srand(time(NULL));
                    INT red = rand() % 255 + 1;
                    INT green = rand() % 255 + 1;
                    INT blue = rand() % 255 + 1;
                    SetClassLong (hwnd, GCL_HBRBACKGROUND, (LONG) CreateSolidBrush (RGB (red, green, blue)));
                    InvalidateRect (hwnd, NULL, TRUE);
                    break;
                 }

                 case SC_CLOSE: {
                    if (MessageBox (hwnd, "Willst du es wirklich schließen?", "Achtung !", MB_YESNO) == IDYES)
                        exit (0);
                    break;
                 }

                 default:
                     return DefWindowProc (hwnd, message, wParam, lParam);
            }
            break;
        }

        case WM_SIZE: {
            int cxCoord = LOWORD (lParam); // 544
            int cyCoord = HIWORD (lParam); // 375
            MoveWindow (button1, 20, cyCoord / 2 - 40, 70, 40, TRUE);
            MoveWindow (button2, 120, cyCoord / 2 - 40, 70, 40, TRUE);
            MoveWindow (static1, 0, 0, cxCoord, 20, TRUE);
            MoveWindow (static2, 0, cyCoord - 20, cxCoord, cyCoord, TRUE);
            MoveWindow (edit1, 20, 45, cxCoord - 40, cyCoord / 2 - 97, TRUE);
            MoveWindow (edit2, 20, cyCoord - 150, cxCoord - 40, cyCoord / 2 - 97, TRUE);
            break;
        }

        case WM_GETMINMAXINFO: {
            LPMINMAXINFO winSize = (LPMINMAXINFO) lParam;
            winSize->ptMinTrackSize.x = 510;
            winSize->ptMinTrackSize.y = 375;
            winSize->ptMaxTrackSize.x = 630;
            winSize->ptMaxTrackSize.y = 425;
            break;
        }

        case WM_CTLCOLOREDIT: {
            if(IDC_EDIT1 == GetDlgCtrlID ((HWND) lParam)) {
                textBrush = CreateSolidBrush (RGB (204, 153, 153));
                SetBkMode ((HDC) wParam, TRANSPARENT);
                return (INT_PTR) textBrush;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            if(IDC_STATIC1 == GetDlgCtrlID ((HWND) lParam)) {
                textBrush = CreateSolidBrush (RGB (104, 153, 153));
                SetTextColor ((HDC) wParam, RGB (255, 0, 0));
                SetBkMode ((HDC) wParam, TRANSPARENT);
                return (INT_PTR) textBrush;
            }
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        }

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
