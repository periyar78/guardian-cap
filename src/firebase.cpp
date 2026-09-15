#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "config.h"
#include "globals.h"
#include "firebase.h"

// Sending to Firebase 

void sendToFirebase() {

    JsonDocument doc;

    doc["gas"]          = data.gas;
    doc["helmetOff"]    = data.helmetOff;

    doc["accelX"]       = serialized(String(data.accelX, 3));
    doc["accelY"]       = serialized(String(data.accelY, 3));
    doc["accelZ"]       = serialized(String(data.accelZ, 3));

    doc["gyroX"]        = serialized(String(data.gyroX, 2));
    doc["gyroY"]        = serialized(String(data.gyroY, 2));
    doc["gyroZ"]        = serialized(String(data.gyroZ, 2));

    doc["tiltX"]        = serialized(String(data.tiltX, 2));
    doc["tiltY"]        = serialized(String(data.tiltY, 2));

    doc["totalAccel"]   = serialized(String(data.totalAccel, 3));

    doc["fallDetected"] = data.fallDetected;

    doc["motorOn"]      = data.motorOn;

    doc["inGeoZone"]    = data.inGeoZone;

    doc["geoZoneName"]  = data.geoZoneName;

    doc["lat"]          = serialized(String(data.lat, 6));
    doc["lng"]          = serialized(String(data.lng, 6));

    doc["altitude"]     = serialized(String(data.altitude, 2));

    doc["heading"]      = serialized(String(data.heading, 2));

    doc["satellites"]   = data.satellites;

    doc["timestamp"]    = data.timestamp;

    String payload;

    serializeJson(doc, payload);

    HTTPClient http;

    http.begin(FIREBASE_URL);

    http.addHeader(
        "Content-Type",
        "application/json"
    );
    int httpCode = http.PUT(payload);

    if (httpCode == HTTP_CODE_OK ||
        httpCode == HTTP_CODE_NO_CONTENT ||
        httpCode == 200) {

    Serial.println("[ FIREBASE ] :) Data sent successfully");

    }
    else {

        Serial.print("[ FIREBASE ] :/ Error: HTTP ");

        Serial.print(httpCode);

        Serial.print(" - ");

        Serial.println(
            http.errorToString(httpCode)
        );

    }

    http.end();

}