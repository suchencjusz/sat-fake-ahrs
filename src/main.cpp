#include <Arduino.h>
#include <Wire.h>
#include <LSM6.h>
#include <LIS3MDL.h>

LIS3MDL mag;
LSM6 imu;

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

  Serial.print(imu.g.x);
  Serial.print(",");
  Serial.print(imu.g.y);
  Serial.print(",");
  Serial.print(imu.g.z);
  Serial.print(",");
  Serial.print(imu.a.x);
  Serial.print(",");
  Serial.print(imu.a.y);
  Serial.print(",");
  Serial.print(imu.a.z);
  Serial.print(",");

  Serial.print(mag.m.x);
  Serial.print(",");
  Serial.print(mag.m.y);
  Serial.print(",");
  Serial.print(mag.m.z);

#ifdef DEBUG
  Serial.print(",");
  Serial.print(__data_sent);
#endif

  Serial.println();
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
  delay(1000/60);

  // bajer do resetowania esp jakby cos walnelo (padnie polaczenie z imu przez te smieszne kable)
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("reset"))
    {
      Serial.println("Restarting...");
      ESP.restart();
    }
  }
}