#include <Arduino.h>

#include "config.h"
#include "globals.h"
#include "alerts.h"

// Alerts with Vibration motor

void handleAlertsAndMotor() {

    motorOn = (gasAlert || fallDetected || inGeoZone);

    data.motorOn = motorOn;

    digitalWrite(MOTOR_PIN, motorOn ? HIGH : LOW);

    data.timestamp = millis();

    Serial.println("[ ALERTS ]");

    if (gasAlert)
        Serial.println("  *** GAS ALERT > DANGEROUS LEVEL! ***");

    if (fallDetected)
        Serial.println("  *** FALL DETECTED > WORKER DOWN! ***");

    if (helmetOff)
        Serial.println("  *** HELMET NOT WORN > Please wear helmet! ***");

    if (inGeoZone) {

        Serial.print("  *** GEOFENCE ALERT > ENTERED: ");

        Serial.print(data.geoZoneName);

        Serial.println(" ***");

    }

    if (!gasAlert &&
        !fallDetected &&
        !helmetOff &&
        !inGeoZone) {

        Serial.println("  All Clear - No alerts");

    }

    Serial.print("[ VIBRATION MOTOR ] ");

    Serial.println(

        motorOn

        ? "ON (Alerting Worker)"

        : "OFF"

    );

}

// Serial lining separation

void printSeparator() {

    Serial.println(
        "-----------------------------------"
    );

}