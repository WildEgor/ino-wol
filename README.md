# ino-wol

ESP8266 wake-on-lan any LAN device via Telegram Bot!

## Usage

- Create and edit bot token and network settings in `src/secrets.cpp`;
```cpp
#include "secrets.h"

String telegramToken = ""; // your bot token

char ssid[] = ""; // your network SSID (name)
char password[] = ""; // your network password
```

- Create and edit devices in `src/devices.cpp`;
```cpp
#include "devices.h"

int numDevices = 2;

TargetDevice devices[] ={
  {
    "PC", // any device name (will show in list of devices)
    IPAddress(192, 168, 1, 2),
    { 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1 }, // device MAC address in hex format
    "", // empty state
    5 // default retry counter
  },
};
```

- Upload `src/main.cpp` to ESP8266 (using PlatformIO);
- [More details](https://wildegor.github.io/posts/WOL/)
