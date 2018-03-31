#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include "macros.h"

/* Declare Windows procedure */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/* Declare Dialog procedure */
BOOL CALLBACK DialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

/* Declare Draw Rectangle procedure */
void DrawRect(HWND hwnd);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T ("Second Laboratory");

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;                        /* This function is called by windows */
    wincl.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;         /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE (IDI_ICON));
    wincl.hIconSm = (HICON) LoadImage (GetModuleHandle (NULL), MAKEINTRESOURCE (IDI_ICON), IMAGE_ICON, 16, 16, 0);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDM_MENU);
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
               _T ("WP Lab#2"),     /* Title Text */
               WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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
    HDC hdc;
    SCROLLINFO si;
    TEXTMETRIC tm;
    LRESULT textSize;
    char writtenText[MAX_LENGTH];
    static HWND listBox;
    static HWND addButton;
    static HWND editBox;
    static HWND scrollBar;
    static int pos = 0;
    static int xMin = 0;
    static int xMax = 255;
    static int maxLength = 20;
    static int cxCoord;
    static int cyCoord;
    static HBRUSH editBrush = (HBRUSH) CreateSolidBrush (RGB (255, 255, 255));

    static int cxChar, cxCaps, cyChar, cxClient, cyClient;
    int iVertPos;

    /* handle the messages */
    switch (message) {
        case WM_CREATE: {
            hdc = GetDC (hwnd);
            GetTextMetrics (hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;
            ReleaseDC (hwnd, hdc);

            listBox = CreateWindowEx (WS_EX_CLIENTEDGE, TEXT ("LISTBOX"), TEXT (""),
                             WS_VISIBLE | WS_CHILD | LBS_NOTIFY,
                             20, 45, 150, 200,
                             hwnd, (HMENU) IDC_LISTBOX, GetModuleHandle (NULL), NULL );

            addButton = CreateWindow (TEXT ("BUTTON"), TEXT ("Add"),
                             WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                             180, 10, 60, 25,
                             hwnd, (HMENU) IDC_ADDBUTTON, GetModuleHandle (NULL), NULL);

            editBox = CreateWindowEx (WS_EX_CLIENTEDGE, TEXT ("EDIT"), TEXT (""),
                             WS_VISIBLE | WS_CHILD,
                             20, 10, 150, 25,
                             hwnd, (HMENU) IDC_EDITBOX, GetModuleHandle (NULL), NULL);

            scrollBar = CreateWindow (TEXT ("SCROLLBAR"), TEXT (""),
                             WS_VISIBLE | WS_CHILD | SBS_HORZ,
                             250, 10, 220, 25,
                             hwnd, (HMENU) IDC_SCROLLBAR, GetModuleHandle (NULL), NULL);

            SendMessage (editBox, EM_LIMITTEXT, WPARAM (maxLength), (LPARAM) NULL);
            SetScrollRange (scrollBar, SB_CTL, xMin, xMax, FALSE);
            SetScrollPos (scrollBar, SB_CTL, pos, TRUE);
            RegisterHotKey(hwnd, HK_EXIT, MOD_CONTROL, 0x57);
            RegisterHotKey(hwnd, HK_HELP, MOD_CONTROL, 0x48);
            break;
        }

        case WM_VSCROLL: {
            si.cbSize = sizeof (si) ;
            si.fMask = SIF_ALL ;
            GetScrollInfo (hwnd, SB_VERT, &si) ;
            iVertPos = si.nPos ;

            switch (LOWORD (wParam)) {
                case SB_TOP:
                    si.nPos = si.nMin;
                    break;

                case SB_BOTTOM:
                    si.nPos = si.nMax;
                    break;

                case SB_LINEUP:
                    si.nPos -= 1;
                    break;

                case SB_LINEDOWN:
                    si.nPos += 1;
                    break;

                case SB_PAGEUP:
                    si.nPos -= si.nPage;
                    break;

                case SB_PAGEDOWN:
                    si.nPos += si.nPage;
                    break;

                case SB_THUMBTRACK:
                    si.nPos = si.nTrackPos;
                    break;

                default:
                    break;
            }

            si.fMask = SIF_POS;
            SetScrollInfo (hwnd, SB_VERT, &si, TRUE);
            GetScrollInfo (hwnd, SB_VERT, &si);
            if (si.nPos != iVertPos) {
                ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
                UpdateWindow(hwnd);
            }
            break;
        }

        case WM_HSCROLL: {
            if ((HWND) lParam != scrollBar)
                break;

            switch (LOWORD (wParam)) {
                case SB_LINELEFT:
                    pos -= 1;
                    DrawRect(hwnd);
                    break;

                case SB_LINERIGHT:
                    pos += 1;
                    DrawRect(hwnd);
                    break;

                case SB_PAGELEFT:
                    pos -= 10;
                    DrawRect(hwnd);
                    break;

                case SB_PAGERIGHT:
                    pos += 10;
                    DrawRect(hwnd);
                    break;

                case SB_TOP:
                    pos = xMin;
                    DrawRect(hwnd);
                    break;

                case SB_BOTTOM:
                    pos = xMax;
                    DrawRect(hwnd);
                    break;

                case SB_THUMBPOSITION:
                    DrawRect(hwnd);
                    break;

                case SB_THUMBTRACK:
                    pos = HIWORD (wParam);
                    DrawRect(hwnd);
                    break;

                default:
                    break;
            }

            SetScrollPos (scrollBar, SB_CTL, pos, TRUE);
            if (pos == xMax) {
                DrawRect(hwnd);
                EnableScrollBar(scrollBar, SB_CTL, ESB_DISABLE_RIGHT);
            }

            if(pos == xMin) {
                DrawRect(hwnd);
                EnableScrollBar(scrollBar, SB_CTL, ESB_DISABLE_LEFT);
            }

            InvalidateRect(editBox, NULL, TRUE);
            break;
        }

        case WM_CONTEXTMENU: {
            if ((HWND) wParam == listBox)
            {
                if (MessageBox (hwnd, TEXT ("Remove selected item?"), TEXT ("Warning"), MB_YESNO) == IDYES) {
                    int id = SendMessage (GetDlgItem (hwnd, IDC_LISTBOX), LB_GETCURSEL, 0, 0);
                    SendMessage (GetDlgItem (hwnd, IDC_LISTBOX), LB_DELETESTRING, id, 0);
                }
            }
            break;
        }

        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_DELETE:
                    DialogBox (GetModuleHandle (NULL), MAKEINTRESOURCE (IDD_DIALOG), hwnd, DialogProcedure);
                    break;

                default:
                    break;
            }
        }

        case WM_HOTKEY: {
            switch (wParam) {
                case HK_EXIT: {
                    PostQuitMessage(0);
                    break;
                }

                case HK_HELP: {
                    MessageBox(hwnd, "Can't help you!!", "Sorry", MB_OK | MB_ICONINFORMATION);
                    break;
                }

                default:
                    break;
            }
            break;
        }

        case WM_COMMAND: {
            switch (LOWORD (wParam)) {
                case IDC_ADDBUTTON: {
                    textSize = SendMessage (editBox, WM_GETTEXT, MAX_LENGTH, (LPARAM) writtenText); // text size
                    writtenText[textSize] = _T ('\0');                                              // add null character at the end
                    SendMessage (listBox, LB_ADDSTRING, 0, (LPARAM) writtenText);                    // add inputed text to edit2
                    RedrawWindow (hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    SendMessage (editBox, WM_SETTEXT, 0, (LPARAM) NULL);
                    break;
                }

                case ID_FILE_EXIT: {
                    exit(0);
                    break;
                }

                case ID_RANDOM_MOVE: {
                    srand (time (0));
                    INT x = rand() % 544 + 1;
                    INT y = rand() % 375 + 1;
                    SetWindowPos (hwnd, HWND_TOP, x, y, 544, 375, SWP_SHOWWINDOW);
                    break;
                }

                case ID_RANDOM_COLOR: {
                    srand(time(NULL));
                    INT red = rand() % 255 + 1;
                    INT green = rand() % 255 + 1;
                    INT blue = rand() % 255 + 1;
                    SetClassLong (hwnd, GCL_HBRBACKGROUND, (LONG) CreateSolidBrush (RGB (red, green, blue)));
                    InvalidateRect (hwnd, NULL, TRUE);
                    break;
                }

                case ID_ABOUT:{
                    MessageBox(hwnd, "Created with love by DrumeaVasile !!", "ABC", MB_OK | MB_ICONINFORMATION);
                    break;
                }
            }
            break;
        }

        case WM_SIZE: {
            cxClient = LOWORD (lParam) ;
            cyClient = HIWORD (lParam) ;
            // Set vertical scroll bar range and page size
            si.cbSize = sizeof (si) ;
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = NUMLINES - 1;
            si.nPage = cyClient / cyChar;
            SetScrollInfo (hwnd, SB_VERT, &si, TRUE);

            cxCoord = LOWORD (lParam); // 544
            cyCoord = HIWORD (lParam); // 375
            MoveWindow (listBox, 25, 50, cxCoord - cxCoord / 9, 210, TRUE);
            MoveWindow (addButton, cxCoord - cxCoord / 12 - cxCoord / 20, 20, 50, 25, TRUE);
            MoveWindow (editBox, 25, 20, cxCoord - cxCoord / 12 - cxCoord / 6, 25, TRUE);
            MoveWindow (scrollBar, 25, 270, cxCoord - cxCoord / 10, 25, TRUE);
            break;
        }

        case WM_GETMINMAXINFO: {
            LPMINMAXINFO winSize = (LPMINMAXINFO) lParam;
            winSize->ptMinTrackSize.x = 300;
            winSize->ptMinTrackSize.y = 175;
            winSize->ptMaxTrackSize.x = 630;
            winSize->ptMaxTrackSize.y = 425;
            break;
        }

        case WM_CTLCOLOREDIT: {
            if(IDC_EDITBOX == GetDlgCtrlID ((HWND) lParam)) {
                SetTextColor ((HDC) wParam, RGB (pos + 1, 0, 255 - pos));
                SetBkMode ((HDC) wParam, TRANSPARENT);
                return (INT_PTR) editBrush;
            }
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        }

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

/*  This function is called when we use dialog boxes  */
BOOL CALLBACK DialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND: {
            switch (LOWORD (wParam)) {
                case IDOK:
                    EndDialog (hwnd, IDOK);
                    break;

                case IDCANCEL:
                    EndDialog (hwnd, IDCANCEL);
                    break;
            }
            break;
        }

        default:
            return FALSE;
    }
    return TRUE;
}

void DrawRect (HWND hwnd){
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    GetClientRect (hwnd, &rect);

    if (rect.bottom == 0)
        return;

    hdc = BeginPaint(hwnd, &ps);
    SetBkMode (hdc, TRANSPARENT);
    INT red = rand() % 255 + 1;
    INT green = rand() % 255 + 1;
    INT blue = rand() % 255 + 1;
    SetClassLong (hwnd, GCL_HBRBACKGROUND, (LONG) CreateSolidBrush (RGB (red, green, blue)));
    InvalidateRect (hwnd, NULL, TRUE);
    EndPaint (hwnd, &ps);

    return;
}

