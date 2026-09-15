#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <config.h>

// Geo zone 

struct GeoZone {
    const char* name;
    double lat;
    double lng;
    float radius;
};

// Sensor Data
struct SensorData {

    int gas;
    
    bool helmetOff;

    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
    float tiltX;
    float tiltY;
    float totalAccel;
    bool fallDetected;
    bool motorOn;

    bool inGeoZone;
    const char* geoZoneName;
    double lat;
    double lng;
    double altitude;
    float heading;
    int satellites;

    unsigned long timestamp;
};

// Global Objects


extern MPU6050 mpu;
extern TinyGPSPlus gps;
extern HardwareSerial gpsSerial;

// Global variables

extern SensorData data;

extern bool gasAlert;
extern bool helmetOff;
extern bool fallDetected;
extern bool motorOn;
extern bool inGeoZone;

// Geofence variables

extern const GeoZone ZONES[];
extern const int ZONE_COUNT;

#endif