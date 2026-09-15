#include "globals.h"

// Global objects

MPU6050 mpu;
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

// Global variables

SensorData data;

bool gasAlert     = false;
bool helmetOff    = false;
bool fallDetected = false;
bool motorOn      = false;
bool inGeoZone    = false;


// Geofencing zones

const GeoZone ZONES[] = {

    {
        "Zone A",
        11.028262,
        77.026765,
        20.0f
    }

    // Add more zones here
    /* Format:
    ,
    {
        "Zone B",
        11.029000,
        77.027500,
        15.0f
    }
    */

};

const int ZONE_COUNT = sizeof(ZONES) / sizeof(ZONES[0]);