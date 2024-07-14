// Contains all devices in networking
#ifndef DEVICES_H
#define DEVICES_H

#include <ESP8266Ping.h>
#include <Arduino.h>

// Add or remove devices from this list
// To get mac address on windows:
// - press Win+R
// - type "cmd" and press Enter
// - type "ipconfig /all" and press Enter
// - find and copy value "physical address" for your network interface to mac address (add 0x prefixes)
// TargetDevice info
struct TargetDevice {
  String name; // device name
  IPAddress ipAddr; // IP address (class)
  byte mac[6]; // MAC address
  String tCallbackQueryId; // telegram callback
  int rCounter; // count fail retries
};

// devices contain available devices
extern TargetDevice devices[];

// numDevices change to match how many devices are in the above array.
extern int numDevices;

#endif