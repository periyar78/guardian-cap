#include <Arduino.h>
#include <TinyGPS++.h>

#include "globals.h"
#include "geofence.h"

// Geofencing summary

void printGeofenceSummary() {

    Serial.println("------------------------------------------");

    if (ZONE_COUNT == 0) {

        Serial.println("Geofence : No zones defined - Feature inactive");
        return;

    }

    Serial.print("Geofence : ");
    Serial.print(ZONE_COUNT);
    Serial.println(" zone(s) active");

    for (int i = 0; i < ZONE_COUNT; i++) {

        Serial.print("  Zone ");
        Serial.print(i + 1);
        Serial.print(": ");

        Serial.print(ZONES[i].name);

        Serial.print("  Radius = ");

        Serial.print(ZONES[i].radius);

        Serial.println(" m");

    }

}

// Geofence checking

void checkGeofence() {

    inGeoZone = false;

    data.inGeoZone = false;

    data.geoZoneName = "None";

    if (ZONE_COUNT == 0 || !gps.location.isValid())
        return;

    for (int i = 0; i < ZONE_COUNT; i++) {

        double distance =
            TinyGPSPlus::distanceBetween(

                gps.location.lat(),
                gps.location.lng(),

                ZONES[i].lat,
                ZONES[i].lng

            );

        if (distance <= ZONES[i].radius) {

            inGeoZone = true;

            data.inGeoZone = true;

            data.geoZoneName = ZONES[i].name;

            Serial.print("[ GEOFENCE ] ENTERED : ");

            Serial.print(ZONES[i].name);

            Serial.print("  Distance : ");

            Serial.print(distance, 1);

            Serial.println(" m");

            break;

        }

    }

    if (!inGeoZone && ZONE_COUNT > 0) {

        Serial.println("[ GEOFENCE ] Outside all zones");

    }

}