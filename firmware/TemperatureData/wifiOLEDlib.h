#ifndef WIFIOLEDLIB_H
#define WIFIOLEDLIB_H

#include <Arduino.h>

void convHHMMSS(unsigned long currSeconds, char *uptimeHHMMSS);
void convDDHHMMSS(unsigned long currSeconds, char *uptimeDDHHMMSS);
void convCurrentTimeET(unsigned long currSeconds, char *currentTimeET);
void getMacWifiShield(char *macWifiShield);
void displayTextOLED(String oledline[]);

#endif
