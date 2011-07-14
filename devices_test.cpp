#include "dinput.h"
#include <windows.h>
#include <stdio.h>
#include "debug.h"

LPDIRECTINPUT8 gp_DI=NULL;

static DIACTIONFORMAT g_ActionFormat;

static DIACTION g_commands_driving[]=
{
  {0,   DIAXIS_DRIVINGR_STEER,             0, "Steer",      },
  {1,   DIAXIS_DRIVINGR_ACCELERATE,        0, "Accelerate", },
  {2,   DIAXIS_DRIVINGR_BRAKE,             0, "Brake",      },
  {3,   DIBUTTON_DRIVINGR_SHIFTUP,         0, "Upshift",    },
  {4,   DIBUTTON_DRIVINGR_SHIFTDOWN,       0, "DownShift",  },
  {5,   DIBUTTON_DRIVINGR_VIEW,            0, "Change View",},
  {6,   DIBUTTON_DRIVINGR_MENU,            0, "Menu"       ,},
  {16, DIAXIS_DRIVINGR_ACCEL_AND_BRAKE,    0, "Accel/Brake",},
  {17, DIHATSWITCH_DRIVINGR_GLANCE,        0, "Glance",},
  {18, DIBUTTON_DRIVINGR_BRAKE,            0, "Brake",},
  {19, DIBUTTON_DRIVINGR_DASHBOARD,        0, "Dashboard",},
  {20, DIBUTTON_DRIVINGR_AIDS,             0, "Aids??",},
  {21, DIBUTTON_DRIVINGR_MAP,              0, "Map",},
  {22, DIBUTTON_DRIVINGR_BOOST,            0, "Boost",},
  {23, DIBUTTON_DRIVINGR_PIT,              0, "Pit",},
  {24, DIBUTTON_DRIVINGR_ACCELERATE_LINK,  0, "Accel",},
  {25, DIBUTTON_DRIVINGR_STEER_LEFT_LINK,  0, "Steer left",},
  {26, DIBUTTON_DRIVINGR_STEER_RIGHT_LINK, 0, "Steer right",},
  {27, DIBUTTON_DRIVINGR_GLANCE_LEFT_LINK, 0, "Glance left",},
  {28, DIBUTTON_DRIVINGR_GLANCE_RIGHT_LINK,0, "Glance right",},
  {29, DIBUTTON_DRIVINGR_DEVICE,           0, "Device",},
  {30, DIBUTTON_DRIVINGR_PAUSE,            0, "Pause",}
};

static DIACTION g_commands_space[]=
{
  {0, DIAXIS_SPACESIM_LATERAL,             0, "Steer",      },
  {1, DIAXIS_SPACESIM_MOVE,                0, "Accelerate", },
  {2, DIAXIS_SPACESIM_THROTTLE,            0, "Brake",      },
  {3, DIBUTTON_SPACESIM_FIRE,              0, "Fire",    },
  {4, DIBUTTON_SPACESIM_WEAPONS,           0, "Weapon",  },
  {5, DIBUTTON_SPACESIM_TARGET,            0, "Target",},
  {6, DIBUTTON_SPACESIM_MENU,              0, "Menu"       ,},
  {16,DIHATSWITCH_SPACESIM_GLANCE,         0, "Glance",},
  {17, DIAXIS_SPACESIM_CLIMB,              0, "Climb",},
  {18, DIAXIS_SPACESIM_ROTATE,             0, "Rotate",},
  {19, DIBUTTON_SPACESIM_VIEW,             0, "View",},
  {20, DIBUTTON_SPACESIM_DISPLAY,          0, "Display",},
  {21, DIBUTTON_SPACESIM_RAISE,            0, "Raise",},
  {22, DIBUTTON_SPACESIM_LOWER,            0, "Lowe",},
  {23, DIBUTTON_SPACESIM_GEAR,             0, "Gear",},
  {24, DIBUTTON_SPACESIM_FIRESECONDARY,    0, "Fire 2",},
  {25, DIBUTTON_SPACESIM_LEFT_LINK,        0, "Left",},
  {26, DIBUTTON_SPACESIM_RIGHT_LINK,       0, "Right",},
  {27, DIBUTTON_SPACESIM_FORWARD_LINK,     0, "Forward",},
  {28, DIBUTTON_SPACESIM_BACKWARD_LINK,    0, "Backward",},
  {29, DIBUTTON_SPACESIM_FASTER_LINK,      0, "Faster",},
  {30, DIBUTTON_SPACESIM_SLOWER_LINK,      0, "Slower",},
  {31, DIBUTTON_SPACESIM_TURN_LEFT_LINK,   0, "Turn left",},
  {32, DIBUTTON_SPACESIM_TURN_RIGHT_LINK,  0, "Turn right",},
  {33, DIBUTTON_SPACESIM_GLANCE_LEFT_LINK, 0, "Glance left",},
  {34, DIBUTTON_SPACESIM_GLANCE_RIGHT_LINK,0, "Glance left",},
  {35, DIBUTTON_SPACESIM_GLANCE_UP_LINK,   0, "Glance up",},
  {36, DIBUTTON_SPACESIM_GLANCE_DOWN_LINK, 0, "Glance down",},
  {37, DIBUTTON_SPACESIM_DEVICE,           0, "Device",},
  {38, DIBUTTON_SPACESIM_PAUSE,            0, "Pause",}
};

static DIACTION g_commands_fps[]=
{
  {0,  DIAXIS_FPS_ROTATE,             0, "Rotate",      },
  {1,  DIAXIS_FPS_MOVE,               0, "Move", },
  {2,  DIBUTTON_FPS_FIRE,             0, "Fire",      },
  {3,  DIBUTTON_FPS_WEAPONS,          0, "Weapon",    },
  {4,  DIBUTTON_FPS_APPLY,            0, "Use",  },
  {5,  DIBUTTON_FPS_SELECT,           0, "Select",},
  {6,  DIBUTTON_FPS_CROUCH,           0, "Crouch",    },
  {7,  DIBUTTON_FPS_JUMP,             0, "Jump",  },
  {8,  DIAXIS_FPS_LOOKUPDOWN,         0, "Look up/down",},
  {9,  DIBUTTON_FPS_STRAFE,           0, "Strafe",},
  {10, DIBUTTON_FPS_MENU,             0, "Menu",},
  {16, DIHATSWITCH_FPS_GLANCE,        0, "Glance",},
  {17, DIBUTTON_FPS_DISPLAY,          0, "Display",},
  {18, DIAXIS_FPS_SIDESTEP,           0, "Sidestep",},
  {19, DIBUTTON_FPS_DODGE,            0, "Dodge",},
  {20, DIBUTTON_FPS_GLANCEL,          0, "Glance left",},
  {21, DIBUTTON_FPS_GLANCER,          0, "Glance right",},
  {22, DIBUTTON_FPS_FIRESECONDARY,    0, "Fire 2",},
  {23, DIBUTTON_FPS_ROTATE_LEFT_LINK, 0, "Rotate left",},
  {24, DIBUTTON_FPS_ROTATE_RIGHT_LINK,0, "Rotate right",},
  {25, DIBUTTON_FPS_FORWARD_LINK,     0, "Forward",},
  {26, DIBUTTON_FPS_BACKWARD_LINK,    0, "Backward",},
  {27, DIBUTTON_FPS_GLANCE_UP_LINK,   0, "Glance up",},
  {28, DIBUTTON_FPS_GLANCE_DOWN_LINK, 0, "Glance down",},
  {29, DIBUTTON_FPS_DEVICE,           0, "Device",},
  {30, DIBUTTON_FPS_PAUSE,            0, "Pause",},
};

static const GUID ACTIONMAP_GUID =
{ 0x0, 0x1, 0x2, { 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa } };

/* This function is a helper to convert a GUID into any possible DInput GUID out there */
const char *_dump_dinput_GUID(const GUID guid) {
    unsigned int i;
    static const struct {
	const GUID guid;
	const char *name;
    } guids[] = {
#define FE(x) { x, #x}
	FE(GUID_XAxis),
	FE(GUID_YAxis),
	FE(GUID_ZAxis),
	FE(GUID_RxAxis),
	FE(GUID_RyAxis),
	FE(GUID_RzAxis),
	FE(GUID_Slider),
	FE(GUID_Button),
	FE(GUID_Key),
	FE(GUID_POV),
	FE(GUID_Unknown),
	FE(GUID_SysMouse),
	FE(GUID_SysKeyboard),
	FE(GUID_Joystick),
	FE(GUID_ConstantForce),
	FE(GUID_RampForce),
	FE(GUID_Square),
	FE(GUID_Sine),
	FE(GUID_Triangle),
	FE(GUID_SawtoothUp),
	FE(GUID_SawtoothDown),
	FE(GUID_Spring),
	FE(GUID_Damper),
	FE(GUID_Inertia),
	FE(GUID_Friction),
	FE(GUID_CustomForce)
#undef FE
    };
    for (i = 0; i < (sizeof(guids) / sizeof(guids[0])); i++) {
	if (IsEqualGUID(guids[i].guid, guid)) {
	    return guids[i].name;
	}
    }
    return debugstr_guid(&guid);
}

void _dump_diactionformatA(LPDIACTIONFORMATA lpdiActionFormat) {
    unsigned int i;

    printf("diaf.dwSize = %d\n", lpdiActionFormat->dwSize);
    printf("diaf.dwActionSize = %d\n", lpdiActionFormat->dwActionSize);
    printf("diaf.dwDataSize = %d\n", lpdiActionFormat->dwDataSize);
    printf("diaf.dwNumActions = %d\n", lpdiActionFormat->dwNumActions);
    printf("diaf.rgoAction = %p\n", lpdiActionFormat->rgoAction);
    printf("diaf.dwGenre = 0x%08x\n", lpdiActionFormat->dwGenre);
    printf("diaf.dwBufferSize = %d\n", lpdiActionFormat->dwBufferSize);
    printf("diaf.lAxisMin = %d\n", lpdiActionFormat->lAxisMin);
    printf("diaf.lAxisMax = %d\n", lpdiActionFormat->lAxisMax);
    printf("diaf.hInstString = %p\n", lpdiActionFormat->hInstString);
    printf("diaf.ftTimeStamp ...\n");
    printf("diaf.dwCRC = 0x%x\n", lpdiActionFormat->dwCRC);
    for (i = 0; i < lpdiActionFormat->dwNumActions; i++)
    {
        printf("diaf.rgoAction[%u]:\n", i);
        printf("\tuAppData=0x%lx\n", lpdiActionFormat->rgoAction[i].uAppData);
        printf("\tdwSemantic=0x%08x\n", lpdiActionFormat->rgoAction[i].dwSemantic);
        printf("\tdwFlags=0x%x\n", lpdiActionFormat->rgoAction[i].dwFlags);
        printf("\tszActionName=%s\n", lpdiActionFormat->rgoAction[i].lptszActionName);
        printf("\tguidInstance=%s\n", debugstr_guid(&lpdiActionFormat->rgoAction[i].guidInstance));
        printf("\tdwObjID=0x%08x\n", lpdiActionFormat->rgoAction[i].dwObjID);
        printf("\tdwHow=0x%x\n", lpdiActionFormat->rgoAction[i].dwHow);
    }
}

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
enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* instance, VOID* context)
{
    printf("%32s ofs: 0x%03x type: 0x%03x %11s", instance->tszName, instance->dwOfs, DIDFT_GETINSTANCE(instance->dwType), _dump_dinput_GUID(instance->guidType));

#define X(x) if (instance->dwType & x) printf(" | "#x"");
    X(DIDFT_RELAXIS)
    X(DIDFT_ABSAXIS)
    X(DIDFT_AXIS)
    X(DIDFT_PSHBUTTON)
    X(DIDFT_TGLBUTTON)
    X(DIDFT_BUTTON)
    X(DIDFT_POV)
    X(DIDFT_COLLECTION)
    X(DIDFT_NODATA)
#undef X

    printf("\n");

    return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    HRESULT hr;
    LPDIRECTINPUTDEVICE8 lpdid;

    if (GET_DIDEVICE_TYPE(instance->dwDevType) == DI8DEVTYPE_MOUSE ||
        GET_DIDEVICE_TYPE(instance->dwDevType) == DI8DEVTYPE_KEYBOARD
    )
        return DIENUM_CONTINUE;

    hr = gp_DI->CreateDevice(instance->guidInstance, &lpdid, NULL);

    if(SUCCEEDED(hr)) {
        _dump_DIDEVICEINSTANCE(instance);
        printf("Objects:\n");
        lpdid->EnumObjects(enumObjectsCallback, NULL, DIDFT_ALL);
    } else {
        printf("Error\n");
        HRNAME(hr);
    }

    return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumSemanticsCallback //Callback function that receive DirectInput devices
(   LPCDIDEVICEINSTANCE lpddi, //Adress of structure describing device instance
    LPDIRECTINPUTDEVICE8 lpdid, //Pointer to device interface
    DWORD dwFlags, //Flags telling why being enumerated
    DWORD dwRemaining, //Number of devices remaining to be enumerated
    LPVOID pvRef) //Application defined value. Always NULL here
{
    HRESULT hr;

    if (GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_MOUSE ||
        GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_KEYBOARD
    )
        return DIENUM_CONTINUE;

    printf("Device %s\n",lpddi->tszProductName);

#define X(x) if (dwFlags & x) printf("\tdwFlags |= "#x"\n");
	X(DIEDBS_MAPPEDPRI1)
	X(DIEDBS_MAPPEDPRI2)
	X(DIEDBS_RECENTDEVICE)
	X(DIEDBS_NEWDEVICE)
#undef X

    hr = lpdid->BuildActionMap(&g_ActionFormat,NULL,DIDBAM_INITIALIZE);
    _dump_diactionformatA(&g_ActionFormat);

    return DIENUM_CONTINUE;

}

#define ARRAYSIZE(a) (sizeof((a)) / sizeof((a)[0]))

int main()
{
    HRESULT hr;

    hr=DirectInput8Create
       (GetModuleHandle (NULL), DIRECTINPUT_VERSION, IID_IDirectInput8A,
       (VOID**)&gp_DI, NULL);

    printf("Devices\n-----------------\n");
    hr = gp_DI->EnumDevices(DI8DEVCLASS_ALL,EnumDevicesCallback, NULL, DIEDFL_ALLDEVICES);


    memset(&g_ActionFormat, 0, sizeof(g_ActionFormat));
    g_ActionFormat.dwSize = sizeof(g_ActionFormat);
    g_ActionFormat.dwActionSize = sizeof(DIACTION);
    g_ActionFormat.dwNumActions = ARRAYSIZE(g_commands_space);
    g_ActionFormat.dwDataSize = 4 * g_ActionFormat.dwNumActions;
    g_ActionFormat.rgoAction = g_commands_space;
    g_ActionFormat.guidActionMap = ACTIONMAP_GUID;
    g_ActionFormat.dwBufferSize = 32;
    g_ActionFormat.dwGenre = DIVIRTUAL_SPACESIM;

    printf("Space combat enumeration\n------------\n");
    hr=gp_DI->EnumDevicesBySemantics
       (NULL, &g_ActionFormat, EnumSemanticsCallback, NULL, DIEDBSFL_AVAILABLEDEVICES );

//    if(FAILED(hr)) printf("Failed?\n");

//    g_ActionFormat.dwNumActions = ARRAYSIZE(g_commands_driving);
//    g_ActionFormat.dwDataSize = 4 * g_ActionFormat.dwNumActions;
//    g_ActionFormat.rgoAction = g_commands_driving;
//    g_ActionFormat.dwGenre = DIVIRTUAL_DRIVING_RACE;

//    printf("Driving race enumeration\n------------\n");
//    hr=gp_DI->EnumDevicesBySemantics
//       (NULL, &g_ActionFormat, EnumSemanticsCallback, NULL, DIEDBSFL_AVAILABLEDEVICES);

//    if(FAILED(hr)) printf("Failed?\n");

//    g_ActionFormat.dwNumActions = ARRAYSIZE(g_commands_fps);
//    g_ActionFormat.dwDataSize = 4 * g_ActionFormat.dwNumActions;
//    g_ActionFormat.rgoAction = g_commands_fps;
//    g_ActionFormat.dwGenre = DIVIRTUAL_FIGHTING_FPS;

//    printf("First person shooter enumeration\n------------\n");
//    hr=gp_DI->EnumDevicesBySemantics
//       (NULL, &g_ActionFormat, EnumSemanticsCallback, NULL, DIEDBSFL_AVAILABLEDEVICES);

//    if(FAILED(hr)) printf("Failed?\n");

    return 0;
}
