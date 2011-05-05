#include <windows.h>
#include "dinput.h"
#include <stdio.h>

#include "action_mapping.h"
#include "debug.h"

LPDIRECTINPUT8 gp_DI=NULL;

static DIACTIONFORMAT g_ActionFormat;

BOOL CALLBACK EnumDevicesCallback //Callback function that receive DirectInput devices
(   LPCDIDEVICEINSTANCE lpddi, //Adress of structure describing device instance
    LPDIRECTINPUTDEVICE8 lpdid, //Pointer to device interface
    DWORD dwFlags, //Flags telling why being enumerated
    DWORD dwRemaining, //Number of devices remaining to be enumerated
    LPVOID pvRef) //Application defined value. Always NULL here
{
      HRESULT hr;

      hr = lpdid->BuildActionMap(&g_ActionFormat,NULL,0);

      HRNAME(hr);

      return DIENUM_STOP;
}

int main()
{
    HRESULT hr;

    hr=DirectInput8Create
       (GetModuleHandle (NULL), DIRECTINPUT_VERSION, IID_IDirectInput8A,
       (VOID**)&gp_DI, NULL);

    memset(&g_ActionFormat, 0, sizeof(g_ActionFormat));
    g_ActionFormat.dwSize = sizeof(g_ActionFormat);
    g_ActionFormat.dwActionSize = sizeof(DIACTION);
    g_ActionFormat.dwDataSize = 4 * sizeof(g_commands) / sizeof(g_commands[0]);
    g_ActionFormat.dwNumActions = sizeof(g_commands) / sizeof(g_commands[0]);
    g_ActionFormat.rgoAction = g_commands;
    g_ActionFormat.guidActionMap = ACTIONMAP_GUID;
    g_ActionFormat.dwGenre = DIVIRTUAL_DRIVING_RACE;

    hr=gp_DI->EnumDevicesBySemantics
       (NULL, &g_ActionFormat, EnumDevicesCallback, NULL, DIEDBSFL_ATTACHEDONLY);

    HRNAME(hr);

    return 0;
}

