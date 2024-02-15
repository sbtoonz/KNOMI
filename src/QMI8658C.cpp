#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "QMI8658C.h"
#include "pinout.h"

#ifdef QMI8658C_SUPPORT
IMUdata acc;

// Instantiate QMI8658C sensor
extern TwoWire i2c0;
SensorQMI8658 qmi8658c = SensorQMI8658();
int32_t qmi8658c_acc[3];

// QMI8658C sensor task
void qmi8658c_task(void * parameter) {
    Serial.println("\r\n******** QMI8658C startup *****\r\n");
    if (!qmi8658c.begin(i2c0,QMI8658_L_SLAVE_ADDRESS,6,7)) {
        Serial.println("Failed to find QMI8658 - check your wiring!");
        while (1) {
            delay(1000);
        }
    }
    Serial.println("QMI8658C found!");
    Serial.println("\r\n******** QMI8658C init ok *****\r\n");
    qmi8658c.configGyroscope(SensorQMI8658::GYR_RANGE_64DPS,SensorQMI8658::GYR_ODR_896_8Hz,SensorQMI8658::LPF_MODE_3,true);
    qmi8658c.enableGyroscope();

    for (;;) {
if (qmi8658c.getDataReady()) {
        // Get accelerometer data
        if (qmi8658c.getGyroscope(acc.x, acc.y, acc.z)) {
            // Store data in the array
            qmi8658c_acc[0] = acc.x;
            qmi8658c_acc[1] = acc.z;
            qmi8658c_acc[2] = acc.y;
        }
#ifdef QMI8658C_DEBUG
        // Print out the raw data if debug mode is enabled
        Serial.println("\r\n******** QMI8658C data *****\r\n");
        Serial.print("X: "); Serial.println(qmi8658c_acc[0]);
        Serial.print("Y: "); Serial.println(qmi8658c_acc[1]);
        Serial.print("Z: "); Serial.println(qmi8658c_acc[2]);
        Serial.println("\r\n****************************\r\n");
#endif
        delay(100);
    }
    }
}

#endif
