#include "devices.h"

int numDevices = 2;

TargetDevice devices[] ={
  {
    "HOME-PC",
    IPAddress(192, 168, 1, 7),
    { 0xA8, 0xA1, 0x59, 0xB8, 0xB2, 0xA2 },
    "",
    5
  },
  // for demo only. You can delete this device
  {
    "UNKNOWN-PC",
    IPAddress(192, 168, 1, 15),
    { 0xA8, 0xA1, 0x59, 0xB8, 0xB2, 0xA1 },
    "",
    5
  },
};
