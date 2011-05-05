
enum eGameActions
{
    eA_STEER,       // Steering
    eB_STEER_LEFT,  // Steer left
    eB_STEER_RIGHT, // Steer right
    eA_ACCELERATE,  // Change speed
    eB_ACCELERATE,  // Speed up
    eB_DECELERATE,  // Slow down
    eA_BRAKE,       // Brake
    eB_BRAKE,       // Brake
    eB_UPSHIFT,     // Shift to higher gear
    eB_DOWNSHIFT,   // Shift to lower gear
    eB_CYCLEVIEW,   // Cycle to next view
    eB_COURSEVIEW,  // Toggle course view
    eB_DRIVERVIEW,  // View from driver's seat
    eB_BRAKEBIAS,   // Brake bias
    eA_VOLUME,      // Sound volume
    eB_MUTE,         // Toggle sound
    NUM_MY_ACTIONS
};

static DIACTION g_commands[]=
{
//Genre-defined virtual axes

  {eA_STEER,       DIAXIS_DRIVINGR_STEER,        0, "Steer",      },
  {eA_ACCELERATE,  DIAXIS_DRIVINGR_ACCELERATE,   0, "Accelerate", },
  {eA_BRAKE,       DIAXIS_DRIVINGR_BRAKE,        0, "Brake",      },

//Genre-defined virtual buttons

  {eB_UPSHIFT,     DIBUTTON_DRIVINGR_SHIFTUP,    0, "Upshift",    },
  {eB_DOWNSHIFT,   DIBUTTON_DRIVINGR_SHIFTDOWN,  0, "DownShift",  },
  {eB_CYCLEVIEW,   DIBUTTON_DRIVINGR_VIEW,       0, "Change View",},

// Actions not defined in the genre that can be assigned to any
// button or axis

  {eA_VOLUME,      DIAXIS_ANY_1,                 0, "Volume",      },
  {eB_MUTE,        DIBUTTON_ANY(0),              0, "Toggle Sound",},

// Actions not defined in the genre that must be assigned to
// particular keys

  {eB_DRIVERVIEW,  DIKEYBOARD_1,                 0, "Driver View",},
  {eB_COURSEVIEW,  DIKEYBOARD_C,                 0, "Course View",},
  {eB_BRAKEBIAS,   DIKEYBOARD_B,                 0, "Brake Bias", },

// Actions mapped to keys as well as to virtual controls

  {eB_UPSHIFT,     DIKEYBOARD_PRIOR,              0, "Upshift",    },
  {eB_DOWNSHIFT,   DIKEYBOARD_NEXT,               0, "Downshift",  },
  {eB_STEER_LEFT,  DIKEYBOARD_LEFT,               0, "Steer Left", },
  {eB_STEER_RIGHT, DIKEYBOARD_RIGHT,              0, "Steer Right",},
  {eB_ACCELERATE,  DIKEYBOARD_UP,                 0, "Accelerate", },
  {eB_DECELERATE,  DIKEYBOARD_DOWN,               0, "Decelerate", },
  {eB_BRAKE,       DIKEYBOARD_END,                0, "Brake",      },

// Actions mapped to buttons as well as to virtual controls and keys

  {eB_UPSHIFT,     DIMOUSE_BUTTON0,              0, "Upshift",   },
  {eB_DOWNSHIFT,   DIMOUSE_BUTTON1,              0, "Downshift", },
};

static const GUID ACTIONMAP_GUID =
{ 0x0, 0x1, 0x2, { 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa } };
