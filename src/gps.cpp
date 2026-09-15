#include <Arduino.h>

#include "config.h"
#include "globals.h"
#include "gps.h"

// Reading GPS data

void readGPS() {

    

    data.lat =
        gps.location.isValid()
        ? gps.location.lat()
        : 0.0;

    data.lng =
        gps.location.isValid()
        ? gps.location.lng()
        : 0.0;

    data.altitude =
        gps.altitude.isValid()
        ? gps.altitude.meters()
        : 0.0;

    data.heading =
        gps.course.isValid()
        ? gps.course.deg()
        : 0.0;

    data.satellites =
        gps.satellites.isValid()
        ? gps.satellites.value()
        : 0;

    Serial.println("[ GPS - NEO-6M ]");

    Serial.print("  Latitude   : ");

    if (gps.location.isValid()) {
        Serial.print(data.lat, 6);
        Serial.println(" °");
    }
    else {
        Serial.println("Acquiring...");
    }

    Serial.print("  Longitude  : ");

    if (gps.location.isValid()) {
        Serial.print(data.lng, 6);
        Serial.println(" °");
    }
    else {
        Serial.println("Acquiring...");
    }

    Serial.print("  Altitude   : ");

    if (gps.altitude.isValid()) {
        Serial.print(data.altitude, 2);
        Serial.println(" m");
    }
    else {
        Serial.println("N/A");
    }

    Serial.print("  Heading    : ");

    if (gps.course.isValid()) {

        Serial.print(data.heading, 2);
        Serial.print(" ° (");
        Serial.print(getDirection(data.heading));
        Serial.println(")");

    }
    else {

        Serial.println("N/A (stationary)");

    }
    Serial.print("  Date (IST) : ");

    if (gps.date.isValid() && gps.time.isValid()) {

        int istMin  = gps.time.minute() + IST_OFFSET_MINUTES;
        int istHour = gps.time.hour() + IST_OFFSET_HOURS + (istMin / 60);

        istMin  = istMin % 60;

        int day = gps.date.day() + (istHour >= 24 ? 1 : 0);
        istHour = istHour % 24;

        printZeroPadded(day);
        Serial.print("/");

        printZeroPadded(gps.date.month());
        Serial.print("/");

        Serial.println(gps.date.year());

    }
    else {

        Serial.println("N/A");

    }

    Serial.print("  Time (IST) : ");

    if (gps.time.isValid()) {

        int istMin  = gps.time.minute() + IST_OFFSET_MINUTES;

        int istHour =
            (gps.time.hour() +
            IST_OFFSET_HOURS +
            (istMin / 60)) % 24;

        istMin = istMin % 60;

        printZeroPadded(istHour);
        Serial.print(":");

        printZeroPadded(istMin);
        Serial.print(":");

        printZeroPadded(gps.time.second());

        Serial.println(" IST");

    }
    else {

        Serial.println("N/A");

    }

    Serial.print("  Satellites : ");

    if (gps.satellites.isValid()) {

        Serial.println(data.satellites);

    }
    else {

        Serial.println("N/A");

    }

    Serial.print("  Maps Link  : ");

    if (gps.location.isValid()) {

        Serial.print("https://maps.google.com/?q=");

        Serial.print(data.lat, 6);

        Serial.print(",");

        Serial.println(data.lng, 6);

    }
    else {

        Serial.println("Waiting for fix...");

    }

    if (millis() > 15000 && gps.charsProcessed() < 10) {

        Serial.println("  ERROR: No GPS data! Check TX - GPIO16 wiring.");

    }

}

// GPS direction

const char* getDirection(float deg) {

    if (deg >= 337.5 || deg < 22.5)
        return "North";

    else if (deg < 67.5)
        return "North-East";

    else if (deg < 112.5)
        return "East";

    else if (deg < 157.5)
        return "South-East";

    else if (deg < 202.5)
        return "South";

    else if (deg < 247.5)
        return "South-West";

    else if (deg < 292.5)
        return "West";

    else
        return "North-West";

}

// Zero padding for co-ordinates

void printZeroPadded(int val) {

    if (val < 10)
        Serial.print("0");

    Serial.print(val);

}