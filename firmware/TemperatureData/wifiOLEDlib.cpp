#include <time.h>
#include "wifiOLEDlib.h"
#include <WiFiNINA.h>


void convHHMMSS(unsigned long currSeconds, char *uptimeHHMMSS) 
{
  int hh, mm, ss;

  hh = (currSeconds/3600);
  mm = (currSeconds -(3600*hh))/60;
  ss = (currSeconds -(3600*hh)-(mm*60));
  sprintf(uptimeHHMMSS, "Time: %02d:%02d:%02d", hh, mm, ss);

};

 
void convDDHHMMSS(unsigned long currSeconds, char *uptimeDDHHMMSS) 
{
  sprintf(uptimeDDHHMMSS, "Uptime: %02.0f %02.0f:%02.0f:%02.0f", floor(currSeconds/86400.0), floor(currSeconds/3600.0), floor(fmod(currSeconds,3600.0)/60.0), fmod(currSeconds,60.0));

};

void convCurrentTimeET(unsigned long currSeconds, char *currentTimeET) 
{
    time_t rawtime = currSeconds - 14400;
    struct tm  ts;
    char buf[70];

    ts = *localtime(&rawtime);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
    sprintf(currentTimeET, buf);

};

void getMacWifiShield(char *macWifiShield) 
{
    byte mac[6];

    WiFi.macAddress(mac);
    sprintf(macWifiShield, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);

};
