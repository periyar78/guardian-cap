#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#include "config.h"
#include "globals.h"
#include "sensors.h"

// Gas sensor

void readGasSensor() {

    data.gas = analogRead(MQ2_PIN);

    gasAlert = (data.gas > GAS_THRESHOLD);

    Serial.println("[ GAS SENSOR ]");

    Serial.print("  Raw Value  : ");
    Serial.println(data.gas);

    Serial.print("  Status     : ");

    Serial.println(
        gasAlert
        ? "*** GAS ALERT *** DANGEROUS LEVEL DETECTED!"
        : "Normal"
    );

}

// Helmet worn status

void readHelmetStatus() {

    data.helmetOff = digitalRead(IR_PIN);

    helmetOff = data.helmetOff;

    Serial.println("[ HELMET STATUS ]");

    Serial.print("  Helmet : ");

    Serial.println(

        helmetOff

        ? "WARNING - Helmet NOT worn!"

        : "Worn correctly"

    );

}

// Accelerometer - fall detection

void readMPU6050() {

    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    mpu.getMotion6(
        &ax,
        &ay,
        &az,
        &gx,
        &gy,
        &gz
    );

    data.accelX = (ax / ACCEL_SCALE) * G_TO_MS2;
    data.accelY = (ay / ACCEL_SCALE) * G_TO_MS2;
    data.accelZ = (az / ACCEL_SCALE) * G_TO_MS2;

    data.gyroX = gx / GYRO_SCALE;
    data.gyroY = gy / GYRO_SCALE;
    data.gyroZ = gz / GYRO_SCALE;

    data.tiltX =
        atan2(
            data.accelY,
            data.accelZ
        ) * 180.0 / PI;

    data.tiltY =
        atan2(
            -data.accelX,
            sqrt(
                data.accelY * data.accelY +
                data.accelZ * data.accelZ
            )
        ) * 180.0 / PI;

    data.totalAccel =
        sqrt(
            data.accelX * data.accelX +
            data.accelY * data.accelY +
            data.accelZ * data.accelZ
        );

    data.fallDetected =
        (
            data.totalAccel < FALL_THRESHOLD * 0.3f ||

            data.totalAccel > FALL_THRESHOLD
        );

    fallDetected = data.fallDetected;

    Serial.println("[ MPU6050 - ACCELEROMETER ]");

    Serial.print("  Accel X : ");
    Serial.print(data.accelX, 2);
    Serial.println(" m/s²");

    Serial.print("  Accel Y : ");
    Serial.print(data.accelY, 2);
    Serial.println(" m/s²");

    Serial.print("  Accel Z : ");
    Serial.print(data.accelZ, 2);
    Serial.println(" m/s²");

    Serial.println("[ MPU6050 - GYROSCOPE ]");

    Serial.print("  Gyro X : ");
    Serial.print(data.gyroX, 2);
    Serial.println(" °/s");

    Serial.print("  Gyro Y : ");
    Serial.print(data.gyroY, 2);
    Serial.println(" °/s");

    Serial.print("  Gyro Z : ");
    Serial.print(data.gyroZ, 2);
    Serial.println(" °/s");

    Serial.println("[ TILT ANGLES ]");

    Serial.print("  Roll  : ");
    Serial.print(data.tiltX, 2);
    Serial.println("°");

    Serial.print("  Pitch : ");
    Serial.print(data.tiltY, 2);
    Serial.println("°");

    Serial.println("[ FALL DETECTION ]");

    Serial.print("  Total Acceleration : ");
    Serial.print(data.totalAccel, 2);
    Serial.println(" m/s²");

    Serial.print("  Status : ");

    Serial.println(

        fallDetected

        ? "*** FALL DETECTED ***"

        : "Normal"

    );

}