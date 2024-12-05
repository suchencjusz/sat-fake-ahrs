#include <Arduino.h>
#include <Wire.h>
#include <LSM6.h>
#include <LIS3MDL.h>

LIS3MDL mag;
LSM6 imu;

char report[256];

// #define DEBUG

#ifdef DEBUG
unsigned long __data_sent = 0;
#endif

void device_status(int status)
{
  // 1 = imu failed
  // 2 = mag failed

  if (status == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }

  if (status == 2)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

void get_data()
{
#ifdef DEBUG
  __data_sent++;
#endif

  imu.read();
  mag.read();

  snprintf(report, sizeof(report), "%6d %6d %6d %6d %6d %6d %6d %6d %6d",
           imu.a.x, imu.a.y, imu.a.z,
           imu.g.x, imu.g.y, imu.g.z,
           mag.m.x, mag.m.y, mag.m.z);

  Serial.println(report);

#ifdef DEBUG
  Serial.print(",");
  Serial.print(__data_sent);
#endif
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);

  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1)
    {
      device_status(1);
    }
  }

  if (!mag.init())
  {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1)
    {
      device_status(2);
    }
  }

  imu.enableDefault();
  mag.enableDefault();
}

void loop()
{

  get_data();
  // if (Serial.available() > 0)
  // {
  //   String command = Serial.readStringUntil('\n');
  //   command.trim();

  //   if (command.equalsIgnoreCase("dupa"))
  //   {
  //     get_data();
  //   }
  // }
}
