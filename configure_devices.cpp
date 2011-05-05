#include <windows.h>
#include "dinput.h"
#include <stdio.h>

#include "debug.h"

#include "action_mapping.h"

static DIACTIONFORMAT g_ActionFormat;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch(iMsg) {
        case WM_DEVICECHANGE://DBT_DEVICEARRIVAL:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE:
            return 0;

        case WM_PAINT:
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd,iMsg,wParam,lParam);
}

int WINAPI WinMain( HINSTANCE hCopy, HINSTANCE hCopyAnt, PSTR szCmd, int iShow)
{
    HRESULT hr;

    DIACTIONFORMAT af;

    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hCopy;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.lpszClassName = "wintest";
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    RegisterClassEx(&wndclass);

    HWND hwnd = CreateWindow("wintest", "Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                   10, 10, 200, 200, NULL, NULL, hCopy, NULL);

    LPDIRECTINPUT8 lpdi;

    hr=DirectInput8Create
       (GetModuleHandle (NULL), DIRECTINPUT_VERSION, IID_IDirectInput8A,
       (VOID**)&lpdi, NULL);

    HRNAME(hr);

    DICONFIGUREDEVICESPARAMS dicdp;

    memset(&g_ActionFormat, 0, sizeof(g_ActionFormat));
    g_ActionFormat.dwSize = sizeof(g_ActionFormat);
    g_ActionFormat.dwActionSize = sizeof(DIACTION);
    g_ActionFormat.dwDataSize = 4 * sizeof(g_commands) / sizeof(g_commands[0]);
    g_ActionFormat.dwNumActions = sizeof(g_commands) / sizeof(g_commands[0]);
    g_ActionFormat.rgoAction = g_commands;
    g_ActionFormat.guidActionMap = ACTIONMAP_GUID;
    g_ActionFormat.dwGenre = DIVIRTUAL_DRIVING_RACE;

    memset(&dicdp, 0, sizeof(dicdp));
    dicdp.dwSize = sizeof(dicdp);
    dicdp.lptszUserNames = NULL;
    dicdp.dwcFormats = 1;
    dicdp.lprgFormats = &af;

    hr = lpdi->ConfigureDevices(NULL,&dicdp,0,0);

    HRNAME(hr);

    ShowWindow(hwnd,iShow);
    UpdateWindow(hwnd);

    MSG msg;

    while ( GetMessage(&msg, NULL, 0, 0) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;

}
