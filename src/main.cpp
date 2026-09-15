#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include "config.h"
#include "globals.h"
#include "sensors.h"
#include "gps.h"
#include "geofence.h"
#include "alerts.h"
#include "firebase.h"

// Setup

void setup() {

    Serial.begin(USB_BAUD);
    delay(3000);

    // WiFi 

    Serial.print("WiFi     : Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);

    delay(2000);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int wifiTries = 0;

    while (WiFi.status() != WL_CONNECTED && wifiTries < 20) {

        delay(500);
        Serial.print(".");

        wifiTries++;

    }

    if (WiFi.status() == WL_CONNECTED) {

        Serial.println(" Connected!");
        Serial.print("IP : ");
        Serial.println(WiFi.localIP());

    }
    else {

        Serial.println(" FAILED - Running Offline");

    }

    // Hardware 

    gpsSerial.begin(
        GPS_BAUD,
        SERIAL_8N1,
        GPS_RX_PIN,
        GPS_TX_PIN
    );

    pinMode(IR_PIN, INPUT);

    pinMode(MOTOR_PIN, OUTPUT);
    digitalWrite(MOTOR_PIN, LOW);

    Wire.begin(MPU_SDA, MPU_SCL);

    mpu.initialize();

    // Boot Screen 

    Serial.println();
    Serial.println("==========================================");
    Serial.println(" Guardian Cap");
    Serial.println(" Smart Safety Helmet");
    Serial.println("==========================================");

    Serial.print("MPU6050 : ");

    Serial.println(
        mpu.testConnection()
        ? "Connected"
        : "FAILED"
    );

    Serial.println("GPS : Waiting for Fix...");

    printGeofenceSummary();

    Serial.println();
    Serial.println("System Ready.");
    Serial.println();

}

// Loop

void loop() {

    while (gpsSerial.available()) {

        gps.encode(gpsSerial.read());

    }

    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate >= 1000) {

        lastUpdate = millis();

        readGasSensor();

        readHelmetStatus();

        readMPU6050();

        readGPS();

        checkGeofence();

        handleAlertsAndMotor();

        printSeparator();

        if (WiFi.status() == WL_CONNECTED) {

            sendToFirebase();

        }
        else {

            WiFi.reconnect();

        }

    }

}