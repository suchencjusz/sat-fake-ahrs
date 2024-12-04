#include <Arduino.h>
#include <Wire.h>
#include <LSM6.h>

LSM6 imu;

void device_status(int status)
{
  if (status == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
}

void get_imu_data()
{
  imu.read();
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

  imu.enableDefault();
}

void loop()
{
  get_imu_data();
  delay(100);

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