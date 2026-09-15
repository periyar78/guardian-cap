#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration 

#define WIFI_SSID        "WIFI_Name"
#define WIFI_PASSWORD    "WIFI_Password"

// Firebase Configuration 

#define FIREBASE_HOST    "Firebase_Host"
#define FIREBASE_API_KEY "API_Key"
#define FIREBASE_PATH    "/helmet/worker_1/latest.json"

#define FIREBASE_URL \
"https://" FIREBASE_HOST FIREBASE_PATH "?auth=" FIREBASE_API_KEY

// Pin Configuration 

#define MQ2_PIN       34
#define IR_PIN        35
#define GPS_RX_PIN    16
#define GPS_TX_PIN    17
#define MPU_SDA       21
#define MPU_SCL       22
#define MOTOR_PIN     25

// Baud Rates 

#define USB_BAUD      115200
#define GPS_BAUD      9600

// Detection Thresholds 

#define GAS_THRESHOLD   2000
#define FALL_THRESHOLD  15.0

// MPU6050 Conversion Factors 

constexpr float ACCEL_SCALE = 16384.0f;
constexpr float GYRO_SCALE  = 131.0f;
constexpr float G_TO_MS2    = 9.81f;

// TIME ZONE 

#define IST_OFFSET_HOURS    5
#define IST_OFFSET_MINUTES  30

#endif