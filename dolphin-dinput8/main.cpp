// Original code and license available at:
// http://code.google.com/p/dolphin-emu/

#include <windows.h>
#include "definitions.h"

#include "MultiDI.h"
#include "DIHandler.h"

#include <stdio.h>

CDIHandler g_diHandler;
SPADStatus g_PadStatus;

void getControllerStatus(SPADStatus* _pPADStatus);

void initializeInput(HWND _hParent)
{
	g_diHandler.InitInput(_hParent);

	EnableWindow(_hParent, FALSE);
	g_diHandler.ConfigInput();
	EnableWindow(_hParent, TRUE);
	SetForegroundWindow(_hParent);

}

void getControllerStatus(SPADStatus* _pPADStatus)
{
	//get keys from dinput
	g_diHandler.UpdateInput();

    // we just support pad 0 at the time
	const SControllerInput& ri = g_diHandler.GetControllerInput(0);

    printf("--------------------\n");
    printf("Buttons S:%d A:%d B:%d X:%d Y:%d Z:%d cLR:%f cUP:%f L:%f R:%f\n",
    ri.bButtonStart, ri.bButtonA, ri.bButtonB,ri.bButtonX,ri.bButtonY,ri.bButtonZ,
    ri.fDPadLR, ri.fDPadUP, ri.fTriggerL, ri.fTriggerR );

    printf("<-: %f /\\: %f \\/: %f ->: %f\n", ri.fMainLR,ri.fMainUP,ri.fCPadLR,ri.fCPadUP);

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch(iMsg) {
        case WM_CREATE:
            return 0;

        case WM_PAINT:
            ValidateRect(hwnd,NULL);
            return 0;

        case WM_DESTROY:
            g_diHandler.CleanupDirectInput();
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd,iMsg,wParam,lParam);
}

DWORD WINAPI ThreadFunc( LPVOID lpParam )
{
    while (true) {
        // update every half second
        Sleep(500);
        getControllerStatus(&g_PadStatus);
    }
}

void createCustomWindow(HINSTANCE hInstance, HWND *hwndOut)
{
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.hInstance = hInstance;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.hbrBackground = (HBRUSH) (COLOR_BACKGROUND+1);
    wndclass.lpszMenuName = NULL;

    wndclass.lpszClassName = "wintest";

    if(!RegisterClass(&wndclass)) {
        printf("Failed to create window\n");
        exit(0);
    }

    *hwndOut = CreateWindow("wintest", "Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                   10, 10, 200, 200, NULL, NULL, hInstance, NULL);

}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hCopy, PSTR szCmd, int iShow)
{
    MSG msg;
    DWORD dwThreadId;
    HANDLE hThread;
    HWND hwnd;

    createCustomWindow(hInstance,&hwnd);

    initializeInput(hwnd);

    hThread = CreateThread( NULL, 0, ThreadFunc, NULL, 0, &dwThreadId);

    if (hThread == NULL) {
      printf("Failed Creating thread\n");
      exit(0);
    }

    ShowWindow(hwnd,iShow);
    UpdateWindow(hwnd);

    while ( GetMessage(&msg, NULL, 0, 0) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
