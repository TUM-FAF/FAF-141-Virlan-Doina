#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <windows.h>

#define BUTTON1 1
#define BUTTON2 2
#define INPUTTEXT 3
#define UPDATE_BTN 4



/*  Declare Windows procedure  */
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hinstance, HINSTANCE hPrevInstance,
                     LPSTR lpszArgument, int iCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG msg;                 /* Here messages to the application are saved */
    WNDCLASSEX wndclass;     /* Data structure for the windowclass */
    char szAppName[ ] = "WindowsApp";  /* Class' name */

    /* The Window structure */
    wndclass.cbSize = sizeof (WNDCLASSEX);     /* cbSize represents the structure's dimension*/
    wndclass.style = CS_HREDRAW | CS_VREDRAW;  /* Redraw the window when size is modified */
    wndclass.lpfnWndProc = WndProc;            /* For the window procedure is responsible the function WndProc */
    wndclass.cbClsExtra = 0;                   /* No extra bytes after the window class */
    wndclass.cbWndExtra = 0;                   /* Structure or the window instance */
    wndclass.hInstance = hinstance;            /* Handle variable of the instance */


    /* Use default icon and mouse-pointer*/
    wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);

    /* Use Windows's white colour as the background of the window */
    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);

    wndclass.lpszMenuName = NULL;                 /* No menu */
    wndclass.lpszClassName = szAppName;           /* Class' name */


    /* Register the window class */
    RegisterClassEx (&wndclass);


    /* The class is registered, let's create the program*/
    hwnd = CreateWindow (
            szAppName,            /* Window's class's name */
            "Lab#1WP",             /* Window's title */
            WS_OVERLAPPEDWINDOW,  /* Window's style */
            CW_USEDEFAULT,        /* Initial position on x axis */
            CW_USEDEFAULT,        /* Initial position on y axis */
            500,        /* Initial dimension on x axis */
            400,        /* Initial dimension on y axis */
            NULL,                 /* Handle variable of base window */
            NULL,                 /* Handle variable of the menu */
            hinstance,            /* Instance's handle variable */
            NULL                  /* Create's parameters */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, iCmdShow);
    /* Updating window (redrawing if necessary) */
    UpdateWindow (hwnd);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&msg, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&msg);
        /* Send message to WndProc */
        DispatchMessage(&msg);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return msg.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg,
                                WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hProgramInstance;
    HWND hwndInputText1, hwndInputText2;
    int iTextLength;
    char* szText;
    switch (iMsg)                  /* handle the messages */
    {
        HDC hdc;
        PAINTSTRUCT ps1;
        RECT rect;
        static int cxClient, cyClient;

        case WM_CREATE:
            CreateWindow(TEXT("button"),
                         TEXT("Send"),
                         WS_VISIBLE | WS_CHILD,
                         100, cyClient+300, 90, 30,
                         hwnd, (HMENU) BUTTON1,
                         NULL, NULL);
            CreateWindow(TEXT("button"),
                         TEXT("Cancel"),
                         WS_VISIBLE | WS_CHILD,
                         350, cyClient +300, 90, 30,
                         hwnd, (HMENU) BUTTON2,
                         NULL, NULL);
            hwndInputText2 = CreateWindowEx((DWORD)NULL,
                                            TEXT("Edit"),
                                            TEXT("Done with Pride and Prejudice by Doina Virlan!"),
                                            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
                                            100, 32, 350, 250,
                                            hwnd, (HMENU) INPUTTEXT,
                                            hProgramInstance, NULL);
            hwndInputText1 = CreateWindowEx((DWORD)NULL,
                                            TEXT("Edit"),
                                            TEXT("doinavirlan@gmail.com"),
                                            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
                                            60, 0, 350, 25,
                                            hwnd, (HMENU) INPUTTEXT,
                                            hProgramInstance, NULL);

            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = LOWORD(lParam);
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case BUTTON1:
                    MessageBox(0, " Message was sent successfully  ", " INFO ", MB_OK | MB_ICONINFORMATION);
                    break;
                case BUTTON2:
                    PostQuitMessage (0);
                    break;
                case UPDATE_BTN:
                    iTextLength = SendMessage(hwndInputText2, WM_GETTEXTLENGTH, 0, 0);
                    szText = (char*)malloc(iTextLength + 1);
                    SendMessage(hwndInputText2, WM_GETTEXT, iTextLength +1, (LPARAM)szText);
                    free(szText);
                    break;
            }
            break;
        case WM_GETMINMAXINFO:      /* Setting minimal size of the window */
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 458;
            ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 356;
            break;
        case WM_SYSCOMMAND:
            switch (wParam)
            {
                case SC_MINIMIZE:   /* Window is moved tot the top left corner of the screen */
                    SetWindowPos(hwnd, HWND_TOP,
                                 0, 0, 0, 0,
                                 SWP_NOSIZE
                                );
                    return MessageBox(NULL,  /* Pops up a new window with a message */
                                      TEXT("The window was moved to the top left conner of the screen"),
                                      TEXT("Moved"), MB_OK);
                    break;
                case SC_MAXIMIZE:    /* Close the window */
                    PostQuitMessage (0);
                    break;
                case SC_CLOSE:    /* Playing a sound */
                    return MessageBeep(MB_ICONERROR);
                    break;
            default:
            return DefWindowProc (hwnd, iMsg, wParam, lParam);
            }
        case WM_PAINT:      /* Place a text in the center of the window */
            hdc = BeginPaint(hwnd, &ps1);
            GetClientRect (hwnd, &rect);
            DrawText(hdc, " Send to :\n\n The Subject:",
                     -1, &rect, DT_LEFT);
            EndPaint(hwnd,&ps1);


        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, iMsg, wParam, lParam);
    }

    return 0;
}
