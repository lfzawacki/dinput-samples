#include <windows.h>
#include "dinput.h"
#include <stdio.h>

#include "debug.h"

LPDIRECTINPUTDEVICE8 gDevices[20];
int numDevices = 0;

void _dump_DIDEVICEINSTANCE(const DIDEVICEINSTANCE* ins)
{
    printf("\
---\nname: %s\nprodName: %s\nsize: %x\nguidId: %s\nguidProd: %s\n\
type: %X sub: %X\nguidFF: %s\nUsagePage %d\nUsage %d\n",
        ins->tszProductName,
        ins->tszInstanceName,
        ins->dwSize,
        debugstr_guid(&ins->guidInstance),
        debugstr_guid(&ins->guidProduct),
        GET_DIDEVICE_TYPE(ins->dwDevType),
        GET_DIDEVICE_SUBTYPE(ins->dwDevType),
        debugstr_guid(&ins->guidFFDriver),
        ins->wUsagePage,
        ins->wUsage);
}

BOOL CALLBACK
enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    HRESULT hr;

    LPDIRECTINPUT8 lpdi = (LPDIRECTINPUT8) context;

    hr = lpdi->CreateDevice(instance->guidInstance, &gDevices[numDevices], NULL);

    if(SUCCEEDED(hr)) {
        numDevices++;
        _dump_DIDEVICEINSTANCE(instance);
    } else {
        HRNAME(hr);
    }

    return DIENUM_CONTINUE;
}

int WINAPI WinMain( HINSTANCE hCopy, HINSTANCE hCopyAnt, PSTR szCmd, int iShow)
{

    HRESULT hr;
    LPDIRECTINPUT8 lpdi = NULL;

    hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                        IID_IDirectInput8A, (void**)&lpdi, NULL);

    HRNAME(hr);

    hr = lpdi->EnumDevices(DI8DEVCLASS_ALL, enumCallback, lpdi, DIEDFL_ALLDEVICES);

    return 0;
}
