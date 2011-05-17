#include <windows.h>
#include <stdio.h>
#include "dinput.h"

#include "debug.h"

//Code adapted from http://www.cs.cmu.edu/~jparise/code/joystick/

LPDIRECTINPUTDEVICE8 joystick = NULL;
LPDIRECTINPUT8 di;

static DIACTIONFORMAT g_ActionFormat;

HWND hwnd;

void _dump_joy(DIJOYSTATE* st)
{
    printf("Ax: (%d,%d,%d)\nRAx: (%d,%d,%d)\nSlider: (%d,%d)\nPov: (%d,%d,%d,%d)\n\n",
                st->lX,st->lY,st->lZ,st->lRx,st->lRy,st->lRz,
                st->rglSlider[0],st->rglSlider[1],
                st->rgdwPOV[0],st->rgdwPOV[1],st->rgdwPOV[2],st->rgdwPOV[3]
           );

    for(int i=0; i<32; i++) {
        printf("%c",st->rgbButtons[i] ? 'o' : 'x');
    }

    printf("\n");
}

void _dump_cap(DIDEVCAPS *cap)
{

    printf("dwFlags: %d\nType %X Sub: %X\ndwAxes: %d\ndwButtons: %d\ndwPOVs: %d\ndwFFSamplePeriod: %d\n\
dwFFMinTimeResolution: %d\ndwFirmwareRevision: %d\n\
dwHardwareRevision: %d\ndwFFDriverVersion: %d\n",
            cap->dwFlags,GET_DIDEVICE_TYPE(cap->dwDevType),GET_DIDEVICE_SUBTYPE(cap->dwDevType),
            cap->dwAxes,cap->dwButtons, cap->dwPOVs,
            cap->dwFFSamplePeriod, cap->dwFFMinTimeResolution, cap->dwFirmwareRevision,
            cap->dwHardwareRevision, cap->dwFFDriverVersion);

}

BOOL CALLBACK
enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    hr = di->CreateDevice(instance->guidInstance, &joystick, NULL);

    printf("Controller %s\nInstance %s\nType %X Sub: %X\n",instance->tszProductName,
            instance->tszInstanceName,
            GET_DIDEVICE_TYPE(instance->dwDevType),
            GET_DIDEVICE_SUBTYPE(instance->dwDevType));

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if (FAILED(hr)) {
        return DIENUM_CONTINUE;
    }

    return DIENUM_STOP;
}

BOOL CALLBACK
enumAxesCallback(const DIDEVICEOBJECTINSTANCE* instance, VOID* context)
{
    HWND hDlg = (HWND)context;

    DIPROPRANGE propRange;
    propRange.diph.dwSize       = sizeof(DIPROPRANGE);
    propRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    propRange.diph.dwHow        = DIPH_BYID;
    propRange.diph.dwObj        = instance->dwType;
    propRange.lMin              = -1000;
    propRange.lMax              = +1000;

    // Set the range for the axis
    if (FAILED(joystick->SetProperty(DIPROP_RANGE, &propRange.diph))) {
        return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}

HRESULT
poll(DIJOYSTATE *js)
{
    HRESULT  hr;

    if (joystick == NULL) {
        return S_OK;
    }

    // Poll the device to read the current state
    hr = joystick->Poll();
    if (FAILED(hr)) {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        hr = joystick->Acquire();
        while (hr == DIERR_INPUTLOST) {
            hr = joystick->Acquire();
        }

        // If we encounter a fatal error, return failure.
        if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED)) {
            return E_FAIL;
        }

        // If another application has control of this device, return successfully.
        // We'll just have to wait our turn to use the joystick.
        if (hr == DIERR_OTHERAPPHASPRIO) {
            return S_OK;
        }
    }

    // Get the input's device state
    if (FAILED(hr = joystick->GetDeviceState(sizeof(DIJOYSTATE), js))) {
        return hr; // The device should have been acquired during the Poll()
    }

    return S_OK;
}



int main (int argc, char const* argv[])
{

    HRESULT hr;

    HWND hwnd = CreateWindow("static", "Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                   10, 10, 200, 200, NULL, NULL, NULL, NULL);


    // Create a DirectInput device
    if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                                       IID_IDirectInput8, (VOID**)&di, NULL))) {
        HRNAME(hr);
        return 1;
    }

    // Look for the first simple joystick we can find.
    if (FAILED(hr = di->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallback,
                                    NULL, DIEDFL_ATTACHEDONLY))) {
        HRNAME(hr);
        return 1;
    }

    // Make sure we got a joystick
    if (joystick == NULL) {
        printf("Joystick not found.\n");
        return E_FAIL;
    }

    DIDEVCAPS capabilities;

    // Set the data format to "simple joystick" - a predefined data format
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    if (FAILED(hr = joystick->SetDataFormat(&c_dfDIJoystick))) {
        HRNAME(hr);
        return 1;
    }

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if (FAILED(hr = joystick->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE |
                                                  DISCL_FOREGROUND))) {
        HRNAME(hr);
        return 1;
    }

    // Determine how many axis the joystick has (so we don't error out setting
    // properties for unavailable axis)
    capabilities.dwSize = sizeof(DIDEVCAPS);
    if (FAILED(hr = joystick->GetCapabilities(&capabilities))) {
         HRNAME(hr);
        return 1;

    }

    _dump_cap(&capabilities);

    // Enumerate the axes of the joyctick and set the range of each axis. Note:
    // we could just use the defaults, but we're just trying to show an example
    // of enumerating device objects (axes, buttons, etc.).
    if (FAILED(hr = joystick->EnumObjects(enumAxesCallback, NULL, DIDFT_AXIS))) {
        HRNAME(hr);
        return 1;
    }

    DIJOYSTATE state;

    // wait for any key
    bool pressed = false;
    printf("Press any joystick key\n");
    while ( !poll(&state) && !pressed ) {
        for (int i=0; i < 32 && !pressed; i++) {
            pressed = pressed || state.rgbButtons[i];
        }
    }

    //wait for input and dump
    while ( !poll(&state) ) {
        printf("-------------\n");
        _dump_joy(&state);
        printf("-------------\n");
        Sleep(100);

    }

    if (joystick) {
        joystick->Unacquire();
    }


    return 0;
}

